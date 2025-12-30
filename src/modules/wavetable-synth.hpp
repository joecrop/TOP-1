#pragma once

#include "core/modules/module.hpp"
#include "core/modules/faust-module.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  /**
   * Wavetable Synth - Morphing wavetable oscillator
   */
  class WavetableSynth : public FaustSynthModule {
    ui::ModuleScreen<WavetableSynth>::ptr screen;
    audio::RTBuffer<float> buf;
  public:

    struct Props : public Properties {
      Property<float> POSITION = {this, "POSITION", 0, {0, 1, 0.02}};         // Blue
      Property<float> DETUNE   = {this, "DETUNE", 0, {0, 0.5, 0.02}};         // Green
      Property<float> ATTACK   = {this, "ATTACK", 0.01, {0.001, 2, 0.01}};    // White
      Property<float> RELEASE  = {this, "RELEASE", 0.3, {0.01, 4, 0.02}};     // Red
      
      Property<int, def, false> KEY        = {this, "KEY", 69, {0, 127, 1}};
      Property<float, def, false> VELOCITY = {this, "VELOCITY", 1, {0, 1, 0.01}};
      Property<bool, def, false> TRIGGER   = {this, "TRIGGER", false};
    } props;

    WavetableSynth();

    void process(const audio::ProcessData&) override;
    void display() override;
  };

} // namespace top1::modules
