#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class VocoderSynth : public SynthModule {

    ui::ModuleScreen<VocoderSynth>::ptr screen;

  public:

    struct Props : Properties {
      Property<int> formant     = {this, "FORMANT",     2,    {1, 5, 1}};
      Property<float> carrier   = {this, "CARRIER",     1.0,  {0.0, 1.0, 0.01}};
      Property<float> modulator = {this, "MODULATOR",   1.0,  {0.0, 1.0, 0.01}};
      Property<float> bandwidth = {this, "BANDWIDTH",   0.5,  {0.1, 2.0, 0.1}};
      Property<float> attack    = {this, "ATTACK",      0.01, {0.001, 1.0, 0.01}};
      Property<float> release   = {this, "RELEASE",     0.2,  {0.01, 2.0, 0.01}};
    } props;

    VocoderSynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    static constexpr int NUM_BANDS = 16;
    
    struct BandpassFilter {
      float z1 = 0, z2 = 0;
      
      void setParams(float freq, float bandwidth, float samplerate) {
        float omega = 2.0f * M_PI * freq / samplerate;
        float bw = 2.0f * M_PI * bandwidth / samplerate;
        this->q = 1.0f / bw;
        this->c = 1.0f / std::tan(omega / 2.0f);
        this->d = 2.0f * std::cos(omega);
      }
      
      float process(float input) {
        float output = (input - c * z1 - z2) / (1.0f + c * q + c * c);
        z2 = z1;
        z1 = output;
        return output * q;
      }
      
      float q, c, d;
    };
    
    struct Voice {
      bool active = false;
      int note = 0;
      float velocity = 0;
      float frequency = 0;
      float phase = 0;
      
      float envValue = 0;
      int envSamples = 0;
      bool releasing = false;
      
      std::array<BandpassFilter, NUM_BANDS> carrierFilters;
      std::array<BandpassFilter, NUM_BANDS> modulatorFilters;
      std::array<float, NUM_BANDS> envelopes;
    };
    
    std::array<Voice, 6> voices;
    
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    
    friend class VocoderSynthScreen;
  };
}
