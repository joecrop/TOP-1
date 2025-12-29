#include "arpeggiator-seq.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>
#include <algorithm>

namespace top1::modules {

  /*
   * Declarations
   */

  class ArpeggiatorSeqScreen : public ui::ModuleScreen<ArpeggiatorSeq> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_pattern(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<ArpeggiatorSeq>::ModuleScreen;
  };

  /*
   * ArpeggiatorSeq - Classic arpeggiator
   */

  ArpeggiatorSeq::ArpeggiatorSeq() :
    SequencerModule(&props),
    screen (new ArpeggiatorSeqScreen(this)) {
    heldNotes.fill(-1);
  }

  void ArpeggiatorSeq::init() {
    currentStep = 0;
    samplesUntilNextStep = 0;
    noteIsOn = false;
    numHeldNotes = 0;
    heldNotes.fill(-1);
  }

  void ArpeggiatorSeq::display() {
    Globals::ui.display(*screen);
  }

  void ArpeggiatorSeq::addNote(int note) {
    // Check if note already exists
    for (int i = 0; i < numHeldNotes; i++) {
      if (heldNotes[i] == note) return;
    }
    
    // Add note and sort
    if (numHeldNotes < 16) {
      heldNotes[numHeldNotes++] = note;
      std::sort(heldNotes.begin(), heldNotes.begin() + numHeldNotes);
    }
  }

  void ArpeggiatorSeq::removeNote(int note) {
    // Find and remove note
    for (int i = 0; i < numHeldNotes; i++) {
      if (heldNotes[i] == note) {
        for (int j = i; j < numHeldNotes - 1; j++) {
          heldNotes[j] = heldNotes[j + 1];
        }
        heldNotes[--numHeldNotes] = -1;
        break;
      }
    }
    
    // Reset step if no notes held
    if (numHeldNotes == 0) {
      currentStep = 0;
    }
  }

  int ArpeggiatorSeq::getArpNote(int step) {
    if (numHeldNotes == 0) return -1;
    
    int pattern = props.pattern;
    int range = props.range;
    int totalNotes = numHeldNotes * range;
    
    // Patterns:
    // 1: Up
    // 2: Down
    // 3: Up-Down
    // 4: Down-Up
    // 5: Random
    // 6: Up (held order)
    // 7: Down (held order)
    // 8: Chord (all at once)
    
    int index = 0;
    int octaveOffset = 0;
    
    switch (pattern) {
    case 1: // Up
      index = step % totalNotes;
      octaveOffset = index / numHeldNotes;
      index = index % numHeldNotes;
      break;
    case 2: // Down
      index = (totalNotes - 1 - (step % totalNotes));
      octaveOffset = index / numHeldNotes;
      index = index % numHeldNotes;
      break;
    case 3: // Up-Down
      {
        int cycleLen = totalNotes * 2 - 2;
        int pos = step % cycleLen;
        if (pos < totalNotes) {
          index = pos;
        } else {
          index = cycleLen - pos;
        }
        octaveOffset = index / numHeldNotes;
        index = index % numHeldNotes;
      }
      break;
    case 4: // Down-Up
      {
        int cycleLen = totalNotes * 2 - 2;
        int pos = step % cycleLen;
        if (pos < totalNotes) {
          index = totalNotes - 1 - pos;
        } else {
          index = pos - totalNotes + 1;
        }
        octaveOffset = index / numHeldNotes;
        index = index % numHeldNotes;
      }
      break;
    case 5: // Random
      index = rand() % numHeldNotes;
      octaveOffset = rand() % range;
      break;
    case 6: // Up (held order)
      index = step % numHeldNotes;
      octaveOffset = 0;
      break;
    case 7: // Down (held order)
      index = (numHeldNotes - 1) - (step % numHeldNotes);
      octaveOffset = 0;
      break;
    case 8: // Chord
      index = 0; // Return first note, caller should handle chord
      octaveOffset = 0;
      break;
    }
    
    return heldNotes[index] + octaveOffset * 12;
  }

  void ArpeggiatorSeq::advanceStep(audio::ProcessData& data) {
    // Turn off previous note if still on
    if (noteIsOn) {
      auto* noteOffEvt = new midi::NoteOffEvent(midi::MidiEvent{
        midi::MidiEvent::NOTE_OFF, 0, nullptr, 0
      });
      noteOffEvt->key = currentNote;
      noteOffEvt->velocity = 64;
      data.midi.push_back(noteOffEvt);
      noteIsOn = false;
    }
    
    // Get next note
    if (numHeldNotes > 0) {
      if (props.pattern == 8) {
        // Chord mode: play all notes
        for (int i = 0; i < numHeldNotes; i++) {
          auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_ON, 0, nullptr, 0
          });
          noteOnEvt->key = heldNotes[i];
          noteOnEvt->velocity = 100;
          data.midi.push_back(noteOnEvt);
        }
      } else {
        // Arpeggio mode: play single note
        currentNote = getArpNote(currentStep);
        if (currentNote >= 0 && currentNote < 128) {
          auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_ON, 0, nullptr, 0
          });
          noteOnEvt->key = currentNote;
          noteOnEvt->velocity = 100;
          data.midi.push_back(noteOnEvt);
          noteIsOn = true;
          
          // Calculate gate duration
          float bpm = Globals::metronome.props.bpm;
          float secondsPerBeat = 60.0f / bpm;
          float secondsPerStep = secondsPerBeat / props.rate;
          float gateLength = secondsPerStep * props.gate;
          samplesUntilNoteOff = static_cast<int>(gateLength * Globals::samplerate);
        }
      }
      
      currentStep++;
    }
  }

  void ArpeggiatorSeq::process(const audio::ProcessData& data) {
    // Handle incoming MIDI for note tracking
    for (auto& evt : data.midi) {
      evt.match([&] (midi::NoteOnEvent& e) {
          addNote(e.key);
        }, [] (auto) {});
    }
    
    for (auto& evt : data.midi) {
      evt.match([&] (midi::NoteOffEvent& e) {
          removeNote(e.key);
        }, [] (auto) {});
    }
    
    // Calculate step timing
    float bpm = Globals::metronome.props.bpm;
    float secondsPerBeat = 60.0f / bpm;
    float secondsPerStep = secondsPerBeat / props.rate;
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
   * ArpeggiatorSeqScreen
   */

  using namespace ui::drawing;

  bool ArpeggiatorSeqScreen::keypress(ui::Key key) {
    return false;
  }

  void ArpeggiatorSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.rate.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.pattern.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.range.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.gate.step(e.clicks);
      break;
    }
  }

  void ArpeggiatorSeqScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_pattern(ctx);
  }

  void ArpeggiatorSeqScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.fillText("ARPEGGIATOR", 160, 20);

    // Parameters
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.fillStyle(Colours::Blue);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);
    ctx.fillText(fmt::format("Rate: 1/{}", module->props.rate.get()), 10, 45);
    
    ctx.fillStyle(Colours::Green);
    const char* patternNames[] = {"Up", "Down", "Up-Down", "Down-Up", "Random", "Order-Up", "Order-Down", "Chord"};
    int patIdx = std::clamp(module->props.pattern.get() - 1, 0, 7);
    ctx.fillText(fmt::format("Pattern: {}", patternNames[patIdx]), 10, 65);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Range: {} oct", module->props.range.get()), 10, 85);
    
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Left, TextAlign::Bottom);
    ctx.fillText(fmt::format("Gate: {:.2f}", module->props.gate.get()), 10, 230);
    
    // Held notes
    ctx.font(Fonts::Bold);
    ctx.font(16.f);
    ctx.fillStyle(Colours::Green);
    ctx.textAlign(TextAlign::Right, TextAlign::Top);
    ctx.fillText(fmt::format("Notes: {}", module->numHeldNotes), 310, 10);

    ctx.restore();
  }

  void ArpeggiatorSeqScreen::draw_pattern(Canvas& ctx) {
    ctx.save();
    
    // Visualize arp pattern
    float vizX = 180;
    float vizY = 80;
    float vizW = 120;
    float vizH = 120;
    
    int pattern = module->props.pattern;
    int range = module->props.range;
    
    if (module->numHeldNotes > 0) {
      int totalSteps = std::min(16, module->numHeldNotes * range);
      float stepW = vizW / totalSteps;
      
      for (int i = 0; i < totalSteps; i++) {
        int note = -1;
        
        // Simplified pattern visualization
        switch (pattern) {
        case 1: // Up
          note = (i * 12) / range;
          break;
        case 2: // Down
          note = ((totalSteps - i - 1) * 12) / range;
          break;
        case 3: // Up-Down
          if (i < totalSteps / 2) {
            note = (i * 12) / range;
          } else {
            note = ((totalSteps - i - 1) * 12) / range;
          }
          break;
        case 8: // Chord - all same height
          note = 60;
          break;
        default:
          note = (i * 12) / range;
          break;
        }
        
        float noteHeight = (note % 60) / 60.0f * vizH;
        float x = vizX + i * stepW;
        
        ctx.fillStyle((i == module->currentStep % totalSteps) ? Colours::Green : Colours::Blue);
        ctx.beginPath();
        ctx.rect(x, vizY + vizH - noteHeight, stepW - 2, noteHeight);
        ctx.fill();
      }
    } else {
      // No notes held
      ctx.font(Fonts::Bold);
      ctx.font(18.f);
      ctx.fillStyle(Colours::Gray60);
      ctx.textAlign(TextAlign::Center, TextAlign::Middle);
      ctx.fillText("Hold notes to start", vizX + vizW/2, vizY + vizH/2);
    }
    
    ctx.restore();
  }

} // namespace top1::modules
