#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <vector>

namespace top1::modules {

  class FingerSeq : public SequencerModule {

    ui::ModuleScreen<FingerSeq>::ptr screen;

  public:

    struct Props : Properties {
      Property<bool> recording  = {this, "RECORDING",  false};
      Property<bool> overdub    = {this, "OVERDUB",    false};
      Property<int>  loopLength = {this, "LOOP_LENGTH", 16,   {1, 64, 1}};
      Property<float> quantize  = {this, "QUANTIZE",   0.0,  {0.0, 1.0, 0.25}};
    } props;

    FingerSeq();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    struct RecordedNote {
      float timeOffset;
      uint8_t note;
      uint8_t velocity;
      float duration;
    };
    
    std::vector<RecordedNote> recordedNotes;
    std::vector<int> activeNotes;
    
    float loopPosition = 0;
    int loopSampleCount = 0;
    bool wasRecording = false;
    
    void startRecording();
    void stopRecording();
    void clearLoop();
    float quantizeTime(float time);
    
    friend class FingerSeqScreen;
  };
}
