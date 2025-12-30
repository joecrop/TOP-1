#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "chorus.faust.h"

namespace top1::modules {

  class ChorusScreen;

  /**
   * Chorus Effect - Stereo chorus with modulated delay
   */
  class ChorusEffect : public EffectModule {
  public:
    struct Props : public Properties {
      Property<float> rate   = {this, "rate", 1.5, {0.1, 5.0, 0.01}};    // Blue
      Property<float> depth  = {this, "depth", 3.0, {0.5, 10.0, 0.1}};   // Green
      Property<float> mix    = {this, "mix", 0.5, {0.0, 1.0, 0.01}};     // White
      Property<int> voices   = {this, "voices", 2, {1, 4, 1}};           // Red
      Property<bool> bypassed = {this, "bypassed", false};
    } props;

    ChorusEffect();
    ~ChorusEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<ChorusScreen> screen;
  };

  class ChorusScreen : public ui::ModuleScreen<ChorusEffect> {
  public:
    using ui::ModuleScreen<ChorusEffect>::ModuleScreen;
    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
