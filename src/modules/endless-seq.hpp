#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>
#include <random>

namespace top1::modules {

  class EndlessSeq : public SequencerModule {

    ui::ModuleScreen<EndlessSeq>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> density    = {this, "DENSITY",    0.5,  {0.0, 1.0, 0.01}};
      Property<float> evolution  = {this, "EVOLUTION",  0.3,  {0.0, 1.0, 0.01}};
      Property<int>   scale      = {this, "SCALE",      1,    {1, 7, 1}};
      Property<int>   octaves    = {this, "OCTAVES",    3,    {1, 4, 1}};
      Property<float> rate       = {this, "RATE",       0.5,  {0.1, 4.0, 0.1}};
    } props;

    EndlessSeq();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    std::random_device rd;
    std::mt19937 gen;
    
    struct Pattern {
      std::array<int, 32> notes;
      std::array<float, 32> velocities;
      std::array<bool, 32> active;
    };
    
    Pattern currentPattern;
    Pattern nextPattern;
    
    int position = 0;
    int beatsPerBar = 16;
    int currentNote = -1;
    int samplesAccumulator = 0;
    
    std::array<int, 7> scaleIntervals = {0, 2, 4, 5, 7, 9, 11};
    
    void evolvePattern();
    int generateNoteForStep(int step);
    
    friend class EndlessSeqScreen;
  };
}
