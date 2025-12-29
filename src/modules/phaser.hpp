#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class PhaserEffect : public EffectModule {

    ui::ModuleScreen<PhaserEffect>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> rate      = {this, "RATE",      0.5,   {0.01, 10.0, 0.1}};
      Property<float> depth     = {this, "DEPTH",     0.5,   {0.0, 1.0, 0.01}};
      Property<float> feedback  = {this, "FEEDBACK",  0.5,   {0.0, 0.95, 0.01}};
      Property<int>   stages    = {this, "STAGES",    4,     {2, 12, 1}};
      Property<bool> bypassed   = {this, "BYPASSED", false};
    } props;

    PhaserEffect();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    static constexpr int MAX_STAGES = 12;
    
    struct AllpassFilter {
      float z1 = 0.0f;
      
      float process(float input, float coefficient) {
        float output = coefficient * input + z1;
        z1 = input - coefficient * output;
        return output;
      }
    };
    
    std::array<AllpassFilter, MAX_STAGES> allpass;
    float lfoPhase = 0.0f;
    float currentCoeff = 0.5f;
    
    friend class PhaserEffectScreen;
  };
}
