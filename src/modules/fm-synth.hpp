#pragma once

#include "core/modules/module.hpp"
#include "core/modules/faust-module.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  /**
   * FM Synthesizer - 2-operator FM synthesis
   */
  class FMSynth : public FaustSynthModule {
    ui::ModuleScreen<FMSynth>::ptr screen;
    audio::RTBuffer<float> buf;
  public:

    struct Props : public Properties {
      Property<float> RATIO   = {this, "RATIO", 2.0, {0.5, 8.0, 0.1}};        // Blue
      Property<float> DEPTH   = {this, "DEPTH", 0.5, {0.0, 1.0, 0.02}};       // Green
      Property<float> ATTACK  = {this, "ATTACK", 0.01, {0.001, 1.0, 0.01}};   // White
      Property<float> RELEASE = {this, "RELEASE", 0.3, {0.01, 2.0, 0.02}};    // Red
      
      Property<int, def, false> KEY        = {this, "KEY", 69, {0, 127, 1}};
      Property<float, def, false> VELOCITY = {this, "VELOCITY", 1, {0, 1, 0.01}};
      Property<bool, def, false> TRIGGER   = {this, "TRIGGER", false};
    } props;

    FMSynth();

    void process(const audio::ProcessData&) override;
    void display() override;
  };

} // namespace top1::modules
