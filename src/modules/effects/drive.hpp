#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "drive.faust.h"

namespace top1::modules {

  class DriveScreen;

  /**
   * Drive Effect - Tube-style overdrive/distortion
   */
  class DriveEffect : public EffectModule {
  public:
    struct Props : public Properties {
      Property<float> drive  = {this, "drive", 0.5, {0.0, 1.0, 0.01}};   // Blue
      Property<float> tone   = {this, "tone", 0.5, {0.0, 1.0, 0.01}};    // Green
      Property<float> output = {this, "output", 0.5, {0.0, 1.0, 0.01}};  // White
      Property<float> mix    = {this, "mix", 1.0, {0.0, 1.0, 0.01}};     // Red
      Property<bool> bypassed = {this, "bypassed", false};
    } props;

    DriveEffect();
    ~DriveEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<DriveScreen> screen;
  };

  class DriveScreen : public ui::ModuleScreen<DriveEffect> {
  public:
    using ui::ModuleScreen<DriveEffect>::ModuleScreen;
    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
