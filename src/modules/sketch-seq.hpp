#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <vector>

namespace top1::modules {

  class SketchSeq : public SequencerModule {

    ui::ModuleScreen<SketchSeq>::ptr screen;

  public:

    struct Props : Properties {
      Property<int>   loopLength = {this, "LOOP_LENGTH", 16,   {4, 64, 1}};
      Property<float> quantize   = {this, "QUANTIZE",    0.25, {0.0, 1.0, 0.25}};
      Property<int>   noteRange  = {this, "NOTE_RANGE",  24,   {12, 48, 1}};
      Property<int>   baseNote   = {this, "BASE_NOTE",   60,   {36, 84, 1}};
    } props;

    SketchSeq();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    struct DrawnNote {
      float position; // 0-1 relative to loop
      int note;
      float duration;
      float velocity;
    };
    
    std::vector<DrawnNote> drawnNotes;
    std::vector<int> activeNotes;
    
    float loopPosition = 0;
    float lastLoopPosition = 0;
    int loopSampleCount = 0;
    
    void clearSketch();
    
    friend class SketchSeqScreen;
  };
}
