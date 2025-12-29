#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  class ReverbEffect : public EffectModule {

    ui::ModuleScreen<ReverbEffect>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> size      = {this, "SIZE",     0.5,   {0.1, 1.0, 0.01}};
      Property<float> damping   = {this, "DAMPING",  0.5,   {0, 1, 0.01}};
      Property<float> wetDry    = {this, "WET/DRY",  0.3,   {0, 1, 0.01}};
      Property<float> width     = {this, "WIDTH",    0.5,   {0, 1, 0.01}};
      Property<bool>  bypassed  = {this, "BYPASS",   false};
    } props;

    ReverbEffect();

    void process(const audio::ProcessData&) override;

    void display() override;
    
    void init() override;
  };
}
