#include <string>

#include "modules/phase-synth.hpp"
#include "modules/phase-synth.faust.h"
#include "core/ui/drawing.hpp"
#include "core/globals.hpp"
#include "util/math.hpp"

namespace top1::modules {

  void PhaseSynth::display() {
    Globals::ui.display(*screen);
  }

  void PhaseSynth::process(const audio::ProcessData& data) {
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

  class PhaseDistortionDisplay : public Drawable {
    float distortion;
    float resonance;
  public:
    PhaseDistortionDisplay(float d, float r) : distortion(d), resonance(r) {}
    
    void draw(Canvas& ctx) override {
      float vizW = 240;
      float vizH = 70;
      
      // Draw phase transfer function
      ctx.beginPath();
      ctx.strokeStyle(Colours::Blue);
      ctx.lineWidth(2);
      
      float d = 0.5f + distortion * 0.4f;
      
      for (int i = 0; i <= 60; i++) {
        float t = (float)i / 60;  // Input phase (0 to 1)
        float x = t * vizW;
        
        // Phase distortion transfer function
        float output;
        if (t < d) {
          output = t / (2 * d) + 0.5f;
        } else {
          output = (t - d) / (2 * (1 - d));
        }
        output = std::fmod(output, 1.0f);
        
        float y = vizH - output * vizH;
        
        if (i == 0) {
          ctx.moveTo(x, y);
        } else {
          ctx.lineTo(x, y);
        }
      }
      ctx.stroke();
      
      // Draw resulting waveform below
      ctx.beginPath();
      ctx.strokeStyle(Colours::Green);
      ctx.lineWidth(1.5f);
      
      for (int i = 0; i <= 60; i++) {
        float t = (float)i / 60;
        float x = t * vizW;
        
        float d = 0.5f + distortion * 0.4f;
        float output;
        if (t < d) {
          output = t / (2 * d) + 0.5f;
        } else {
          output = (t - d) / (2 * (1 - d));
        }
        
        // Convert to cosine wave
        float wave = std::cos(output * 2 * M_PI);
        
        // Add resonance harmonic
        wave = wave * (1 - resonance * 0.5f) + 
               std::sin(t * 4 * M_PI) * resonance * 0.5f;
        
        float y = vizH / 2 - wave * vizH * 0.35f;
        
        if (i == 0) {
          ctx.moveTo(x, y);
        } else {
          ctx.lineTo(x, y);
        }
      }
      ctx.stroke();
      
      // Distortion point marker
      float dX = d * vizW;
      ctx.beginPath();
      ctx.strokeStyle(Colours::Red);
      ctx.moveTo(dX, 0);
      ctx.lineTo(dX, vizH);
      ctx.stroke();
    }
  };

} // namespace top1::ui::drawing

namespace top1::modules {

  // PhaseSynthScreen - concrete screen class
  class PhaseSynthScreen : public ui::ModuleScreen<PhaseSynth> {
    virtual void draw(ui::drawing::Canvas& ctx) override;
    virtual bool keypress(ui::Key key) override;
    virtual void rotary(ui::RotaryEvent e) override;
  public:
    using ui::ModuleScreen<PhaseSynth>::ModuleScreen;
  };

  bool PhaseSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void PhaseSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.DISTORT.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.RESO.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.MODDEPTH.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.MODRATE.step(e.clicks);
      break;
    }
  }

  void PhaseSynthScreen::draw(ui::drawing::Canvas& ctx) {
    using namespace ui;
    using namespace drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("PHASE", 160, 20);

    // Draw phase distortion visualization
    ctx.save();
    ctx.translate(40, 45);
    PhaseDistortionDisplay(module->props.DISTORT, module->props.RESO).draw(ctx);
    ctx.restore();

    // Parameters
    float paramY = 145;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Distortion (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("DISTORT", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.0f}%", module->props.DISTORT.get() * 100), 50, paramY + 14);

    // Resonance (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("RESO", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.0f}%", module->props.RESO.get() * 100), 120, paramY + 14);

    // Mod Depth (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("DEPTH", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.0f}%", module->props.MODDEPTH.get() * 100), 200, paramY + 14);

    // Mod Rate (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("RATE", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.1f}", module->props.MODRATE.get()), 270, paramY + 14);
  }

  // PhaseSynth Constructor
  PhaseSynth::PhaseSynth() :
    FaustSynthModule(std::make_unique<FAUSTCLASS>(), &props),
    screen(new PhaseSynthScreen(this)) {}

} // namespace top1::modules
