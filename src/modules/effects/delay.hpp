#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "delay.faust.h"

namespace top1::modules {

  class DelayScreen;  // Forward declaration

  /**
   * Delay Effect - tempo-synced delay with feedback and filtering
   */
  class DelayEffect : public EffectModule {
  public:

    struct Props : public Properties {
      Property<float> time     = {this, "time",     0.5,  {0.01, 2.0, 0.01}};   // Blue knob
      Property<float> feedback = {this, "feedback", 0.3,  {0.0, 0.95, 0.01}};   // Green knob
      Property<float> mix      = {this, "mix",      0.5,  {0.0, 1.0, 0.01}};    // White knob
      Property<float> tone     = {this, "tone",     0.5,  {0.0, 1.0, 0.01}};    // Red knob
      Property<bool> bypassed  = {this, "bypassed", false};
    } props;

    DelayEffect();
    ~DelayEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<DelayScreen> screen;
  };

  /**
   * Delay Effect Screen
   */
  class DelayScreen : public ui::ModuleScreen<DelayEffect> {
  public:
    using ui::ModuleScreen<DelayEffect>::ModuleScreen;

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
