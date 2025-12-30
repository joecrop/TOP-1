#include <string>
#include <cmath>
#include <random>

#include "modules/string-synth.hpp"
#include "modules/string-synth.faust.h"
#include "core/ui/drawing.hpp"
#include "core/globals.hpp"
#include "util/math.hpp"

namespace top1::modules {

  void StringSynth::display() {
    Globals::ui.display(*screen);
  }

  void StringSynth::process(const audio::ProcessData& data) {
    for (auto &&nEvent : data.midi) {
      nEvent.match([&] (midi::NoteOnEvent& e) {
          if (e.channel == 0) {
            props.KEY = e.key;
            props.TRIGGER = 1;
            props.VELOCITY = float(e.velocity)/128.f;
            stringVibration = 1.0f;  // Trigger animation
          }
        }, [] (auto&&) {});
    }
    
    // Decay string vibration for animation
    stringVibration *= 0.998f;
    
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

  class StringDisplay : public Drawable {
    float damp;
    float vibration;
    float symp;
  public:
    StringDisplay(float d, float v, float s) : damp(d), vibration(v), symp(s) {}
    
    void draw(Canvas& ctx) override {
      float vizW = 240;
      float vizH = 60;
      int numPoints = 32;
      
      // Draw main string
      ctx.beginPath();
      ctx.strokeStyle(Colours::White);
      ctx.lineWidth(2);
      
      for (int i = 0; i <= numPoints; i++) {
        float t = (float)i / numPoints;
        
        // String vibration envelope (pinned at ends)
        float envelope = std::sin(t * M_PI);
        
        // Multiple harmonics based on damping (brighter = more harmonics)
        float harm1 = std::sin(t * M_PI) * vibration;
        float harm2 = std::sin(t * M_PI * 2) * vibration * (1 - damp) * 0.5f;
        float harm3 = std::sin(t * M_PI * 3) * vibration * (1 - damp) * (1 - damp) * 0.25f;
        
        float vib = (harm1 + harm2 + harm3) * envelope * 20;
        
        float x = t * vizW;
        float y = vizH/2 + vib;
        
        if (i == 0) {
          ctx.moveTo(x, y);
        } else {
          ctx.lineTo(x, y);
        }
      }
      ctx.stroke();
      
      // Draw sympathetic strings if symp > 0
      if (symp > 0) {
        for (int s = 0; s < 2; s++) {
          float yOffset = (s == 0) ? -25 : 25;
          
          ctx.beginPath();
          ctx.strokeStyle(Colours::White.dim(0.3f + symp * 0.5f));
          ctx.lineWidth(1);
          
          for (int i = 0; i <= numPoints; i++) {
            float t = (float)i / numPoints;
            float envelope = std::sin(t * M_PI);
            float vib = std::sin(t * M_PI + s * 0.5f) * vibration * envelope * 15;
            
            float x = t * vizW;
            float y = yOffset + vizH/2 + vib;
            
            if (i == 0) {
              ctx.moveTo(x, y);
            } else {
              ctx.lineTo(x, y);
            }
          }
          ctx.stroke();
        }
      }
      
      // String end points (bridge/nut)
      ctx.beginPath();
      ctx.fillStyle(Colours::Gray60);
      ctx.rect(-5, 0, 10, vizH);
      ctx.fill();
      ctx.beginPath();
      ctx.rect(vizW - 5, 0, 10, vizH);
      ctx.fill();
    }
  };

} // namespace top1::ui::drawing

namespace top1::modules {

  // StringSynthScreen - concrete screen class
  class StringSynthScreen : public ui::ModuleScreen<StringSynth> {
    virtual void draw(ui::drawing::Canvas& ctx) override;
    virtual bool keypress(ui::Key key) override;
    virtual void rotary(ui::RotaryEvent e) override;
  public:
    using ui::ModuleScreen<StringSynth>::ModuleScreen;
  };

  bool StringSynthScreen::keypress(ui::Key key) {
    if (key == ui::Key::K_PLAY) {
      // Trigger string vibration for animation
      module->stringVibration = 1.0f;
      return true;
    }
    return false;
  }

  void StringSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.DAMP.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.DECAY.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.EXCITE.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.SYMP.step(e.clicks);
      break;
    }
  }

  void StringSynthScreen::draw(ui::drawing::Canvas& ctx) {
    using namespace ui;
    using namespace drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("STRING", 160, 20);

    // Excitation type label
    ctx.font(Fonts::Norm);
    ctx.font(16);
    ctx.fillStyle(Colours::Gray60);
    const char* exciteNames[] = {"NOISE", "PLUCK", "BOW"};
    int exciteIdx = std::min(2, (int)module->props.EXCITE.get());
    ctx.fillText(exciteNames[exciteIdx], 160, 42);

    // Draw string visualization
    ctx.save();
    ctx.translate(40, 70);
    StringDisplay(module->props.DAMP, module->stringVibration, module->props.SYMP).draw(ctx);
    ctx.restore();

    // Parameters
    float paramY = 160;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Damp (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("DAMP", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.DAMP.get() * 100), 50, paramY + 14);

    // Decay (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DECAY", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.1f}s", module->props.DECAY.get()), 120, paramY + 14);

    // Excite (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("EXCITE", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(exciteNames[exciteIdx], 200, paramY + 14);

    // Symp (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("SYMP", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.SYMP.get() * 333), 270, paramY + 14);
  }

  // StringSynth Constructor
  StringSynth::StringSynth() :
    FaustSynthModule(std::make_unique<FAUSTCLASS>(), &props),
    screen(new StringSynthScreen(this)) {}

} // namespace top1::modules
