#include <string>

#include "modules/fm-synth.hpp"
#include "modules/fm-synth.faust.h"
#include "core/ui/drawing.hpp"
#include "core/globals.hpp"
#include "util/math.hpp"

namespace top1::modules {

  void FMSynth::display() {
    Globals::ui.display(*screen);
  }

  void FMSynth::process(const audio::ProcessData& data) {
    for (auto &&nEvent : data.midi) {
      nEvent.match([&] (midi::NoteOnEvent& e) {
          if (e.channel == 0) {
            props.KEY = e.key;
            props.TRIGGER = 1;
            props.VELOCITY = float(e.velocity)/128.f;
          }
        }, [] (auto&&) {});
    }
    buf.clear();
    FaustSynthModule::process({buf.data(), data.nframes});
    for_both(buf.begin(), buf.end(), data.audio.proc.begin(),
      data.audio.proc.end(), [] (auto in, auto& out) {out += in;});
    for (auto &&nEvent : data.midi) {
      nEvent.match([&] (midi::NoteOffEvent& e) {
          if (e.channel == 0) {
            if (e.key == props.KEY) {
              props.TRIGGER = 0;
            }
          }
        }, [] (auto&&) {});
    };
  }

} // top1::modules

  /****************************************/
  /* Screen drawing                       */
  /****************************************/

namespace top1::ui::drawing {

  class FMDiagram : public Drawable {
    float ratio;
    float depth;
  public:
    FMDiagram(float r, float d) : ratio(r), depth(d) {}
    
    void draw(Canvas& ctx) override {
      // Modulator circle
      float modRadius = 25 + depth * 15;
      ctx.beginPath();
      ctx.arc(0, -40, modRadius, 0, 2 * M_PI);
      ctx.strokeStyle(Colours::Green);
      ctx.lineWidth(2);
      ctx.stroke();
      ctx.font(Fonts::Norm);
      ctx.font(12);
      ctx.fillStyle(Colours::Green);
      ctx.textAlign(TextAlign::Center, TextAlign::Middle);
      ctx.fillText("MOD", 0, -40);

      // Arrow from mod to carrier
      ctx.beginPath();
      ctx.moveTo(0, -40 + modRadius + 2);
      ctx.lineTo(0, 20);
      ctx.strokeStyle(Colours::Gray60);
      ctx.lineWidth(2);
      ctx.stroke();
      ctx.beginPath();
      ctx.moveTo(-5, 15);
      ctx.lineTo(0, 20);
      ctx.lineTo(5, 15);
      ctx.stroke();

      // Carrier circle
      ctx.beginPath();
      ctx.arc(0, 40, 25, 0, 2 * M_PI);
      ctx.strokeStyle(Colours::Blue);
      ctx.lineWidth(2);
      ctx.stroke();
      ctx.fillStyle(Colours::Blue);
      ctx.fillText("CAR", 0, 40);

      // Ratio indicator
      ctx.font(Fonts::Bold);
      ctx.font(14);
      ctx.fillStyle(Colours::White);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText(fmt::format("{:.1f}:1", ratio), -50, 0);
    }
  };

} // namespace top1::ui::drawing

namespace top1::modules {

  // FMSynthScreen - concrete screen class
  class FMSynthScreen : public ui::ModuleScreen<FMSynth> {
    virtual void draw(ui::drawing::Canvas& ctx) override;
    virtual bool keypress(ui::Key key) override;
    virtual void rotary(ui::RotaryEvent e) override;
  public:
    using ui::ModuleScreen<FMSynth>::ModuleScreen;
  };

  bool FMSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void FMSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.RATIO.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.DEPTH.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.ATTACK.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.RELEASE.step(e.clicks);
      break;
    }
  }

  void FMSynthScreen::draw(ui::drawing::Canvas& ctx) {
    using namespace ui;
    using namespace drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("FM SYNTH", 160, 20);

    // Draw FM diagram
    ctx.save();
    ctx.translate(160, 90);
    FMDiagram(module->props.RATIO, module->props.DEPTH).draw(ctx);
    ctx.restore();

    // Parameters
    float paramY = 160;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Ratio (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("RATIO", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.1f}", module->props.RATIO.get()), 50, paramY + 14);

    // Depth (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DEPTH", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.DEPTH.get() * 100), 120, paramY + 14);

    // Attack (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("ATK", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}ms", module->props.ATTACK.get() * 1000), 200, paramY + 14);

    // Release (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("REL", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.2f}s", module->props.RELEASE.get()), 270, paramY + 14);
  }

  // FMSynth Constructor
  FMSynth::FMSynth() :
    FaustSynthModule(std::make_unique<FAUSTCLASS>(), &props),
    screen(new FMSynthScreen(this)) {}

} // namespace top1::modules
