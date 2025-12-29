#include "phaser.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  class PhaserEffectScreen : public ui::ModuleScreen<PhaserEffect> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_lfo(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<PhaserEffect>::ModuleScreen;
  };

  PhaserEffect::PhaserEffect() :
    EffectModule(&props),
    screen (new PhaserEffectScreen(this)) {}

  void PhaserEffect::display() {
    Globals::ui.display(*screen);
  }

  void PhaserEffect::process(const audio::ProcessData& data) {
    if (props.bypassed) return;

    float samplerate = Globals::samplerate;

    for (uint i = 0; i < data.nframes; i++) {
      // Update LFO
      lfoPhase += 2.0f * M_PI * props.rate / samplerate;
      if (lfoPhase >= 2.0f * M_PI) lfoPhase -= 2.0f * M_PI;
      
      float lfo = std::sin(lfoPhase);
      
      // Map LFO to allpass coefficient (0.0 to 0.95)
      currentCoeff = 0.5f + (lfo * props.depth * 0.45f);
      
      float input = data.audio.proc[i];
      float phased = input;
      
      // Process through allpass stages
      for (int s = 0; s < props.stages; s++) {
        phased = allpass[s].process(phased, currentCoeff);
      }
      
      // Mix with feedback
      data.audio.proc[i] = input + phased * props.feedback;
    }
  }

  using namespace ui::drawing;

  bool PhaserEffectScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_RED_CLICK:
      module->props.bypassed.step(1);
      return true;
    default:
      return false;
    }
  }

  void PhaserEffectScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.rate.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.depth.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.feedback.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.stages.step(e.clicks);
      break;
    }
  }

  void PhaserEffectScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_lfo(ctx);
  }

  void PhaserEffectScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("PHASER", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Rate: {:.2f}Hz", module->props.rate.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Depth: {:.0f}%", module->props.depth.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Feedback: {:.0f}%", module->props.feedback.get() * 100), 10, 90);
    
    ctx.fillStyle(Colours::Red);
    ctx.fillText(fmt::format("Stages: {}", module->props.stages.get()), 10, 110);

    if (module->props.bypassed) {
      ctx.fillStyle(Colours::Red);
      ctx.font(Fonts::Bold);
      ctx.font(18.f);
      ctx.textAlign(TextAlign::Right, TextAlign::Bottom);
      ctx.fillText("BYPASSED", 310, 230);
    }

    ctx.restore();
  }

  void PhaserEffectScreen::draw_lfo(Canvas& ctx) {
    ctx.save();

    float lfoX = 180;
    float lfoY = 100;
    float lfoW = 120;
    float lfoH = 80;

    // Draw LFO waveform
    ctx.beginPath();
    for (int i = 0; i <= 100; i++) {
      float phase = (i / 100.0f) * 2.0f * M_PI;
      float value = std::sin(phase);
      
      float x = lfoX + (i / 100.0f) * lfoW;
      float y = lfoY + lfoH/2 - value * lfoH/2 * module->props.depth;
      
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.strokeStyle(Colours::Green);
    ctx.lineWidth(2.0f);
    ctx.stroke();

    // Draw current position
    float currentX = lfoX + (module->lfoPhase / (2.0f * M_PI)) * lfoW;
    float currentY = lfoY + lfoH/2 - std::sin(module->lfoPhase) * lfoH/2 * module->props.depth;
    
    ctx.beginPath();
    ctx.circle(currentX, currentY, 5);
    ctx.fillStyle(Colours::Blue);
    ctx.fill();

    // Draw center line
    ctx.beginPath();
    ctx.moveTo(lfoX, lfoY + lfoH/2);
    ctx.lineTo(lfoX + lfoW, lfoY + lfoH/2);
    ctx.strokeStyle(Colours::White.dim(0.3));
    ctx.lineWidth(1.0f);
    ctx.stroke();

    ctx.restore();
  }

} // namespace top1::modules
