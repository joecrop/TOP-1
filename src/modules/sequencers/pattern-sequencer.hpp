#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"

namespace top1::modules {

  class PatternSeqScreen;  // Forward declaration

  /**
   * Pattern Sequencer - 16-step pattern sequencer synced to metronome
   * Inspired by OP-1's sequencers
   */
  class PatternSequencer : public SequencerModule {
  public:
    static constexpr int NUM_STEPS = 16;
    static constexpr int NUM_NOTES = 16;  // 16 notes/drums can be triggered per step

    struct Step {
      bool active[NUM_NOTES] = {false};  // Which notes START on this step
      int velocity[NUM_NOTES] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};  // Velocity per note
      int length[NUM_NOTES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  // Note length in steps (1 = single step)
    };

    struct Props : public Properties {
      Property<int> currentStep  = {this, "step", 0, {0, NUM_STEPS - 1, 1}};
      Property<int> patternLength = {this, "length", 16, {1, NUM_STEPS, 1}};  // Blue knob
      Property<int> noteOffset   = {this, "offset", 48, {24, 84, 1}};         // Green knob: base MIDI note
      Property<float> swing      = {this, "swing", 0.0f, {0.0f, 0.5f, 0.01f}}; // White knob
      Property<int> scale        = {this, "scale", 0, {0, 5, 1}};             // Red knob: scale type
      Property<bool> running     = {this, "running", false};
      Property<bool> bypassed    = {this, "bypassed", false};  // When bypassed, keyboard works normally
    } props;

    std::array<Step, NUM_STEPS> pattern;  // The pattern data
    
    // Scale definitions (semitone offsets from root) - 16 notes each
    static constexpr int scaleChromatic[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    static constexpr int scaleMajor[16] = {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26};
    static constexpr int scaleMinor[16] = {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26};
    static constexpr int scalePentatonic[16] = {0, 2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36};
    static constexpr int scaleBlues[16] = {0, 3, 5, 6, 7, 10, 12, 15, 17, 18, 19, 22, 24, 27, 29, 30};
    static constexpr int scaleDrums[16] = {36, 38, 42, 46, 41, 45, 49, 51, 37, 39, 44, 48, 50, 53, 55, 57};  // Standard drum notes

    PatternSequencer();
    ~PatternSequencer() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleStep(int step, int note);
    void clearPattern();
    int getNoteForSlot(int slot);  // Get MIDI note for a slot based on scale
    int getSlotForNote(int midiNote);  // Get slot for a MIDI note based on scale
    
    // Cursor position for editing when stopped
    int cursorStep = 0;
    
    // Metronome sync
    int lastBeat = -1;
    float accumulator = 0.0f;  // For sub-beat timing
    
    // Recording state - track held notes for variable length recording
    struct HeldNote {
      bool held = false;
      int startStep = 0;
      int slot = -1;
    };
    HeldNote heldNotes[NUM_NOTES];  // Track which notes are currently held
    
    // Playback state - track which notes are currently ringing
    int ringingNotes[NUM_NOTES] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};  // -1 = not ringing, else = remaining steps

  private:
    std::unique_ptr<PatternSeqScreen> screen;
  };

  /**
   * Pattern Sequencer Screen
   */
  class PatternSeqScreen : public ui::ModuleScreen<PatternSequencer> {
  public:
    using ui::ModuleScreen<PatternSequencer>::ModuleScreen;

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
