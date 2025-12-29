#include "euclidean-seq.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  /*
   * Declarations
   */

  class EuclideanSeqScreen : public ui::ModuleScreen<EuclideanSeq> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_circular_pattern(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<EuclideanSeq>::ModuleScreen;
  };

  /*
   * EuclideanSeq - Euclidean rhythm generator
   */

  EuclideanSeq::EuclideanSeq() :
    SequencerModule(&props),
    screen (new EuclideanSeqScreen(this)) {}

  void EuclideanSeq::init() {
    currentStep = 0;
    samplesUntilNextStep = 0;
    noteIsOn = false;
  }

  void EuclideanSeq::display() {
    Globals::ui.display(*screen);
  }

  // Bjorklund's algorithm for Euclidean rhythms
  std::vector<bool> EuclideanSeq::generatePattern() {
    int length = std::max(1, props.length.get());
    int pulses = std::clamp(props.pulses.get(), 0, length);
    int offset = props.offset.get() % length;
    
    std::vector<bool> pattern(length, false);
    
    if (pulses == 0) return pattern;
    if (pulses == length) {
      std::fill(pattern.begin(), pattern.end(), true);
      return pattern;
    }
    
    // Bjorklund's algorithm
    std::vector<std::vector<int>> groups;
    
    // Initialize with pulses and silences
    for (int i = 0; i < pulses; i++) {
      groups.push_back({1});
    }
    for (int i = 0; i < length - pulses; i++) {
      groups.push_back({0});
    }
    
    // Distribute until we can't anymore
    while (groups.size() > 1) {
      int minSize = std::min(pulses, (int)groups.size() - pulses);
      if (minSize == 0) break;
      
      // Append second half to first half
      for (int i = 0; i < minSize; i++) {
        auto& back = groups.back();
        groups[i].insert(groups[i].end(), back.begin(), back.end());
        groups.pop_back();
      }
      
      pulses = minSize;
    }
    
    // Flatten groups into pattern
    int idx = 0;
    for (auto& group : groups) {
      for (int val : group) {
        if (idx < length) {
          pattern[idx++] = (val == 1);
        }
      }
    }
    
    // Apply rotation/offset
    if (offset > 0) {
      std::rotate(pattern.begin(), pattern.begin() + offset, pattern.end());
    }
    
    return pattern;
  }

  void EuclideanSeq::advanceStep(audio::ProcessData& data) {
    auto pattern = generatePattern();
    
    // Turn off previous note if still on
    if (noteIsOn) {
      auto* noteOffEvt = new midi::NoteOffEvent(midi::MidiEvent{
        midi::MidiEvent::NOTE_OFF, 0, nullptr, 0
      });
      noteOffEvt->key = props.note;
      noteOffEvt->velocity = 64;
      data.midi.push_back(noteOffEvt);
      noteIsOn = false;
    }
    
    // Check if current step should trigger
    if (pattern[currentStep]) {
      // Send note on
      auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
        midi::MidiEvent::NOTE_ON, 0, nullptr, 0
      });
      noteOnEvt->key = props.note;
      noteOnEvt->velocity = 100;
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

  void EuclideanSeq::process(const audio::ProcessData& data) {
    float bpm = Globals::metronome.props.bpm;
    float beatsPerStep = 4.0f / props.length; // Assuming 4 beats per bar
    float secondsPerStep = (60.0f / bpm) * beatsPerStep;
    int samplesPerStep = static_cast<int>(secondsPerStep * Globals::samplerate);
    
    for (uint f = 0; f < data.nframes; f++) {
      // Check if we need to turn off the note
      if (noteIsOn && samplesUntilNoteOff > 0) {
        samplesUntilNoteOff--;
        if (samplesUntilNoteOff == 0) {
          // Note: We can't add MIDI events mid-frame easily, so we'll handle this next frame
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
   * EuclideanSeqScreen
   */

  using namespace ui::drawing;

  bool EuclideanSeqScreen::keypress(ui::Key key) {
    return false;
  }

  void EuclideanSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.length.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.pulses.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.offset.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.note.step(e.clicks);
      break;
    }
  }

  void EuclideanSeqScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_circular_pattern(ctx);
  }

  void EuclideanSeqScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18.f);
    ctx.fillStyle(Colours::White);
    ctx.fillText("EUCLIDEAN", 160, 20);

    // Parameter labels and values
    ctx.font(Fonts::Bold);
    ctx.font(24.f);
    
    // Length
    ctx.fillStyle(Colours::Green);
    ctx.textAlign(TextAlign::Right, TextAlign::Middle);
    ctx.fillText("length", 280, 50);
    ctx.font(Fonts::Bold);
    ctx.font(40.f);
    ctx.fillText(fmt::format("{}", module->props.length.get()), 310, 50);
    
    // Pulses
    ctx.font(Fonts::Bold);
    ctx.font(24.f);
    ctx.fillStyle(Colours::Red);
    ctx.textAlign(TextAlign::Right, TextAlign::Middle);
    ctx.fillText("pulses", 280, 180);
    ctx.font(Fonts::Bold);
    ctx.font(40.f);
    ctx.fillText(fmt::format("{}", module->props.pulses.get()), 310, 180);
    
    // Offset
    ctx.font(Fonts::Bold);
    ctx.font(24.f);
    ctx.fillStyle(Colours::Blue);
    ctx.textAlign(TextAlign::Left, TextAlign::Middle);
    ctx.fillText("offset", 10, 180);
    ctx.font(Fonts::Bold);
    ctx.font(40.f);
    ctx.fillText(fmt::format("{}", module->props.offset.get()), 10, 210);

    ctx.restore();
  }

  void EuclideanSeqScreen::draw_circular_pattern(Canvas& ctx) {
    ctx.save();
    
    // Draw circular pattern like reference image
    float centerX = 130;
    float centerY = 120;
    float radius = 70;
    
    auto pattern = module->generatePattern();
    int length = module->props.length;
    int currentStep = module->currentStep;
    
    // Draw outer circle
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(2.0f);
    ctx.beginPath();
    ctx.circle(centerX, centerY, radius);
    ctx.stroke();
    
    // Draw steps
    for (int i = 0; i < length; i++) {
      float angle = (2.0f * M_PI * i / length) - M_PI / 2.0f; // Start at top
      float x = centerX + std::cos(angle) * radius;
      float y = centerY + std::sin(angle) * radius;
      
      // Determine color based on pulse/offset
      Colour color;
      if (i == currentStep) {
        color = Colours::White; // Current step
      } else if (pattern[i]) {
        color = Colours::Red; // Pulse step
      } else {
        color = Colours::Blue.dim(0.5); // Empty step
      }
      
      float dotSize = pattern[i] ? 8.0f : 4.0f;
      if (i == currentStep) dotSize = 10.0f;
      
      ctx.beginPath();
      ctx.circle(x, y, dotSize);
      ctx.fillStyle(color);
      ctx.fill();
      
      // Draw connecting lines for pulses
      if (pattern[i] && i > 0) {
        // Find previous pulse
        int prevPulse = i - 1;
        while (prevPulse >= 0 && !pattern[prevPulse]) {
          prevPulse--;
        }
        if (prevPulse >= 0) {
          float prevAngle = (2.0f * M_PI * prevPulse / length) - M_PI / 2.0f;
          float prevX = centerX + std::cos(prevAngle) * (radius - 15);
          float prevY = centerY + std::sin(prevAngle) * (radius - 15);
          float currX = centerX + std::cos(angle) * (radius - 15);
          float currY = centerY + std::sin(angle) * (radius - 15);
          
          ctx.strokeStyle(Colours::Green.dim(0.3));
          ctx.lineWidth(1.5f);
          ctx.beginPath();
          ctx.moveTo(prevX, prevY);
          ctx.lineTo(currX, currY);
          ctx.stroke();
        }
      }
    }
    
    ctx.restore();
  }

} // namespace top1::modules
