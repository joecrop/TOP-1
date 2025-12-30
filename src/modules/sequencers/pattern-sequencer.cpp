#include "pattern-sequencer.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  // Scale definitions
  constexpr int PatternSequencer::scaleChromatic[16];
  constexpr int PatternSequencer::scaleMajor[16];
  constexpr int PatternSequencer::scaleMinor[16];
  constexpr int PatternSequencer::scalePentatonic[16];
  constexpr int PatternSequencer::scaleBlues[16];
  constexpr int PatternSequencer::scaleDrums[16];

  PatternSequencer::PatternSequencer() :
    SequencerModule(&props),
    screen(std::make_unique<PatternSeqScreen>(this)) {
    clearPattern();
  }

  void PatternSequencer::clearPattern() {
    for (auto& step : pattern) {
      for (int i = 0; i < NUM_NOTES; i++) {
        step.active[i] = false;
        step.velocity[i] = 100;
        step.length[i] = 1;
      }
    }
    // Also reset held and ringing note state
    for (int i = 0; i < NUM_NOTES; i++) {
      heldNotes[i].held = false;
      ringingNotes[i] = -1;
    }
  }

  void PatternSequencer::toggleStep(int step, int note) {
    if (step >= 0 && step < NUM_STEPS && note >= 0 && note < NUM_NOTES) {
      pattern[step].active[note] = !pattern[step].active[note];
    }
  }

  int PatternSequencer::getNoteForSlot(int slot) {
    if (slot < 0 || slot >= NUM_NOTES) return 60;  // Middle C fallback
    
    int offset = props.noteOffset.get();
    int scaleType = props.scale.get();
    
    const int* scale;
    switch (scaleType) {
      case 0: scale = scaleChromatic; break;
      case 1: scale = scaleMajor; break;
      case 2: scale = scaleMinor; break;
      case 3: scale = scalePentatonic; break;
      case 4: scale = scaleBlues; break;
      case 5: scale = scaleDrums; return scaleDrums[slot];  // Drums use absolute notes
      default: scale = scaleChromatic; break;
    }
    
    return offset + scale[slot];
  }

  int PatternSequencer::getSlotForNote(int midiNote) {
    // Find which slot (0-7) corresponds to a MIDI note based on current scale
    int offset = props.noteOffset.get();
    int scaleType = props.scale.get();
    
    const int* scale;
    switch (scaleType) {
      case 0: scale = scaleChromatic; break;
      case 1: scale = scaleMajor; break;
      case 2: scale = scaleMinor; break;
      case 3: scale = scalePentatonic; break;
      case 4: scale = scaleBlues; break;
      case 5: scale = scaleDrums; 
        // For drums, find exact match
        for (int i = 0; i < NUM_NOTES; i++) {
          if (scaleDrums[i] == midiNote) return i;
        }
        return -1;
      default: scale = scaleChromatic; break;
    }
    
    // Find matching slot
    for (int i = 0; i < NUM_NOTES; i++) {
      if (offset + scale[i] == midiNote) return i;
    }
    return -1;  // Not in scale
  }

  void PatternSequencer::process(const audio::ProcessData& data) {
    // Get the current edit step (either cursor when stopped, or playhead when playing)
    int editStep = props.running.get() ? props.currentStep.get() : cursorStep;
    int patternLen = props.patternLength.get();
    
    // Process incoming MIDI to record notes into the pattern
    for (auto& ev : data.midi) {
      ev.match(
        [&](midi::NoteOnEvent& e) {
          if (e.velocity > 0) {
            int slot = getSlotForNote(e.key);
            if (slot >= 0 && slot < NUM_NOTES) {
              // Start recording this note
              pattern[editStep].active[slot] = true;
              pattern[editStep].velocity[slot] = e.velocity;
              pattern[editStep].length[slot] = 1;  // Start with length 1
              
              // Track that this note is being held (for extending length)
              heldNotes[slot].held = true;
              heldNotes[slot].startStep = editStep;
              heldNotes[slot].slot = slot;
            }
          }
        },
        [&](midi::NoteOffEvent& e) {
          int slot = getSlotForNote(e.key);
          if (slot >= 0 && slot < NUM_NOTES && heldNotes[slot].held) {
            // Note released - calculate final length
            int startStep = heldNotes[slot].startStep;
            int length = 1;
            if (props.running.get()) {
              // Calculate how many steps the note was held
              int currentStep = props.currentStep.get();
              if (currentStep >= startStep) {
                length = currentStep - startStep + 1;
              } else {
                // Wrapped around pattern
                length = (patternLen - startStep) + currentStep + 1;
              }
              // Clamp length to pattern length
              if (length > patternLen) length = patternLen;
            }
            pattern[startStep].length[slot] = length;
            heldNotes[slot].held = false;
          }
        },
        [](auto&&) {}
      );
    }
    
    // If running, extend held notes as we advance
    if (props.running.get()) {
      for (int n = 0; n < NUM_NOTES; n++) {
        if (heldNotes[n].held) {
          int startStep = heldNotes[n].startStep;
          int currentStep = props.currentStep.get();
          int length;
          if (currentStep >= startStep) {
            length = currentStep - startStep + 1;
          } else {
            length = (patternLen - startStep) + currentStep + 1;
          }
          if (length <= patternLen) {
            pattern[startStep].length[n] = length;
          }
        }
      }
    }
    
    if (!props.running.get()) {
      // When stopped, send note-offs for any ringing notes
      for (int n = 0; n < NUM_NOTES; n++) {
        if (ringingNotes[n] >= 0) {
          int midiNote = getNoteForSlot(n);
          data.midi.emplace_back(new midi::NoteOffEvent(midiNote));
          ringingNotes[n] = -1;
        }
      }
      return;  // Not running - don't advance or trigger notes
    }
    
    // Calculate step timing from metronome
    float bpm = Globals::metronome.props.bpm.get();
    float samplesPerBeat = (60.0f / bpm) * Globals::samplerate;
    float samplesPerStep = samplesPerBeat / 4.0f;  // 16th notes
    
    // Apply swing to even steps
    int currentStep = props.currentStep.get();
    float swingAmount = props.swing.get();
    if (currentStep % 2 == 1) {
      samplesPerStep *= (1.0f + swingAmount);
    } else {
      samplesPerStep *= (1.0f - swingAmount * 0.5f);
    }
    
    accumulator += data.nframes;
    
    if (accumulator >= samplesPerStep) {
      accumulator -= samplesPerStep;
      
      // Advance to next step
      int nextStep = (currentStep + 1) % patternLen;
      props.currentStep = nextStep;
      
      // First, decrement ringing notes and send note-offs for expired ones
      for (int n = 0; n < NUM_NOTES; n++) {
        if (ringingNotes[n] > 0) {
          ringingNotes[n]--;
          if (ringingNotes[n] == 0) {
            // Note has finished - send note-off
            int midiNote = getNoteForSlot(n);
            data.midi.emplace_back(new midi::NoteOffEvent(midiNote));
            ringingNotes[n] = -1;
          }
        }
      }
      
      // Trigger notes that START on this step
      Step& step = pattern[nextStep];
      for (int n = 0; n < NUM_NOTES; n++) {
        if (step.active[n]) {
          int midiNote = getNoteForSlot(n);
          int velocity = step.velocity[n];
          int length = step.length[n];
          
          // If this note is already ringing, send note-off first
          if (ringingNotes[n] >= 0) {
            data.midi.emplace_back(new midi::NoteOffEvent(midiNote));
          }
          
          // Create MIDI note-on event
          data.midi.emplace_back(new midi::NoteOnEvent(midiNote, velocity));
          
          // Track how long this note should ring
          ringingNotes[n] = length;
        }
      }
    }
  }

  void PatternSequencer::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* PatternSeqScreen Implementation      */
  /****************************************/

  using namespace ui::drawing;

  void PatternSeqScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    int currentStep = module->props.currentStep.get();
    int patternLength = module->props.patternLength.get();
    bool running = module->props.running.get();
    
    // The edit column is either the cursor (when stopped) or playhead (when playing)
    int editColumn = running ? currentStep : module->cursorStep;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("PATTERN SEQ", 160, 16);

    // Running indicator
    if (running) {
      ctx.font(Fonts::Norm);
      ctx.font(12);
      ctx.fillStyle(Colours::Green);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText("▶ REC", 305, 16);
    } else {
      ctx.font(Fonts::Norm);
      ctx.font(12);
      ctx.fillStyle(Colours::Blue);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText("■ EDIT", 305, 16);
    }

    // Draw step grid - compressed height
    float gridX = 20;
    float gridY = 32;
    float cellW = 18;
    float cellH = 9;  // Reduced from 14 to 9 (5 pixels less)
    float spacing = 1;

    // Draw note labels on left (reversed - high notes on top)
    ctx.font(Fonts::Norm);
    ctx.font(8);
    ctx.textAlign(TextAlign::Right, TextAlign::Middle);
    for (int n = 0; n < PatternSequencer::NUM_NOTES; n++) {
      int displayRow = PatternSequencer::NUM_NOTES - 1 - n;  // Reverse: slot 15 at top, slot 0 at bottom
      int midiNote = module->getNoteForSlot(n);
      std::string noteName = midi::noteName(midiNote);
      ctx.fillStyle(Colours::Gray60);
      ctx.fillText(noteName, gridX - 3, gridY + displayRow * (cellH + spacing) + cellH / 2);
    }

    // Draw cursor column highlight (full column)
    float cursorX = gridX + editColumn * (cellW + spacing);
    ctx.beginPath();
    ctx.rect({cursorX - 1, gridY - 2}, {cellW + 2, PatternSequencer::NUM_NOTES * (cellH + spacing) + 2});
    if (running) {
      ctx.strokeStyle(Colours::Green);
    } else {
      ctx.strokeStyle(Colours::Blue);
    }
    ctx.lineWidth(2);
    ctx.stroke();

    // Draw grid cells (reversed - high notes on top)
    // First pass: draw background cells and continuation markers
    for (int step = 0; step < PatternSequencer::NUM_STEPS; step++) {
      for (int note = 0; note < PatternSequencer::NUM_NOTES; note++) {
        int displayRow = PatternSequencer::NUM_NOTES - 1 - note;  // Reverse: slot 15 at top, slot 0 at bottom
        float x = gridX + step * (cellW + spacing);
        float y = gridY + displayRow * (cellH + spacing);
        
        // Dim steps beyond pattern length
        bool inPattern = step < patternLength;
        bool isEditColumn = (step == editColumn);
        
        // Check if this cell is a "continuation" of a previous note
        bool isContinuation = false;
        for (int prevStep = 0; prevStep < step; prevStep++) {
          if (module->pattern[prevStep].active[note]) {
            int noteLength = module->pattern[prevStep].length[note];
            if (prevStep + noteLength > step) {
              isContinuation = true;
              break;
            }
          }
        }

        ctx.beginPath();
        ctx.rect({x, y}, {cellW, cellH});
        
        // Inactive/continuation step background
        Colour bgCol;
        if (isContinuation) {
          // Continuation cells get a dimmer version of green
          bgCol = Colours::Green.dim(0.5f);
        } else {
          bgCol = inPattern ? Colours::Gray70 : Colours::Gray70.dim(0.3f);
        }
        if (!inPattern) bgCol = bgCol.dim(0.3f);
        if (isEditColumn) bgCol = bgCol.brighten(0.15f);
        ctx.fillStyle(bgCol);
        ctx.fill();
      }
    }
    
    // Second pass: draw note START markers (bright, with length indicator)
    for (int step = 0; step < PatternSequencer::NUM_STEPS; step++) {
      for (int note = 0; note < PatternSequencer::NUM_NOTES; note++) {
        if (!module->pattern[step].active[note]) continue;
        
        int displayRow = PatternSequencer::NUM_NOTES - 1 - note;
        float x = gridX + step * (cellW + spacing);
        float y = gridY + displayRow * (cellH + spacing);
        int noteLength = module->pattern[step].length[note];
        
        bool inPattern = step < patternLength;
        bool isEditColumn = (step == editColumn);
        
        // Calculate total width of note (spans multiple cells)
        float totalWidth = noteLength * (cellW + spacing) - spacing;
        // Clamp to not exceed grid
        int maxLength = PatternSequencer::NUM_STEPS - step;
        if (noteLength > maxLength) {
          totalWidth = maxLength * (cellW + spacing) - spacing;
        }
        
        // Active step - all notes are green
        Colour stepCol = Colours::Green;
        
        if (!inPattern) stepCol = stepCol.dim(0.3f);
        if (isEditColumn) stepCol = stepCol.brighten(0.3f);
        
        // Draw the full note bar
        ctx.beginPath();
        ctx.rect({x, y}, {totalWidth, cellH});
        ctx.fillStyle(stepCol);
        ctx.fill();
        
        // Draw a small marker at the start to indicate note-on position
        ctx.beginPath();
        ctx.rect({x, y}, {3, cellH});
        ctx.fillStyle(stepCol.brighten(0.3f));
        ctx.fill();
      }
    }

    // Draw playhead position indicator (triangle above current column)
    float playheadX = gridX + editColumn * (cellW + spacing) + cellW / 2;
    ctx.beginPath();
    ctx.moveTo(playheadX, gridY - 6);
    ctx.lineTo(playheadX - 4, gridY - 2);
    ctx.lineTo(playheadX + 4, gridY - 2);
    ctx.closePath();
    if (running) {
      ctx.fillStyle(Colours::Green);
    } else {
      ctx.fillStyle(Colours::Blue);
    }
    ctx.fill();

    // Parameter labels at bottom of screen
    float paramY = 195;
    ctx.font(Fonts::Norm);
    ctx.font(10);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    
    // Length (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("LEN", 40, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(16);
    ctx.fillText(fmt::format("{}", patternLength), 40, paramY + 11);
    
    // Note offset (Green)
    ctx.font(Fonts::Norm);
    ctx.font(10);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("ROOT", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(16);
    ctx.fillText(midi::noteName(module->props.noteOffset.get()), 120, paramY + 11);
    
    // Swing (White)
    ctx.font(Fonts::Norm);
    ctx.font(10);
    ctx.fillStyle(Colours::White);
    ctx.fillText("SWING", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(16);
    ctx.fillText(fmt::format("{:.0f}%", module->props.swing.get() * 100), 200, paramY + 11);
    
    // Scale (Red)
    ctx.font(Fonts::Norm);
    ctx.font(10);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("SCALE", 280, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(12);
    const char* scaleNames[] = {"CHROM", "MAJOR", "MINOR", "PENTA", "BLUES", "DRUMS"};
    ctx.fillText(scaleNames[module->props.scale.get()], 280, paramY + 11);
  }

  bool PatternSeqScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_PLAY:
      module->props.running = !module->props.running.get();
      if (module->props.running.get()) {
        // Starting playback - sync cursor to playhead
        module->props.currentStep = module->cursorStep;
        module->accumulator = 0;
      } else {
        // Stopping - sync cursor to where we stopped
        module->cursorStep = module->props.currentStep.get();
      }
      return true;
    default:
      return false;
    }
  }

  void PatternSeqScreen::rotary(ui::RotaryEvent e) {
    bool shift = Globals::ui.keys[ui::K_SHIFT];
    
    switch (e.rotary) {
    case ui::Rotary::Blue:
      if (shift) {
        // Shift + Blue: change pattern length
        module->props.patternLength.step(e.clicks);
      } else {
        // Blue: move cursor (when not playing)
        if (!module->props.running.get()) {
          int newCursor = module->cursorStep + e.clicks;
          int len = module->props.patternLength.get();
          // Wrap around
          while (newCursor < 0) newCursor += len;
          while (newCursor >= len) newCursor -= len;
          module->cursorStep = newCursor;
        }
      }
      break;
    case ui::Rotary::Green:
      module->props.noteOffset.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.swing.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.scale.step(e.clicks);
      break;
    }
  }

} // namespace top1::modules
