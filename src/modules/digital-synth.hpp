#pragma once

#include "core/modules/module.hpp"
#include "core/modules/faust-module.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  /**
   * Digital Synth - Lo-fi bitcrushed synthesis
   */
  class DigitalSynth : public FaustSynthModule {
    ui::ModuleScreen<DigitalSynth>::ptr screen;
    audio::RTBuffer<float> buf;
  public:

    struct Props : public Properties {
      Property<float> BITS    = {this, "BITS", 8, {2, 16, 1}};        // Blue
      Property<float> SRATE   = {this, "SRATE", 1, {1, 32, 1}};       // Green
      Property<float> WAVE    = {this, "WAVE", 0, {0, 3, 0.1}};       // White
      Property<float> GLITCH  = {this, "GLITCH", 0, {0, 1, 0.02}};    // Red
      
      Property<int, def, false> KEY        = {this, "KEY", 69, {0, 127, 1}};
      Property<float, def, false> VELOCITY = {this, "VELOCITY", 1, {0, 1, 0.01}};
      Property<bool, def, false> TRIGGER   = {this, "TRIGGER", false};
    } props;

    DigitalSynth();

    void process(const audio::ProcessData&) override;
    void display() override;
  };

} // namespace top1::modules
