#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class DNASynth : public SynthModule {

    ui::ModuleScreen<DNASynth>::ptr screen;

  public:

    struct Props : Properties {
      Property<int>   tableA     = {this, "TABLE_A",    1,    {1, 8, 1}};
      Property<int>   tableB     = {this, "TABLE_B",    2,    {1, 8, 1}};
      Property<float> morph      = {this, "MORPH",      0.5,  {0.0, 1.0, 0.01}};
      Property<float> mutate     = {this, "MUTATE",     0.0,  {0.0, 1.0, 0.01}};
      Property<float> attack     = {this, "ATTACK",     0.01, {0.001, 2.0, 0.01}};
      Property<float> decay      = {this, "DECAY",      0.3,  {0.001, 2.0, 0.01}};
      Property<float> sustain    = {this, "SUSTAIN",    0.7,  {0.0, 1.0, 0.01}};
      Property<float> release    = {this, "RELEASE",    0.3,  {0.001, 3.0, 0.01}};
    } props;

    DNASynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    static constexpr int WAVETABLE_SIZE = 256;
    static constexpr int NUM_TABLES = 8;
    
    std::array<std::array<float, WAVETABLE_SIZE>, NUM_TABLES> wavetables;
    
    struct Voice {
      bool active = false;
      int note = 0;
      float velocity = 0;
      float frequency = 0;
      float phase = 0;
      
      enum EnvStage { Attack, Decay, Sustain, Release, Off };
      EnvStage envStage = Off;
      float envValue = 0;
      int envSamples = 0;
      int envDuration = 0;
    };
    
    std::array<Voice, 8> voices;
    
    void initWavetables();
    float interpolateWavetable(int tableIdx, float phase);
    float morphTables(float phase, int tableA, int tableB, float morph, float mutate);
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    void updateEnvelope(Voice& voice);
    
    friend class DNASynthScreen;
  };
}
