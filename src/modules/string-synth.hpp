#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class StringSynth : public SynthModule {

    ui::ModuleScreen<StringSynth>::ptr screen;

  public:

    struct Props : Properties {
      Property<float> tension    = {this, "TENSION",    0.5,  {0.0, 1.0, 0.01}};
      Property<float> damping    = {this, "DAMPING",    0.3,  {0.0, 1.0, 0.01}};
      Property<float> pickPos    = {this, "PICK POS",   0.2,  {0.01, 0.99, 0.01}};
      Property<float> brightness = {this, "BRIGHTNESS", 0.5,  {0.0, 1.0, 0.01}};
      Property<float> attack     = {this, "ATTACK",     0.01, {0.001, 0.5, 0.01}};
      Property<float> decay      = {this, "DECAY",      0.3,  {0.01, 3.0, 0.01}};
    } props;

    StringSynth();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    static constexpr int MAX_DELAY_SIZE = 4096;
    
    struct Voice {
      bool active = false;
      int note = 0;
      float velocity = 0;
      
      std::array<float, MAX_DELAY_SIZE> delayLine;
      int delayLength = 0;
      int writePos = 0;
      float excitation = 0;
      
      float envValue = 0;
      int envSamples = 0;
      
      float lpFilterState = 0;
    };
    
    std::array<Voice, 6> voices;
    
    void triggerVoice(int note, float velocity);
    void releaseVoice(int note);
    
    friend class StringSynthScreen;
  };
}
