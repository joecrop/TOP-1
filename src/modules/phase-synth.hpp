#pragma once

#include "core/modules/module.hpp"
#include "core/modules/faust-module.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  /**
   * Phase Synth - Phase Distortion synthesis (CZ-style)
   */
  class PhaseSynth : public FaustSynthModule {
    ui::ModuleScreen<PhaseSynth>::ptr screen;
    audio::RTBuffer<float> buf;
  public:

    struct Props : public Properties {
      Property<float> DISTORT   = {this, "DISTORT", 0.5, {0, 1, 0.02}};     // Blue
      Property<float> RESO      = {this, "RESO", 0.5, {0, 1, 0.02}};        // Green
      Property<float> MODDEPTH  = {this, "MODDEPTH", 0.3, {0, 1, 0.02}};    // White
      Property<float> MODRATE   = {this, "MODRATE", 2, {0.1, 10, 0.1}};     // Red
      
      Property<int, def, false> KEY        = {this, "KEY", 69, {0, 127, 1}};
      Property<float, def, false> VELOCITY = {this, "VELOCITY", 1, {0, 1, 0.01}};
      Property<bool, def, false> TRIGGER   = {this, "TRIGGER", false};
    } props;

    PhaseSynth();

    void process(const audio::ProcessData&) override;
    void display() override;
  };

} // namespace top1::modules
