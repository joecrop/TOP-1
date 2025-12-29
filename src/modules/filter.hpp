#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  class FilterEffect : public EffectModule {

    ui::ModuleScreen<FilterEffect>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> cutoff    = {this, "CUTOFF",    1000.0, {20, 20000, 10}};
      Property<float> resonance = {this, "RESONANCE", 0.5,    {0, 1, 0.01}};
      Property<float> wetDry    = {this, "WET/DRY",   1.0,    {0, 1, 0.01}};
      Property<int, wrap> type  = {this, "TYPE",      0,      {0, 2, 1}}; // 0=LP, 1=HP, 2=BP
      Property<bool>  bypassed  = {this, "BYPASS",    false};
    } props;

    FilterEffect();

    void process(const audio::ProcessData&) override;

    void display() override;
    
    void init() override;
  };
}
