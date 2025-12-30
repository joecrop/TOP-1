#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "nitro.faust.h"

namespace top1::modules {

  class NitroScreen;  // Forward declaration

  /**
   * Nitro Effect - Compressor/Boost
   */
  class NitroEffect : public EffectModule {
  public:

    struct Props : public Properties {
      Property<float> drive   = {this, "DRIVE", 0.5, {0, 1, 0.02}};       // Blue
      Property<float> ratio   = {this, "RATIO", 4, {1, 20, 0.5}};         // Green
      Property<float> attack  = {this, "ATTACK", 0.01, {0.001, 0.1, 0.002}};  // White
      Property<float> release = {this, "RELEASE", 0.1, {0.01, 1, 0.01}};  // Red
      Property<bool> bypassed = {this, "bypassed", false};
    } props;

    NitroEffect();
    ~NitroEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;
    
    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }
    
    // For visualization
    float gainReduction = 0.0f;

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<NitroScreen> screen;
  };

  /**
   * Nitro Effect Screen
   */
  class NitroScreen : public ui::ModuleScreen<NitroEffect> {
  public:
    using ui::ModuleScreen<NitroEffect>::ModuleScreen;

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
