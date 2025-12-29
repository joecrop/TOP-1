#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  class CompressorEffect : public EffectModule {

    ui::ModuleScreen<CompressorEffect>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> threshold = {this, "THRESHOLD", -20.0, {-60.0, 0.0, 1.0}};
      Property<float> ratio     = {this, "RATIO",     4.0,   {1.0, 20.0, 0.5}};
      Property<float> attack    = {this, "ATTACK",    0.005, {0.001, 0.1, 0.001}};
      Property<float> release   = {this, "RELEASE",   0.05,  {0.01, 1.0, 0.01}};
      Property<float> makeupGain = {this, "MAKEUP",   0.0,   {-20.0, 20.0, 0.5}};
      Property<bool> bypassed   = {this, "BYPASSED", false};
    } props;

    CompressorEffect();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    float envelopeFollower = 0.0f;
    float gainReduction = 0.0f;
    
    float computeGain(float inputLevel);
    
    friend class CompressorEffectScreen;
  };
}
