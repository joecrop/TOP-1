#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "cwo.faust.h"

namespace top1::modules {

  class CWOScreen;  // Forward declaration

  /**
   * CWO Effect - Tape Wow and Flutter
   */
  class CWOEffect : public EffectModule {
  public:

    struct Props : public Properties {
      Property<float> rate    = {this, "RATE", 2, {0.1, 10, 0.1}};       // Blue
      Property<float> depth   = {this, "DEPTH", 0.3, {0, 1, 0.02}};      // Green
      Property<float> flutter = {this, "FLUTTER", 0.2, {0, 1, 0.02}};    // White
      Property<float> mix     = {this, "MIX", 0.5, {0, 1, 0.02}};        // Red
      Property<bool> bypassed = {this, "bypassed", false};
    } props;

    CWOEffect();
    ~CWOEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<CWOScreen> screen;
  };

  /**
   * CWO Effect Screen
   */
  class CWOScreen : public ui::ModuleScreen<CWOEffect> {
  public:
    using ui::ModuleScreen<CWOEffect>::ModuleScreen;

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  private:
    float phase = 0;  // Animation phase
  };

} // namespace top1::modules
