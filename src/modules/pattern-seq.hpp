#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include "core/audio/midi.hpp"

namespace top1::modules {

  class PatternSeq : public SequencerModule {

    ui::ModuleScreen<PatternSeq>::ptr screen;

  public:

    struct Props : Properties {
      Property<int> length  = {this, "LENGTH",  16,  {1, 32, 1}};
      Property<float> gate  = {this, "GATE",    0.5, {0.1, 1.0, 0.05}};
      
      // Per-step properties stored as arrays
      std::array<int, 32> notes;     // MIDI notes for each step
      std::array<int, 32> velocities; // Velocity for each step (0 = rest)
      
      Props() {
        // Initialize with middle C and medium velocity
        notes.fill(60);
        velocities.fill(80);
      }
    } props;

    PatternSeq();

    void process(const audio::ProcessData&) override;

    void display() override;
    
    void init() override;

  private:
    int currentStep = 0;
    int samplesUntilNextStep = 0;
    bool noteIsOn = false;
    int samplesUntilNoteOff = 0;
    int editStep = 0; // Which step is being edited
    
    void advanceStep(audio::ProcessData& data);
    
    friend class PatternSeqScreen;
  };
}
