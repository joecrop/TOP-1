#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>
#include <random>

namespace top1::modules {

  class TombolaSeq : public SequencerModule {

    ui::ModuleScreen<TombolaSeq>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> probability = {this, "PROBABILITY", 0.7,  {0.0, 1.0, 0.01}};
      Property<float> randomness  = {this, "RANDOMNESS",  0.5,  {0.0, 1.0, 0.01}};
      Property<int>   steps       = {this, "STEPS",       16,   {4, 32, 1}};
      Property<int>   division    = {this, "DIVISION",    4,    {1, 16, 1}};
    } props;

    TombolaSeq();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
    
    std::array<int, 32> notes;
    std::array<float, 32> probabilities;
    
    int position = 0;
    int currentNote = -1;
    int samplesAccumulator = 0;
    
    friend class TombolaSeqScreen;
  };
}
