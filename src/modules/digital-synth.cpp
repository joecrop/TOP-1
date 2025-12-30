#include <string>

#include "modules/digital-synth.hpp"
#include "modules/digital-synth.faust.h"
#include "core/ui/drawing.hpp"
#include "core/globals.hpp"
#include "util/math.hpp"

namespace top1::modules {

  void DigitalSynth::display() {
    Globals::ui.display(*screen);
  }

  void DigitalSynth::process(const audio::ProcessData& data) {
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

  class BitDisplay : public Drawable {
    int bits;
    float wave;
  public:
    BitDisplay(int b, float w) : bits(b), wave(w) {}
    
    void draw(Canvas& ctx) override {
      float vizW = 240;
      float vizH = 60;
      
      // Draw stepped waveform representing bit depth
      int steps = std::pow(2, std::min(bits, 6));
      float stepH = vizH / steps;
      
      ctx.beginPath();
      ctx.strokeStyle(Colours::Blue);
      ctx.lineWidth(2);
      
      int numSamples = 60;
      float prevY = vizH / 2;
      
      for (int i = 0; i < numSamples; i++) {
        float t = (float)i / numSamples;
        float x = t * vizW;
        
        // Generate wave value based on wave type
        float waveVal;
        if (wave < 1) {
          waveVal = std::fmod(t * 4, 1.0f) * 2 - 1;  // Saw
        } else if (wave < 2) {
          waveVal = (std::fmod(t * 4, 1.0f) > 0.5f) ? 1 : -1;  // Square
        } else if (wave < 3) {
          waveVal = std::abs(std::fmod(t * 4, 1.0f) * 2 - 1) * 2 - 1;  // Tri
        } else {
          waveVal = std::sin(t * 31.4f + i * 0.7f);  // Pseudo-noise
        }
        
        // Quantize to bit depth
        float quantized = std::floor(waveVal * steps / 2) / (steps / 2);
        float y = vizH / 2 - quantized * vizH * 0.4f;
        
        // Draw as steps
        if (i == 0) {
          ctx.moveTo(x, y);
        } else {
          ctx.lineTo(x, prevY);  // Horizontal
          ctx.lineTo(x, y);      // Vertical step
        }
        prevY = y;
      }
      ctx.stroke();
      
      // Bit depth indicator bars on the right
      ctx.font(10);
      ctx.fillStyle(Colours::Gray60);
      for (int i = 0; i < std::min(bits, 8); i++) {
        float y = 5 + i * 7;
        ctx.beginPath();
        if (i < bits) {
          ctx.fillStyle(Colours::Blue);
        } else {
          ctx.fillStyle(Colours::Gray60.dim(0.3f));
        }
        ctx.rect(vizW + 5, y, 15, 5);
        ctx.fill();
      }
    }
  };

} // namespace top1::ui::drawing

namespace top1::modules {

  // DigitalSynthScreen - concrete screen class
  class DigitalSynthScreen : public ui::ModuleScreen<DigitalSynth> {
    virtual void draw(ui::drawing::Canvas& ctx) override;
    virtual bool keypress(ui::Key key) override;
    virtual void rotary(ui::RotaryEvent e) override;
  public:
    using ui::ModuleScreen<DigitalSynth>::ModuleScreen;
  };

  bool DigitalSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void DigitalSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.BITS.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.SRATE.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.WAVE.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.GLITCH.step(e.clicks);
      break;
    }
  }

  void DigitalSynthScreen::draw(ui::drawing::Canvas& ctx) {
    using namespace ui;
    using namespace drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("DIGITAL", 160, 20);

    // Wave type label
    const char* waveNames[] = {"SAW", "SQUARE", "TRI", "NOISE"};
    int waveIdx = std::min(3, (int)module->props.WAVE.get());
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.fillStyle(Colours::Gray60);
    ctx.fillText(waveNames[waveIdx], 160, 40);

    // Draw bit display visualization
    ctx.save();
    ctx.translate(20, 55);
    BitDisplay((int)module->props.BITS.get(), module->props.WAVE.get()).draw(ctx);
    ctx.restore();

    // Parameters
    float paramY = 140;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Bits (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("BITS", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{}", (int)module->props.BITS.get()), 50, paramY + 14);

    // Sample Rate (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("S.RATE", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("1/{}", (int)module->props.SRATE.get()), 120, paramY + 14);

    // Wave (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("WAVE", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(16);
    ctx.fillText(waveNames[waveIdx], 200, paramY + 16);

    // Glitch (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("GLITCH", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.GLITCH.get() * 100), 270, paramY + 14);
  }

  // DigitalSynth Constructor
  DigitalSynth::DigitalSynth() :
    FaustSynthModule(std::make_unique<FAUSTCLASS>(), &props),
    screen(new DigitalSynthScreen(this)) {}

} // namespace top1::modules
