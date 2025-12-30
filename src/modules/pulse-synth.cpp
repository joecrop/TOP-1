#include <string>

#include "modules/pulse-synth.hpp"
#include "modules/pulse-synth.faust.h"
#include "core/ui/drawing.hpp"
#include "core/globals.hpp"
#include "util/math.hpp"

namespace top1::modules {

  void PulseSynth::display() {
    Globals::ui.display(*screen);
  }

  void PulseSynth::process(const audio::ProcessData& data) {
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

  class PulseWave : public Drawable {
    float width;
    float pwm;
  public:
    PulseWave(float w, float p) : width(w), pwm(p) {}
    
    void draw(Canvas& ctx) override {
      float waveW = 240;
      float waveH = 60;

      // Center line
      ctx.beginPath();
      ctx.moveTo(0, 0);
      ctx.lineTo(waveW, 0);
      ctx.strokeStyle(Colours::Gray60);
      ctx.lineWidth(1);
      ctx.stroke();

      // Draw pulse wave (2 cycles)
      ctx.beginPath();
      ctx.strokeStyle(Colours::Blue);
      ctx.lineWidth(2);
      
      float cycleW = waveW / 2;
      for (int c = 0; c < 2; c++) {
        float x0 = c * cycleW;
        float highW = cycleW * width;
        
        // High portion
        ctx.moveTo(x0, -waveH/2 + 5);
        ctx.lineTo(x0 + highW, -waveH/2 + 5);
        
        // Falling edge
        ctx.lineTo(x0 + highW, waveH/2 - 5);
        
        // Low portion
        ctx.lineTo(x0 + cycleW, waveH/2 - 5);
        
        // Rising edge (except last cycle)
        if (c < 1) {
          ctx.lineTo(x0 + cycleW, -waveH/2 + 5);
        }
      }
      ctx.stroke();

      // PWM indicator
      if (pwm > 0) {
        ctx.font(Fonts::Norm);
        ctx.font(10);
        ctx.fillStyle(Colours::Green);
        ctx.textAlign(TextAlign::Center, TextAlign::Bottom);
        ctx.fillText("~PWM~", waveW/2, -waveH/2 - 5);
      }
    }
  };

} // namespace top1::ui::drawing

namespace top1::modules {

  // PulseSynthScreen - concrete screen class
  class PulseSynthScreen : public ui::ModuleScreen<PulseSynth> {
    virtual void draw(ui::drawing::Canvas& ctx) override;
    virtual bool keypress(ui::Key key) override;
    virtual void rotary(ui::RotaryEvent e) override;
  public:
    using ui::ModuleScreen<PulseSynth>::ModuleScreen;
  };

  bool PulseSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void PulseSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.WIDTH.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.PWM.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.ATTACK.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.RELEASE.step(e.clicks);
      break;
    }
  }

  void PulseSynthScreen::draw(ui::drawing::Canvas& ctx) {
    using namespace ui;
    using namespace drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("PULSE", 160, 20);

    // Draw pulse wave visualization
    ctx.save();
    ctx.translate(40, 100);
    PulseWave(module->props.WIDTH, module->props.PWM).draw(ctx);
    ctx.restore();

    // Parameters
    float paramY = 160;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Width (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("WIDTH", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.WIDTH.get() * 100), 50, paramY + 14);

    // PWM (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("PWM", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.1f}Hz", module->props.PWM.get()), 120, paramY + 14);

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

  // PulseSynth Constructor
  PulseSynth::PulseSynth() :
    FaustSynthModule(std::make_unique<FAUSTCLASS>(), &props),
    screen(new PulseSynthScreen(this)) {}

} // namespace top1::modules
