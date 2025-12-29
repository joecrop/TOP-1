#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class VoltageSynth : public SynthModule {

    ui::ModuleScreen<VoltageSynth>::ptr screen;

  public:

    struct Props : Properties {
      Property<int>   waveform   = {this, "WAVEFORM",   1,    {1, 3, 1}};
      Property<float> cutoff     = {this, "CUTOFF",     0.5,  {0.0, 1.0, 0.01}};
      Property<float> resonance  = {this, "RESONANCE",  0.3,  {0.0, 1.0, 0.01}};
      Property<float> envAmount  = {this, "ENV_AMOUNT", 0.5,  {0.0, 1.0, 0.01}};
      Property<float> attack     = {this, "ATTACK",     0.01, {0.001, 2.0, 0.01}};
      Property<float> decay      = {this, "DECAY",      0.3,  {0.001, 2.0, 0.01}};
      Property<float> sustain    = {this, "SUSTAIN",    0.7,  {0.0, 1.0, 0.01}};
      Property<float> release    = {this, "RELEASE",    0.3,  {0.001, 3.0, 0.01}};
    } props;

    VoltageSynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    struct SVFilter {
      float ic1eq = 0;
      float ic2eq = 0;
      
      void process(float input, float cutoff, float resonance, float& lp, float& bp, float& hp) {
        float g = std::tan(M_PI * cutoff);
        float k = 2.0f - 2.0f * resonance;
        
        float a1 = 1.0f / (1.0f + g * (g + k));
        float a2 = g * a1;
        float a3 = g * a2;
        
        float v3 = input - ic2eq;
        float v1 = a1 * ic1eq + a2 * v3;
        float v2 = ic2eq + a2 * ic1eq + a3 * v3;
        
        ic1eq = 2.0f * v1 - ic1eq;
        ic2eq = 2.0f * v2 - ic2eq;
        
        lp = v2;
        bp = v1;
        hp = input - k * v1 - v2;
      }
    };
    
    struct Voice {
      bool active = false;
      int note = 0;
      float velocity = 0;
      float frequency = 0;
      float phase = 0;
      
      SVFilter filter;
      
      enum EnvStage { Attack, Decay, Sustain, Release, Off };
      EnvStage ampEnvStage = Off;
      EnvStage filterEnvStage = Off;
      float ampEnvValue = 0;
      float filterEnvValue = 0;
      int ampEnvSamples = 0;
      int filterEnvSamples = 0;
      int ampEnvDuration = 0;
      int filterEnvDuration = 0;
    };
    
    std::array<Voice, 8> voices;
    
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    void updateEnvelope(Voice& voice, Voice::EnvStage& stage, float& value, int& samples, int& duration, bool isFilter);
    
    friend class VoltageSynthScreen;
  };
}
