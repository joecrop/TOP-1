#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class PulseSynth : public SynthModule {

    ui::ModuleScreen<PulseSynth>::ptr screen;

  public:

    struct Props : Properties {
      // Oscillator parameters
      Property<float> pulseWidth = {this, "PULSE WIDTH", 0.5,  {0.0, 1.0, 0.01}};
      Property<float> detune     = {this, "DETUNE",      0.0,  {-1.0, 1.0, 0.01}};
      Property<int>   octave     = {this, "OCTAVE",      0,    {-2, 2, 1}};
      Property<float> glide      = {this, "GLIDE",       0.0,  {0.0, 1.0, 0.01}};
      
      // Filter
      Property<float> cutoff     = {this, "CUTOFF",      1.0,  {0.0, 1.0, 0.01}};
      Property<float> resonance  = {this, "RESONANCE",   0.0,  {0.0, 1.0, 0.01}};
      Property<float> filterEnv  = {this, "FILTER ENV",  0.5,  {-1.0, 1.0, 0.01}};
      
      // Envelope
      Property<float> attack     = {this, "ATTACK",      0.01, {0.001, 2.0, 0.01}};
      Property<float> decay      = {this, "DECAY",       0.3,  {0.001, 2.0, 0.01}};
      Property<float> sustain    = {this, "SUSTAIN",     0.7,  {0.0, 1.0, 0.01}};
      Property<float> release    = {this, "RELEASE",     0.5,  {0.001, 3.0, 0.01}};
      
      // LFO
      Property<float> lfoRate    = {this, "LFO RATE",    2.0,  {0.1, 20.0, 0.1}};
      Property<float> lfoDepth   = {this, "LFO DEPTH",   0.0,  {0.0, 1.0, 0.01}};
    } props;

    PulseSynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    // State Variable Filter
    struct SVFilter {
      float lowpass = 0;
      float bandpass = 0;
      float f = 0;
      float q = 0;
      
      void setParams(float cutoff, float resonance) {
        f = 2.0f * std::sin(M_PI * cutoff);
        q = 1.0f - resonance * 0.99f;
      }
      
      float process(float input) {
        lowpass += f * bandpass;
        float highpass = input - lowpass - q * bandpass;
        bandpass += f * highpass;
        return lowpass;
      }
    };
    
    // Per-voice state
    struct Voice {
      bool active = false;
      int note = 0;
      float velocity = 0;
      float targetFreq = 0;
      float currentFreq = 0;
      
      float phase = 0;
      
      // Envelope state
      enum EnvStage { Attack, Decay, Sustain, Release, Off };
      EnvStage envStage = Off;
      float envValue = 0;
      int envSamples = 0;
      int envDuration = 0;
      
      SVFilter filter;
    };
    
    std::array<Voice, 6> voices;
    float lfoPhase = 0;
    
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    float generatePulse(float phase, float width);
    void updateEnvelope(Voice& voice);
    
    friend class PulseSynthScreen;
  };
}
