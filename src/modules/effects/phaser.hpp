#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "phaser.faust.h"

namespace top1::modules {

  class PhaserScreen;

  /**
   * Phaser Effect - Classic analog-style phaser
   * 
   * Uses multiple allpass filter stages swept by LFO to create
   * characteristic phase-shifting sound.
   */
  class PhaserEffect : public EffectModule {
  public:
    struct Props : public Properties {
      Property<float> rate   = {this, "rate", 0.5, {0.05, 4, 0.05}};   // Blue: LFO rate
      Property<float> depth  = {this, "depth", 0.6, {0, 1, 0.02}};     // Green: Sweep depth
      Property<int> stages   = {this, "stages", 4, {2, 8, 1}};         // White: Stages
      Property<float> mix    = {this, "mix", 0.5, {0, 1, 0.02}};       // Red: Dry/Wet
      Property<bool> bypassed = {this, "bypassed", false};
    } props;

    PhaserEffect();
    ~PhaserEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }

    float lfoPhase = 0.0f;  // For UI animation

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<PhaserScreen> screen;
  };

  class PhaserScreen : public ui::ModuleScreen<PhaserEffect> {
  public:
    using ui::ModuleScreen<PhaserEffect>::ModuleScreen;
    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
