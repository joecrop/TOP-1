#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  class DelayEffect : public EffectModule {

    ui::ModuleScreen<DelayEffect>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> time      = {this, "TIME",     0.5,   {0.01, 2.0, 0.01}};
      Property<float> feedback  = {this, "FEEDBACK", 0.5,   {0, 0.95, 0.01}};
      Property<float> wetDry    = {this, "WET/DRY",  0.5,   {0, 1, 0.01}};
      Property<float> filter    = {this, "FILTER",   1.0,   {0, 1, 0.01}};
      Property<bool>  bypassed  = {this, "BYPASS",   false};
      Property<bool>  tempoSync = {this, "TEMPO_SYNC", true};
    } props;

    DelayEffect();

    void process(const audio::ProcessData&) override;

    void display() override;
    
    void init() override;
  };
}
