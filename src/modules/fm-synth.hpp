#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class FMSynth : public SynthModule {

    ui::ModuleScreen<FMSynth>::ptr screen;

  public:

    struct Props : Properties {
      // Operator frequencies (as ratios of fundamental)
      Property<float> op1Ratio = {this, "OP1 RATIO", 1.0,  {0.5, 16.0, 0.5}};
      Property<float> op2Ratio = {this, "OP2 RATIO", 2.0,  {0.5, 16.0, 0.5}};
      Property<float> op3Ratio = {this, "OP3 RATIO", 3.0,  {0.5, 16.0, 0.5}};
      Property<float> op4Ratio = {this, "OP4 RATIO", 4.0,  {0.5, 16.0, 0.5}};
      
      // Operator levels (0-1)
      Property<float> op1Level = {this, "OP1 LEVEL", 0.8, {0.0, 1.0, 0.01}};
      Property<float> op2Level = {this, "OP2 LEVEL", 0.6, {0.0, 1.0, 0.01}};
      Property<float> op3Level = {this, "OP3 LEVEL", 0.4, {0.0, 1.0, 0.01}};
      Property<float> op4Level = {this, "OP4 LEVEL", 0.3, {0.0, 1.0, 0.01}};
      
      // Envelope
      Property<float> attack  = {this, "ATTACK",  0.01, {0.001, 2.0, 0.01}};
      Property<float> decay   = {this, "DECAY",   0.1,  {0.001, 2.0, 0.01}};
      Property<float> sustain = {this, "SUSTAIN", 0.7,  {0.0, 1.0, 0.01}};
      Property<float> release = {this, "RELEASE", 0.3,  {0.001, 3.0, 0.01}};
      
      // Algorithm (FM routing topology) - 8 classic DX7-style algorithms
      Property<int> algorithm = {this, "ALGORITHM", 1, {1, 8, 1}};
      
      // Modulation indices
      Property<float> modIndex = {this, "MOD INDEX", 2.0, {0.0, 10.0, 0.1}};
    } props;

    FMSynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    // Per-voice state
    struct Voice {
      bool active = false;
      int note = 0;
      float velocity = 0;
      float baseFreq = 0;
      
      // Operator phases
      float op1Phase = 0;
      float op2Phase = 0;
      float op3Phase = 0;
      float op4Phase = 0;
      
      // Envelope state
      enum EnvStage { Attack, Decay, Sustain, Release, Off };
      EnvStage envStage = Off;
      float envValue = 0;
      int envSamples = 0;
      int envDuration = 0;
    };
    
    std::array<Voice, 6> voices;
    
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    float processOperator(float phase, float modulation);
    float processAlgorithm(Voice& voice);
    void updateEnvelope(Voice& voice);
    
    friend class FMSynthScreen;
  };
}
