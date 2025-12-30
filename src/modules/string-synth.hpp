#pragma once

#include "core/modules/module.hpp"
#include "core/modules/faust-module.hpp"
#include "core/ui/module-ui.hpp"

namespace top1::modules {

  /**
   * String Synth - Karplus-Strong plucked string synthesis
   */
  class StringSynth : public FaustSynthModule {
    ui::ModuleScreen<StringSynth>::ptr screen;
    audio::RTBuffer<float> buf;
  public:

    struct Props : public Properties {
      Property<float> DAMP    = {this, "DAMP", 0.5, {0, 1, 0.02}};      // Blue
      Property<float> DECAY   = {this, "DECAY", 2, {0.1, 10, 0.1}};     // Green
      Property<float> EXCITE  = {this, "EXCITE", 0, {0, 2, 0.1}};       // White
      Property<float> SYMP    = {this, "SYMP", 0, {0, 0.3, 0.01}};      // Red
      
      Property<int, def, false> KEY        = {this, "KEY", 69, {0, 127, 1}};
      Property<float, def, false> VELOCITY = {this, "VELOCITY", 1, {0, 1, 0.01}};
      Property<bool, def, false> TRIGGER   = {this, "TRIGGER", false};
    } props;

    StringSynth();

    void process(const audio::ProcessData&) override;
    void display() override;
    
    float stringVibration = 0.0f;  // For animation
  };

} // namespace top1::modules
