#pragma once

#include "core/modules/module.hpp"
#include "core/modules/faust-module.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  /**
   * Cluster Synth - Granular cloud synthesis
   */
  class ClusterSynth : public FaustSynthModule {
    ui::ModuleScreen<ClusterSynth>::ptr screen;
    audio::RTBuffer<float> buf;
  public:

    struct Props : public Properties {
      Property<float> GRAINSIZE = {this, "GRAINSIZE", 50, {5, 200, 5}};     // Blue
      Property<float> DENSITY   = {this, "DENSITY", 0.5, {0.1, 1.0, 0.02}}; // Green
      Property<float> SPREAD    = {this, "SPREAD", 0.3, {0, 1, 0.02}};      // White
      Property<float> TEXTURE   = {this, "TEXTURE", 0.5, {0, 1, 0.02}};     // Red
      
      Property<int, def, false> KEY        = {this, "KEY", 69, {0, 127, 1}};
      Property<float, def, false> VELOCITY = {this, "VELOCITY", 1, {0, 1, 0.01}};
      Property<bool, def, false> TRIGGER   = {this, "TRIGGER", false};
    } props;

    ClusterSynth();

    void process(const audio::ProcessData&) override;
    void display() override;
  };

} // namespace top1::modules
