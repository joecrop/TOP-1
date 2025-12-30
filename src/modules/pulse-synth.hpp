#pragma once

#include "core/modules/module.hpp"
#include "core/modules/faust-module.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  /**
   * Pulse Synthesizer - Classic pulse wave with PWM
   */
  class PulseSynth : public FaustSynthModule {
    ui::ModuleScreen<PulseSynth>::ptr screen;
    audio::RTBuffer<float> buf;
  public:

    struct Props : public Properties {
      Property<float> WIDTH   = {this, "WIDTH", 0.5, {0.05, 0.95, 0.02}};    // Blue
      Property<float> PWM     = {this, "PWM", 0.0, {0.0, 10.0, 0.2}};        // Green
      Property<float> ATTACK  = {this, "ATTACK", 0.01, {0.001, 1.0, 0.01}};  // White
      Property<float> RELEASE = {this, "RELEASE", 0.3, {0.01, 2.0, 0.02}};   // Red
      
      Property<int, def, false> KEY        = {this, "KEY", 69, {0, 127, 1}};
      Property<float, def, false> VELOCITY = {this, "VELOCITY", 1, {0, 1, 0.01}};
      Property<bool, def, false> TRIGGER   = {this, "TRIGGER", false};
    } props;

    PulseSynth();

    void process(const audio::ProcessData&) override;
    void display() override;
  };

} // namespace top1::modules
