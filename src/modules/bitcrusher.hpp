#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  class BitcrusherEffect : public EffectModule {

    ui::ModuleScreen<BitcrusherEffect>::ptr screen;

  public:

    struct Props : Properties {
      Property<int>   bitDepth   = {this, "BIT_DEPTH",   16,    {1, 16, 1}};
      Property<float> sampleRate = {this, "SAMPLE_RATE", 44100, {100, 44100, 100}};
      Property<float> mix        = {this, "MIX",         1.0,   {0.0, 1.0, 0.01}};
      Property<bool> bypassed    = {this, "BYPASSED",    false};
    } props;

    BitcrusherEffect();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    float heldSample = 0.0f;
    float phaseAccumulator = 0.0f;
    
    friend class BitcrusherEffectScreen;
  };
}
