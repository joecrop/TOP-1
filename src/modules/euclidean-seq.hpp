#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include "core/audio/midi.hpp"

namespace top1::modules {

  class EuclideanSeq : public SequencerModule {

    ui::ModuleScreen<EuclideanSeq>::ptr screen;

  public:

    struct Props : Properties {
      Property<int> length  = {this, "LENGTH",  16,  {1, 32, 1}};
      Property<int> pulses  = {this, "PULSES",  4,   {0, 32, 1}};
      Property<int> offset  = {this, "OFFSET",  0,   {0, 31, 1}};
      Property<int> note    = {this, "NOTE",    60,  {0, 127, 1}}; // MIDI note
      Property<float> gate  = {this, "GATE",    0.5, {0.1, 1.0, 0.05}}; // Gate length as fraction of step
    } props;

    EuclideanSeq();

    void process(const audio::ProcessData&) override;

    void display() override;
    
    void init() override;
    
    std::vector<bool> generatePattern();

  private:
    int currentStep = 0;
    int samplesUntilNextStep = 0;
    bool noteIsOn = false;
    int samplesUntilNoteOff = 0;
    
    void advanceStep(audio::ProcessData& data);
    
    friend class EuclideanSeqScreen;
  };
}
