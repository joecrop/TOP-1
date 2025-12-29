#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  class ChorusEffect : public EffectModule {

    ui::ModuleScreen<ChorusEffect>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> rate      = {this, "RATE",     0.5,   {0.1, 10.0, 0.1}};
      Property<float> depth     = {this, "DEPTH",    0.5,   {0, 1, 0.01}};
      Property<float> wetDry    = {this, "WET/DRY",  0.5,   {0, 1, 0.01}};
      Property<float> feedback  = {this, "FEEDBACK", 0.3,   {-0.9, 0.9, 0.01}};
      Property<bool>  bypassed  = {this, "BYPASS",   false};
    } props;

    ChorusEffect();

    void process(const audio::ProcessData&) override;

    void display() override;
    
    void init() override;
  };
}
