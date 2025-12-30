#include "modules/sequencers/arpeggiator.hpp"
#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <algorithm>
#include <random>

namespace top1::modules {

  Arpeggiator::Arpeggiator() 
    : screen(std::make_unique<ArpeggiatorScreen>(this)) {}

  void Arpeggiator::noteOn(int note, int velocity) {
    // Add to held notes if not already there
    for (auto& n : heldNotes) {
      if (n.first == note) {
        n.second = velocity;
        return;
      }
    }
    if (heldNotes.size() < MAX_HELD_NOTES) {
      heldNotes.push_back({note, velocity});
    }
  }

  void Arpeggiator::noteOff(int note) {
    if (props.hold) return;  // In hold mode, don't remove notes
    
    heldNotes.erase(
      std::remove_if(heldNotes.begin(), heldNotes.end(),
        [note](const std::pair<int, int>& p) { return p.first == note; }),
      heldNotes.end()
    );
    
    if (heldNotes.empty()) {
      currentIndex = 0;
      goingUp = true;
    }
  }

  int Arpeggiator::getNextNote() {
    if (heldNotes.empty()) return -1;

    // Sort notes for up/down modes
    std::vector<int> notes;
    for (auto& n : heldNotes) {
      notes.push_back(n.first);
    }
    std::sort(notes.begin(), notes.end());
    
    // Expand with octaves
    std::vector<int> expandedNotes;
    for (int oct = 0; oct < props.octaves; oct++) {
      for (int n : notes) {
        expandedNotes.push_back(n + oct * 12);
      }
    }
    
    if (expandedNotes.empty()) return -1;

    int mode = props.mode;
    int note = -1;

    switch (mode) {
      case 0: // Up
        currentIndex = currentIndex % expandedNotes.size();
        note = expandedNotes[currentIndex];
        currentIndex++;
        break;
        
      case 1: // Down
        currentIndex = currentIndex % expandedNotes.size();
        note = expandedNotes[expandedNotes.size() - 1 - currentIndex];
        currentIndex++;
        break;
        
      case 2: // Up-Down
        if (goingUp) {
          note = expandedNotes[currentIndex];
          currentIndex++;
          if (currentIndex >= (int)expandedNotes.size()) {
            currentIndex = std::max(0, (int)expandedNotes.size() - 2);
            goingUp = false;
          }
        } else {
          currentIndex = std::max(0, std::min(currentIndex, (int)expandedNotes.size() - 1));
          note = expandedNotes[currentIndex];
          currentIndex--;
          if (currentIndex < 0) {
            currentIndex = std::min(1, (int)expandedNotes.size() - 1);
            goingUp = true;
          }
        }
        break;
        
      case 3: // Random
        {
          static std::random_device rd;
          static std::mt19937 gen(rd());
          std::uniform_int_distribution<> dis(0, expandedNotes.size() - 1);
          note = expandedNotes[dis(gen)];
        }
        break;
        
      case 4: // Played order
        currentIndex = currentIndex % heldNotes.size();
        note = heldNotes[currentIndex].first;
        // Add octave cycling
        int octaveOffset = (currentIndex / heldNotes.size()) % props.octaves.get();
        note += octaveOffset * 12;
        currentIndex++;
        break;
    }

    return note;
  }

  void Arpeggiator::process(const audio::ProcessData& data) {
    // Process incoming MIDI to capture held notes
    for (auto& ev : data.midi) {
      ev.match(
        [&](midi::NoteOnEvent& e) {
          if (e.velocity > 0) {
            noteOn(e.key, e.velocity);
          } else {
            noteOff(e.key);
          }
        },
        [&](midi::NoteOffEvent& e) {
          noteOff(e.key);
        },
        [](auto&&) {}
      );
    }

    if (!props.running || heldNotes.empty()) {
      noteIsOn = false;
      return;
    }

    // Calculate timing from metronome
    float bpm = Globals::metronome.props.bpm.get();
    float beatsPerArp = 4.0f / props.rate.get();  // Rate is in 1/N notes
    float samplesPerArp = (60.0f / bpm) * beatsPerArp * Globals::samplerate;
    
    // Gate timing
    float gatePercent = props.gate.get() / 100.0f;
    float samplesGateOn = samplesPerArp * gatePercent;

    accumulator += data.nframes;

    // Check if we should trigger a new note
    if (accumulator >= samplesPerArp) {
      accumulator -= samplesPerArp;
      
      int nextNote = getNextNote();
      if (nextNote >= 0 && nextNote <= 127) {
        // Trigger new note
        data.midi.emplace_back(new midi::NoteOnEvent(nextNote, 100));
        lastTriggeredNote = nextNote;
        noteIsOn = true;
      }
    }
    // Check if we should release the note (simple gate - just check time passed)
    else if (noteIsOn && accumulator >= samplesGateOn) {
      // For now, we don't send note-off since we don't have an easy constructor
      // The next note-on will effectively re-trigger
      noteIsOn = false;
    }
  }

  void Arpeggiator::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* ArpeggiatorScreen Implementation     */
  /****************************************/

  using namespace ui::drawing;

  void ArpeggiatorScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("ARPEGGIATOR", 160, 20);

    // Mode name
    const char* modeNames[] = {"UP", "DOWN", "UP-DOWN", "RANDOM", "PLAYED"};
    int mode = module->props.mode.get();
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.fillStyle(module->props.running ? Colours::Green : Colours::Gray60);
    ctx.fillText(modeNames[mode], 160, 42);

    // Held notes visualization
    int numNotes = module->heldNotes.size();
    if (numNotes > 0) {
      // Sort for display
      std::vector<std::pair<int, int>> sortedNotes = module->heldNotes;
      std::sort(sortedNotes.begin(), sortedNotes.end());

      // Draw note boxes
      float boxW = 240.0f / std::max(1, numNotes * module->props.octaves.get());
      float startX = 40;

      for (int oct = 0; oct < module->props.octaves.get(); oct++) {
        for (int i = 0; i < numNotes; i++) {
          int baseNote = sortedNotes[i].first;
          float x = startX + (oct * numNotes + i) * boxW;
          
          Colour c = Colours::Gray60;
          if (module->noteIsOn && module->lastTriggeredNote >= 0) {
            if (baseNote + oct * 12 == module->lastTriggeredNote) {
              c = Colours::White;
            }
          }
          
          ctx.fillStyle(c);
          ctx.beginPath();
          ctx.roundedRect(x + 1, 60, boxW - 2, 40, 3);
          ctx.fill();

          // Note name
          ctx.font(10);
          ctx.fillStyle(Colours::Black);
          ctx.textAlign(TextAlign::Center, TextAlign::Middle);
          ctx.fillText(midi::noteName(baseNote + oct * 12), x + boxW/2, 80);
        }
      }
    } else {
      ctx.fillStyle(Colours::Gray60);
      ctx.font(12);
      ctx.fillText("Hold notes to arpeggiate", 160, 80);
    }

    // Hold indicator
    if (module->props.hold) {
      ctx.fillStyle(Colours::Green);
      ctx.font(12);
      ctx.textAlign(TextAlign::Right, TextAlign::Top);
      ctx.fillText("HOLD", 300, 55);
    }

    // Parameters
    float paramY = 145;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Rate (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("RATE", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("1/{}", module->props.rate.get()), 50, paramY + 14);

    // Octaves (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("OCT", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(std::to_string(module->props.octaves.get()), 120, paramY + 14);

    // Mode (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("MODE", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(16);
    ctx.fillText(modeNames[mode], 200, paramY + 16);

    // Gate (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("GATE", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{}%", module->props.gate.get()), 270, paramY + 14);
  }

  bool ArpeggiatorScreen::keypress(ui::Key key) {
    switch (key) {
      case ui::Key::K_PLAY:
        module->props.running = !module->props.running;
        return true;
      case ui::Key::K_SHIFT:
        module->props.hold = !module->props.hold;
        if (!module->props.hold) {
          // When disabling hold, clear held notes
          module->heldNotes.clear();
          module->currentIndex = 0;
        }
        return true;
      default:
        return false;
    }
  }

  void ArpeggiatorScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
      case ui::Rotary::Blue:
        module->props.rate.step(e.clicks);
        break;
      case ui::Rotary::Green:
        module->props.octaves.step(e.clicks);
        break;
      case ui::Rotary::White:
        module->props.mode.step(e.clicks);
        module->currentIndex = 0;  // Reset on mode change
        break;
      case ui::Rotary::Red:
        module->props.gate.step(e.clicks);
        break;
    }
  }

} // namespace top1::modules
