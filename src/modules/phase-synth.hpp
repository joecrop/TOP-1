#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class PhaseSynth : public SynthModule {

    ui::ModuleScreen<PhaseSynth>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> index     = {this, "INDEX",      1.0,  {0.0, 8.0, 0.1}};
      Property<float> feedback  = {this, "FEEDBACK",   0.0,  {0.0, 0.99, 0.01}};
      Property<float> resonance = {this, "RESONANCE",  1.0,  {0.5, 4.0, 0.1}};
      Property<float> brightness = {this, "BRIGHTNESS", 0.5, {0.0, 1.0, 0.01}};
      Property<float> attack    = {this, "ATTACK",     0.01, {0.001, 2.0, 0.01}};
      Property<float> decay     = {this, "DECAY",      0.3,  {0.001, 2.0, 0.01}};
      Property<float> sustain   = {this, "SUSTAIN",    0.7,  {0.0, 1.0, 0.01}};
      Property<float> release   = {this, "RELEASE",    0.3,  {0.001, 3.0, 0.01}};
    } props;

    PhaseSynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    struct Voice {
      bool active = false;
      int note = 0;
      float velocity = 0;
      float frequency = 0;
      
      float phase = 0;
      float feedbackSample = 0;
      
      enum EnvStage { Attack, Decay, Sustain, Release, Off };
      EnvStage envStage = Off;
      float envValue = 0;
      int envSamples = 0;
      int envDuration = 0;
      
      float lpFilterState = 0;
    };
    
    std::array<Voice, 6> voices;
    
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    void updateEnvelope(Voice& voice);
    
    friend class PhaseSynthScreen;
  };
}
