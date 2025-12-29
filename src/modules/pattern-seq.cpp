#include "pattern-seq.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  /*
   * Declarations
   */

  class PatternSeqScreen : public ui::ModuleScreen<PatternSeq> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_pattern_grid(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<PatternSeq>::ModuleScreen;
  };

  /*
   * PatternSeq - Step sequencer
   */

  PatternSeq::PatternSeq() :
    SequencerModule(&props),
    screen (new PatternSeqScreen(this)) {}

  void PatternSeq::init() {
    currentStep = 0;
    samplesUntilNextStep = 0;
    noteIsOn = false;
    editStep = 0;
  }

  void PatternSeq::display() {
    Globals::ui.display(*screen);
  }

  void PatternSeq::advanceStep(audio::ProcessData& data) {
    // Turn off previous note if still on
    if (noteIsOn) {
      auto* noteOffEvt = new midi::NoteOffEvent(midi::MidiEvent{
        midi::MidiEvent::NOTE_OFF, 0, nullptr, 0
      });
      noteOffEvt->key = props.notes[currentStep];
      noteOffEvt->velocity = 64;
      data.midi.push_back(noteOffEvt);
      noteIsOn = false;
    }
    
    // Check if current step has a note (velocity > 0)
    if (currentStep < props.length && props.velocities[currentStep] > 0) {
      // Send note on
      auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
        midi::MidiEvent::NOTE_ON, 0, nullptr, 0
      });
      noteOnEvt->key = props.notes[currentStep];
      noteOnEvt->velocity = props.velocities[currentStep];
      data.midi.push_back(noteOnEvt);
      noteIsOn = true;
      
      // Calculate when to send note off (gate length)
      float bpm = Globals::metronome.props.bpm;
      float beatsPerStep = 4.0f / props.length; // Assuming 4 beats per bar
      float secondsPerStep = (60.0f / bpm) * beatsPerStep;
      float gateLength = secondsPerStep * props.gate;
      samplesUntilNoteOff = static_cast<int>(gateLength * Globals::samplerate);
    }
    
    // Advance to next step
    currentStep = (currentStep + 1) % props.length;
  }

  void PatternSeq::process(const audio::ProcessData& data) {
    float bpm = Globals::metronome.props.bpm;
    float beatsPerStep = 4.0f / props.length; // Assuming 4 beats per bar
    float secondsPerStep = (60.0f / bpm) * beatsPerStep;
    int samplesPerStep = static_cast<int>(secondsPerStep * Globals::samplerate);
    
    for (uint f = 0; f < data.nframes; f++) {
      // Check if we need to turn off the note
      if (noteIsOn && samplesUntilNoteOff > 0) {
        samplesUntilNoteOff--;
        if (samplesUntilNoteOff == 0) {
          noteIsOn = false;
        }
      }
      
      // Check if it's time for next step
      samplesUntilNextStep--;
      if (samplesUntilNextStep <= 0) {
        advanceStep(const_cast<audio::ProcessData&>(data));
        samplesUntilNextStep = samplesPerStep;
      }
    }
  }

  /*
   * PatternSeqScreen
   */

  using namespace ui::drawing;

  bool PatternSeqScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_BLUE_CLICK:
      // Toggle step on/off
      if (module->props.velocities[module->editStep] > 0) {
        module->props.velocities[module->editStep] = 0;
      } else {
        module->props.velocities[module->editStep] = 80;
      }
      return true;
    case ui::K_GREEN_CLICK:
      // Cycle through octaves
      module->props.notes[module->editStep] = (module->props.notes[module->editStep] + 12) % 128;
      return true;
    default:
      return false;
    }
  }

  void PatternSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      // Change which step to edit
      module->editStep = (module->editStep + e.clicks + 32) % 32;
      if (module->editStep >= module->props.length) {
        module->editStep = module->props.length - 1;
      }
      break;
    case ui::Rotary::Green:
      // Change note pitch
      module->props.notes[module->editStep] = std::clamp(
        module->props.notes[module->editStep] + e.clicks,
        0, 127
      );
      break;
    case ui::Rotary::White:
      // Change pattern length
      module->props.length.step(e.clicks);
      if (module->editStep >= module->props.length) {
        module->editStep = module->props.length - 1;
      }
      break;
    case ui::Rotary::Red:
      // Change velocity
      if (module->props.velocities[module->editStep] == 0) {
        module->props.velocities[module->editStep] = 80;
      } else {
        module->props.velocities[module->editStep] = std::clamp(
          module->props.velocities[module->editStep] + e.clicks * 5,
          1, 127
        );
      }
      break;
    }
  }

  void PatternSeqScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_pattern_grid(ctx);
  }

  void PatternSeqScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18.f);
    ctx.fillStyle(Colours::White);
    ctx.fillText("PATTERN", 160, 20);

    // Length
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);
    ctx.fillText(fmt::format("Length: {}", module->props.length.get()), 10, 10);
    
    // Edit step info
    int editStep = module->editStep;
    const char* noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    int note = module->props.notes[editStep];
    int octave = (note / 12) - 1;
    int noteIdx = note % 12;
    int velocity = module->props.velocities[editStep];
    
    ctx.font(Fonts::Bold);
    ctx.font(24.f);
    ctx.fillStyle(Colours::Green);
    ctx.textAlign(TextAlign::Right, TextAlign::Top);
    ctx.fillText(fmt::format("Step {}", editStep + 1), 310, 10);
    
    ctx.font(Fonts::Bold);
    ctx.font(32.f);
    if (velocity > 0) {
      ctx.fillStyle(Colours::Green);
      ctx.textAlign(TextAlign::Right, TextAlign::Bottom);
      ctx.fillText(fmt::format("{}{}", noteNames[noteIdx], octave), 310, 230);
    } else {
      ctx.fillStyle(Colours::Gray60);
      ctx.textAlign(TextAlign::Right, TextAlign::Bottom);
      ctx.fillText("---", 310, 230);
    }

    ctx.restore();
  }

  void PatternSeqScreen::draw_pattern_grid(Canvas& ctx) {
    ctx.save();
    
    // Draw step sequencer grid
    float gridX = 20;
    float gridY = 50;
    float stepWidth = 16;
    float stepHeight = 120;
    
    int length = module->props.length;
    int currentStep = module->currentStep;
    int editStep = module->editStep;
    
    // Draw note range reference
    float minNote = 127;
    float maxNote = 0;
    for (int i = 0; i < length; i++) {
      if (module->props.velocities[i] > 0) {
        minNote = std::min(minNote, (float)module->props.notes[i]);
        maxNote = std::max(maxNote, (float)module->props.notes[i]);
      }
    }
    if (maxNote < minNote) {
      minNote = 48;
      maxNote = 72;
    }
    float noteRange = std::max(12.0f, maxNote - minNote + 12);
    
    // Draw grid background
    ctx.strokeStyle(Colours::Gray60.dim(0.5));
    ctx.lineWidth(1.0f);
    ctx.beginPath();
    ctx.rect(gridX, gridY, std::min(16, length) * stepWidth, stepHeight);
    ctx.stroke();
    
    // Draw steps
    for (int i = 0; i < std::min(16, length); i++) {
      float x = gridX + i * stepWidth;
      int velocity = module->props.velocities[i];
      
      // Step background
      if (i == currentStep) {
        ctx.fillStyle(Colours::White.dim(0.2));
        ctx.beginPath();
        ctx.rect(x, gridY, stepWidth, stepHeight);
        ctx.fill();
      }
      
      if (i == editStep) {
        ctx.strokeStyle(Colours::Blue);
        ctx.lineWidth(2.0f);
        ctx.beginPath();
        ctx.rect(x + 1, gridY + 1, stepWidth - 2, stepHeight - 2);
        ctx.stroke();
      }
      
      // Draw note if active
      if (velocity > 0) {
        int note = module->props.notes[i];
        float noteY = gridY + stepHeight - ((note - minNote) / noteRange) * stepHeight;
        float barHeight = (velocity / 127.0f) * 30 + 5;
        
        ctx.fillStyle(Colours::Green);
        ctx.beginPath();
        ctx.rect(x + 2, noteY - barHeight/2, stepWidth - 4, barHeight);
        ctx.fill();
      }
    }
    
    ctx.restore();
  }

} // namespace top1::modules
