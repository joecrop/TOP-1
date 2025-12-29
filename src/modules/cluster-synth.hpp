#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>
#include <random>

namespace top1::modules {

  class ClusterSynth : public SynthModule {

    ui::ModuleScreen<ClusterSynth>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> grainSize   = {this, "GRAIN_SIZE",   0.05, {0.01, 0.5, 0.01}};
      Property<float> density     = {this, "DENSITY",      0.5,  {0.1, 1.0, 0.01}};
      Property<float> pitch       = {this, "PITCH",        0.0,  {-12.0, 12.0, 0.1}};
      Property<float> spread      = {this, "SPREAD",       0.3,  {0.0, 1.0, 0.01}};
      Property<float> attack      = {this, "ATTACK",       0.01, {0.001, 1.0, 0.01}};
      Property<float> release     = {this, "RELEASE",      0.3,  {0.01, 2.0, 0.01}};
    } props;

    ClusterSynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    static constexpr int MAX_GRAINS = 32;
    
    struct Grain {
      bool active = false;
      float phase = 0;
      float frequency = 0;
      float amplitude = 0;
      float grainPhase = 0;
      float grainDuration = 0;
    };
    
    struct Voice {
      bool active = false;
      int note = 0;
      float velocity = 0;
      float baseFrequency = 0;
      
      std::array<Grain, MAX_GRAINS> grains;
      
      float envValue = 0;
      bool releasing = false;
      int envSamples = 0;
    };
    
    std::array<Voice, 4> voices;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
    
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    void spawnGrain(Voice& voice);
    
    friend class ClusterSynthScreen;
  };
}
