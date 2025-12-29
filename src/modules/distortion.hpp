#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  class DistortionEffect : public EffectModule {

    ui::ModuleScreen<DistortionEffect>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> drive     = {this, "DRIVE",    1.0,   {1.0, 10.0, 0.1}};
      Property<int>   type      = {this, "TYPE",     1,     {1, 4, 1}};
      Property<float> tone      = {this, "TONE",     0.5,   {0.0, 1.0, 0.01}};
      Property<float> mix       = {this, "MIX",      0.5,   {0.0, 1.0, 0.01}};
      Property<bool> bypassed   = {this, "BYPASSED", false};
    } props;

    DistortionEffect();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    float toneFilterState = 0.0f;
    
    float applyDistortion(float input, int type);
    
    friend class DistortionEffectScreen;
  };
}
