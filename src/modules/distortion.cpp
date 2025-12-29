#include "distortion.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  class DistortionEffectScreen : public ui::ModuleScreen<DistortionEffect> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_transfer_curve(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<DistortionEffect>::ModuleScreen;
  };

  DistortionEffect::DistortionEffect() :
    EffectModule(&props),
    screen (new DistortionEffectScreen(this)) {}

  void DistortionEffect::display() {
    Globals::ui.display(*screen);
  }

  float DistortionEffect::applyDistortion(float input, int type) {
    float driven = input * props.drive;
    
    switch (type) {
    case 1: // Soft clip
      if (driven > 1.0f) return 2.0f/3.0f;
      else if (driven < -1.0f) return -2.0f/3.0f;
      else return driven - (driven * driven * driven) / 3.0f;
      
    case 2: // Hard clip
      return std::clamp(driven, -1.0f, 1.0f);
      
    case 3: // Fuzz
      {
        float sign = (driven >= 0) ? 1.0f : -1.0f;
        return sign * (1.0f - std::exp(-std::abs(driven)));
      }
      
    case 4: // Fold
      {
        float folded = driven;
        while (folded > 1.0f) folded = 2.0f - folded;
        while (folded < -1.0f) folded = -2.0f - folded;
        return folded;
      }
      
    default:
      return driven;
    }
  }

  void DistortionEffect::process(const audio::ProcessData& data) {
    if (props.bypassed) return;

    float toneCoeff = props.tone;

    for (uint i = 0; i < data.nframes; i++) {
      float input = data.audio.proc[i];
      
      // Apply distortion
      float distorted = applyDistortion(input, props.type);
      
      // Tone filter (simple lowpass)
      toneFilterState = toneFilterState * toneCoeff + distorted * (1.0f - toneCoeff);
      
      // Mix and normalize
      float output = input * (1.0f - props.mix) + toneFilterState * props.mix;
      data.audio.proc[i] = std::clamp(output / props.drive, -1.0f, 1.0f);
    }
  }

  using namespace ui::drawing;

  bool DistortionEffectScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_RED_CLICK:
      module->props.bypassed.step(1);
      return true;
    default:
      return false;
    }
  }

  void DistortionEffectScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.drive.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.type.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.tone.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.mix.step(e.clicks);
      break;
    }
  }

  void DistortionEffectScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_transfer_curve(ctx);
  }

  void DistortionEffectScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("DISTORTION", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Drive: {:.1f}", module->props.drive.get()), 10, 50);

    const char* typeNames[] = {"Soft", "Hard", "Fuzz", "Fold"};
    int typeIdx = std::clamp(module->props.type.get() - 1, 0, 3);
    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Type: {}", typeNames[typeIdx]), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Tone: {:.0f}%", module->props.tone.get() * 100), 10, 90);
    ctx.fillText(fmt::format("Mix: {:.0f}%", module->props.mix.get() * 100), 10, 110);

    if (module->props.bypassed) {
      ctx.fillStyle(Colours::Red);
      ctx.font(Fonts::Bold);
      ctx.font(18.f);
      ctx.textAlign(TextAlign::Right, TextAlign::Bottom);
      ctx.fillText("BYPASSED", 310, 230);
    }

    ctx.restore();
  }

  void DistortionEffectScreen::draw_transfer_curve(Canvas& ctx) {
    ctx.save();

    float curveX = 180;
    float curveY = 80;
    float curveW = 120;
    float curveH = 120;

    // Draw axes
    ctx.beginPath();
    ctx.moveTo(curveX, curveY + curveH/2);
    ctx.lineTo(curveX + curveW, curveY + curveH/2);
    ctx.moveTo(curveX + curveW/2, curveY);
    ctx.lineTo(curveX + curveW/2, curveY + curveH);
    ctx.strokeStyle(Colours::White.dim(0.3));
    ctx.lineWidth(1.0f);
    ctx.stroke();

    // Draw transfer curve
    ctx.beginPath();
    for (int i = 0; i <= 100; i++) {
      float input = -2.0f + (i / 100.0f) * 4.0f;
      float output = module->applyDistortion(input, module->props.type);
      
      float x = curveX + ((input + 2.0f) / 4.0f) * curveW;
      float y = curveY + curveH/2 - (output * curveH / 4.0f);
      
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.strokeStyle(Colours::Green);
    ctx.lineWidth(2.0f);
    ctx.stroke();

    ctx.restore();
  }

} // namespace top1::modules
