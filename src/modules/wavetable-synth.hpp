#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>
#include <vector>

namespace top1::modules {

  class WavetableSynth : public SynthModule {

    ui::ModuleScreen<WavetableSynth>::ptr screen;

  public:

    struct Props : Properties {
      // Wavetable parameters
      Property<int> waveform    = {this, "WAVEFORM",    1,    {1, 8, 1}};
      Property<float> position  = {this, "POSITION",    0.0,  {0.0, 1.0, 0.01}};
      Property<float> morph     = {this, "MORPH",       0.0,  {0.0, 1.0, 0.01}};
      
      // Oscillator
      Property<float> detune    = {this, "DETUNE",      0.0,  {-1.0, 1.0, 0.01}};
      Property<int> octave      = {this, "OCTAVE",      0,    {-2, 2, 1}};
      
      // Filter
      Property<float> cutoff    = {this, "CUTOFF",      1.0,  {0.0, 1.0, 0.01}};
      Property<float> resonance = {this, "RESONANCE",   0.2,  {0.0, 1.0, 0.01}};
      
      // Envelope
      Property<float> attack    = {this, "ATTACK",      0.01, {0.001, 2.0, 0.01}};
      Property<float> decay     = {this, "DECAY",       0.2,  {0.001, 2.0, 0.01}};
      Property<float> sustain   = {this, "SUSTAIN",     0.7,  {0.0, 1.0, 0.01}};
      Property<float> release   = {this, "RELEASE",     0.3,  {0.001, 3.0, 0.01}};
    } props;

    WavetableSynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    static constexpr int WAVETABLE_SIZE = 2048;
    static constexpr int NUM_WAVETABLES = 8;
    
    // Wavetables
    std::array<std::array<float, WAVETABLE_SIZE>, NUM_WAVETABLES> wavetables;
    
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
      float frequency = 0;
      
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
    
    void initWavetables();
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    float sampleWavetable(int tableIndex, float phase, float morph);
    void updateEnvelope(Voice& voice);
    
    friend class WavetableSynthScreen;
  };
}
