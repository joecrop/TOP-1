#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/audio/faust.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"

#include "spring.faust.h"

namespace top1::modules {

  class SpringScreen;  // Forward declaration

  /**
   * Spring Effect - Spring reverb
   */
  class SpringEffect : public EffectModule {
  public:

    struct Props : public Properties {
      Property<float> tension = {this, "TENSION", 0.5f, {0.f, 1.f, 0.01f}};
      Property<float> mix     = {this, "MIX", 0.5f, {0.f, 1.f, 0.01f}};
      Property<float> decay   = {this, "DECAY", 0.6f, {0.f, 1.f, 0.01f}};
      Property<float> tone    = {this, "TONE", 0.5f, {0.f, 1.f, 0.01f}};
      Property<bool> bypassed = {this, "bypassed", false};
    } props;

    SpringEffect();
    ~SpringEffect() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    void toggleBypass() { props.bypassed = !props.bypassed.get(); }
    bool isBypassed() const { return props.bypassed.get(); }

  private:
    audio::FaustWrapper faust;
    std::unique_ptr<SpringScreen> screen;
  };

  /**
   * Spring Effect Screen
   */
  class SpringScreen : public ui::ModuleScreen<SpringEffect> {
  public:
    using ui::ModuleScreen<SpringEffect>::ModuleScreen;

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  private:
    // Animation state
    float springBob = 0;
    float springVel = 0;
  };

} // namespace top1::modules
