#include <string>
#include <cmath>

#include "modules/wavetable-synth.hpp"
#include "modules/wavetable-synth.faust.h"
#include "core/ui/drawing.hpp"
#include "core/globals.hpp"
#include "util/math.hpp"

namespace top1::modules {

  void WavetableSynth::display() {
    Globals::ui.display(*screen);
  }

  void WavetableSynth::process(const audio::ProcessData& data) {
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

  class WavetableDisplay : public Drawable {
    float position;
    float detune;
  public:
    WavetableDisplay(float p, float d) : position(p), detune(d) {}
    
    void draw(Canvas& ctx) override {
      float vizW = 240;
      float vizH = 80;
      
      // Draw waveform based on position
      ctx.beginPath();
      ctx.strokeStyle(Colours::White);
      ctx.lineWidth(2);
      
      int numPoints = 64;
      for (int i = 0; i <= numPoints; i++) {
        float t = (float)i / numPoints;
        float phase = t * 2 * M_PI;
        
        // Calculate morphed waveform
        float sine = std::sin(phase);
        float tri = 2.0f * std::abs(2.0f * (t - std::floor(t + 0.5f))) - 1.0f;
        float saw = 2.0f * (t - std::floor(t + 0.5f));
        float sqr = std::sin(phase) >= 0 ? 1.0f : -1.0f;
        
        // Morph between waveforms
        float value;
        if (position < 0.333f) {
          float morph = position / 0.333f;
          value = sine * (1 - morph) + tri * morph;
        } else if (position < 0.666f) {
          float morph = (position - 0.333f) / 0.333f;
          value = tri * (1 - morph) + saw * morph;
        } else {
          float morph = (position - 0.666f) / 0.334f;
          value = saw * (1 - morph) + sqr * morph;
        }
        
        float x = t * vizW;
        float y = vizH/2 - value * vizH * 0.4f;
        
        if (i == 0) {
          ctx.moveTo(x, y);
        } else {
          ctx.lineTo(x, y);
        }
      }
      ctx.stroke();
      
      // Show detune (spread indicator)
      if (detune > 0) {
        float spread = detune * 10;
        ctx.beginPath();
        ctx.strokeStyle(Colours::Green.dim(0.5f));
        ctx.lineWidth(1);
        ctx.moveTo(vizW/2, 5);
        ctx.lineTo(vizW/2 - spread, 15);
        ctx.moveTo(vizW/2, 5);
        ctx.lineTo(vizW/2 + spread, 15);
        ctx.stroke();
      }
    }
  };

} // namespace top1::ui::drawing

namespace top1::modules {

  // WavetableSynthScreen - concrete screen class
  class WavetableSynthScreen : public ui::ModuleScreen<WavetableSynth> {
    virtual void draw(ui::drawing::Canvas& ctx) override;
    virtual bool keypress(ui::Key key) override;
    virtual void rotary(ui::RotaryEvent e) override;
  public:
    using ui::ModuleScreen<WavetableSynth>::ModuleScreen;
  };

  bool WavetableSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void WavetableSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.POSITION.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.DETUNE.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.ATTACK.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.RELEASE.step(e.clicks);
      break;
    }
  }

  void WavetableSynthScreen::draw(ui::drawing::Canvas& ctx) {
    using namespace ui;
    using namespace drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("WAVETABLE", 160, 20);

    // Show waveform names
    ctx.font(Fonts::Norm);
    ctx.font(14);
    const char* waveNames[] = {"SINE", "TRI", "SAW", "SQR"};
    float pos = module->props.POSITION;
    
    for (int i = 0; i < 4; i++) {
      float wavePos = i / 3.0f;
      float dist = std::abs(pos - wavePos);
      float alpha = std::max(0.3f, 1.0f - dist * 2);
      
      ctx.fillStyle(Colours::White.dim(alpha));
      float x = 40 + i * 80;
      ctx.fillText(waveNames[i], x, 45);
    }

    // Draw wavetable visualization
    ctx.save();
    ctx.translate(40, 65);
    WavetableDisplay(module->props.POSITION, module->props.DETUNE).draw(ctx);
    ctx.restore();

    // Position slider
    float sliderY = 155;
    ctx.beginPath();
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(1);
    ctx.moveTo(40, sliderY);
    ctx.lineTo(280, sliderY);
    ctx.stroke();
    
    ctx.beginPath();
    ctx.fillStyle(Colours::Blue);
    float posX = 40 + pos * 240;
    ctx.circle(posX, sliderY, 6);
    ctx.fill();

    // Parameters
    float paramY = 175;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Position (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("POS", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.POSITION.get() * 100), 50, paramY + 14);

    // Detune (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DETUNE", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.DETUNE.get() * 200), 120, paramY + 14);

    // Attack (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("ATK", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    if (module->props.ATTACK < 1.0f) {
      ctx.fillText(fmt::format("{:.0f}ms", module->props.ATTACK.get() * 1000), 200, paramY + 14);
    } else {
      ctx.fillText(fmt::format("{:.2f}s", module->props.ATTACK.get()), 200, paramY + 14);
    }

    // Release (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("REL", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    if (module->props.RELEASE < 1.0f) {
      ctx.fillText(fmt::format("{:.0f}ms", module->props.RELEASE.get() * 1000), 270, paramY + 14);
    } else {
      ctx.fillText(fmt::format("{:.2f}s", module->props.RELEASE.get()), 270, paramY + 14);
    }
  }

  // WavetableSynth Constructor
  WavetableSynth::WavetableSynth() :
    FaustSynthModule(std::make_unique<FAUSTCLASS>(), &props),
    screen(new WavetableSynthScreen(this)) {}

} // namespace top1::modules
