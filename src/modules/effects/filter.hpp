#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "filter.faust.h"

namespace top1::modules {

  class FilterScreen;

  /**
   * Filter Effect - Resonant lowpass filter with envelope follower
   */
  class FilterEffect : public EffectModule {
  public:
    struct Props : public Properties {
      Property<float> cutoff    = {this, "cutoff", 1000, {100, 10000, 1}};  // Blue
      Property<float> resonance = {this, "reso", 0.5, {0.0, 0.95, 0.01}};   // Green
      Property<float> envAmount = {this, "env", 0.0, {-1.0, 1.0, 0.01}};    // White
      Property<float> mix       = {this, "mix", 1.0, {0.0, 1.0, 0.01}};     // Red
      Property<bool> bypassed   = {this, "bypassed", false};
    } props;

    FilterEffect();
    ~FilterEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<FilterScreen> screen;
  };

  class FilterScreen : public ui::ModuleScreen<FilterEffect> {
  public:
    using ui::ModuleScreen<FilterEffect>::ModuleScreen;
    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
