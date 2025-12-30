#include <string>

#include "modules/cluster-synth.hpp"
#include "modules/cluster-synth.faust.h"
#include "core/ui/drawing.hpp"
#include "core/globals.hpp"
#include "util/math.hpp"

namespace top1::modules {

  void ClusterSynth::display() {
    Globals::ui.display(*screen);
  }

  void ClusterSynth::process(const audio::ProcessData& data) {
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

  class GrainCloud : public Drawable {
    float density;
    float spread;
    float texture;
  public:
    GrainCloud(float d, float sp, float t) : density(d), spread(sp), texture(t) {}
    
    void draw(Canvas& ctx) override {
      float vizW = 240;
      float vizH = 60;
      
      // Draw grain particles
      int numGrains = 20 + density * 30;
      
      for (int i = 0; i < numGrains; i++) {
        float t = (float)i / numGrains;
        float x = t * vizW;
        
        // Pseudo-random positioning based on spread
        float yOffset = std::sin(t * 17.3f + spread * 5) * spread * vizH * 0.4f;
        float y = vizH / 2 + yOffset;
        
        // Size based on texture
        float size = 2 + texture * 4 + std::sin(t * 23.7f) * 2;
        
        // Color intensity based on density
        float alpha = 0.3f + density * 0.5f;
        
        ctx.beginPath();
        ctx.fillStyle(Colours::Blue.dim(1.0f - t * 0.5f));
        ctx.globalAlpha(alpha);
        ctx.circle(x, y, size);
        ctx.fill();
      }
      ctx.globalAlpha(1.0f);
      
      // Draw center line
      ctx.beginPath();
      ctx.strokeStyle(Colours::Gray60);
      ctx.lineWidth(1);
      ctx.moveTo(0, vizH/2);
      ctx.lineTo(vizW, vizH/2);
      ctx.stroke();
    }
  };

} // namespace top1::ui::drawing

namespace top1::modules {

  // ClusterSynthScreen - concrete screen class
  class ClusterSynthScreen : public ui::ModuleScreen<ClusterSynth> {
    virtual void draw(ui::drawing::Canvas& ctx) override;
    virtual bool keypress(ui::Key key) override;
    virtual void rotary(ui::RotaryEvent e) override;
  public:
    using ui::ModuleScreen<ClusterSynth>::ModuleScreen;
  };

  bool ClusterSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void ClusterSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.GRAINSIZE.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.DENSITY.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.SPREAD.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.TEXTURE.step(e.clicks);
      break;
    }
  }

  void ClusterSynthScreen::draw(ui::drawing::Canvas& ctx) {
    using namespace ui;
    using namespace drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("CLUSTER", 160, 20);

    // Draw grain cloud visualization
    ctx.save();
    ctx.translate(40, 50);
    GrainCloud(module->props.DENSITY, module->props.SPREAD, module->props.TEXTURE).draw(ctx);
    ctx.restore();

    // Parameters
    float paramY = 140;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Grain Size (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("SIZE", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.0f}ms", module->props.GRAINSIZE.get()), 50, paramY + 14);

    // Density (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DENSITY", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.0f}%", module->props.DENSITY.get() * 100), 120, paramY + 14);

    // Spread (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("SPREAD", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.0f}%", module->props.SPREAD.get() * 100), 200, paramY + 14);

    // Texture (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("TEXTURE", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.0f}%", module->props.TEXTURE.get() * 100), 270, paramY + 14);
  }

  // ClusterSynth Constructor
  ClusterSynth::ClusterSynth() :
    FaustSynthModule(std::make_unique<FAUSTCLASS>(), &props),
    screen(new ClusterSynthScreen(this)) {}

} // namespace top1::modules
