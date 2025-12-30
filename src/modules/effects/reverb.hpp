#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "reverb.faust.h"

namespace top1::modules {

  class ReverbScreen;  // Forward declaration

  /**
   * Reverb Effect - Schroeder-style spring reverb
   */
  class ReverbEffect : public EffectModule {
  public:

    struct Props : public Properties {
      Property<float> size     = {this, "size",     0.5,  {0.1, 1.0, 0.01}};   // Blue knob
      Property<float> damp     = {this, "damp",     0.5,  {0.0, 1.0, 0.01}};   // Green knob
      Property<float> mix      = {this, "mix",      0.3,  {0.0, 1.0, 0.01}};   // White knob
      Property<float> decay    = {this, "decay",    0.5,  {0.1, 0.95, 0.01}}; // Red knob
      Property<bool> bypassed  = {this, "bypassed", false};
    } props;

    ReverbEffect();
    ~ReverbEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<ReverbScreen> screen;
  };

  /**
   * Reverb Effect Screen
   */
  class ReverbScreen : public ui::ModuleScreen<ReverbEffect> {
  public:
    using ui::ModuleScreen<ReverbEffect>::ModuleScreen;

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
