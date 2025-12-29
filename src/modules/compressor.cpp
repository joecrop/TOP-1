#include "compressor.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  class CompressorEffectScreen : public ui::ModuleScreen<CompressorEffect> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_meter(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<CompressorEffect>::ModuleScreen;
  };

  CompressorEffect::CompressorEffect() :
    EffectModule(&props),
    screen (new CompressorEffectScreen(this)) {}

  void CompressorEffect::display() {
    Globals::ui.display(*screen);
  }

  float CompressorEffect::computeGain(float inputLevel) {
    // Convert to dB
    float inputDb = 20.0f * std::log10(std::max(inputLevel, 0.00001f));
    
    // Calculate gain reduction
    float gainDb = 0.0f;
    if (inputDb > props.threshold) {
      float overThreshold = inputDb - props.threshold;
      gainDb = overThreshold * (1.0f - 1.0f / props.ratio);
    }
    
    // Convert back to linear
    return std::pow(10.0f, -gainDb / 20.0f);
  }

  void CompressorEffect::process(const audio::ProcessData& data) {
    if (props.bypassed) return;

    float samplerate = Globals::samplerate;
    float attackCoeff = std::exp(-1.0f / (props.attack * samplerate));
    float releaseCoeff = std::exp(-1.0f / (props.release * samplerate));
    float makeupGainLinear = std::pow(10.0f, props.makeupGain / 20.0f);

    for (uint i = 0; i < data.nframes; i++) {
      float inputLevel = std::abs(data.audio.proc[i]);
      
      // Envelope follower
      if (inputLevel > envelopeFollower) {
        envelopeFollower = attackCoeff * envelopeFollower + (1.0f - attackCoeff) * inputLevel;
      } else {
        envelopeFollower = releaseCoeff * envelopeFollower + (1.0f - releaseCoeff) * inputLevel;
      }
      
      // Compute gain reduction
      float targetGain = computeGain(envelopeFollower);
      gainReduction = targetGain;
      
      // Apply compression and makeup gain
      data.audio.proc[i] *= targetGain * makeupGainLinear;
    }
  }

  using namespace ui::drawing;

  bool CompressorEffectScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_RED_CLICK:
      module->props.bypassed.step(1);
      return true;
    default:
      return false;
    }
  }

  void CompressorEffectScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.threshold.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.ratio.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.attack.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.makeupGain.step(e.clicks);
      break;
    }
  }

  void CompressorEffectScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_meter(ctx);
  }

  void CompressorEffectScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("COMPRESSOR", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Threshold: {:.1f}dB", module->props.threshold.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Ratio: {:.1f}:1", module->props.ratio.get()), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Attack: {:.3f}s", module->props.attack.get()), 10, 90);
    ctx.fillText(fmt::format("Release: {:.2f}s", module->props.release.get()), 10, 110);
    ctx.fillText(fmt::format("Makeup: {:.1f}dB", module->props.makeupGain.get()), 10, 130);

    if (module->props.bypassed) {
      ctx.fillStyle(Colours::Red);
      ctx.font(Fonts::Bold);
      ctx.font(18.f);
      ctx.textAlign(TextAlign::Right, TextAlign::Bottom);
      ctx.fillText("BYPASSED", 310, 230);
    }

    ctx.restore();
  }

  void CompressorEffectScreen::draw_meter(Canvas& ctx) {
    ctx.save();

    float meterX = 200;
    float meterY = 60;
    float meterW = 100;
    float meterH = 150;

    // Draw transfer curve
    ctx.beginPath();
    for (int i = 0; i <= 100; i++) {
      float inputDb = -60.0f + (i / 100.0f) * 60.0f;
      float outputDb = inputDb;
      
      if (inputDb > module->props.threshold) {
        float overThreshold = inputDb - module->props.threshold;
        outputDb = module->props.threshold + overThreshold / module->props.ratio;
      }
      
      float x = meterX + ((inputDb + 60.0f) / 60.0f) * meterW;
      float y = meterY + meterH - ((outputDb + 60.0f) / 60.0f) * meterH;
      
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.strokeStyle(Colours::Green);
    ctx.lineWidth(2.0f);
    ctx.stroke();

    // Draw reference line
    ctx.beginPath();
    ctx.moveTo(meterX, meterY + meterH);
    ctx.lineTo(meterX + meterW, meterY);
    ctx.strokeStyle(Colours::White.dim(0.3));
    ctx.lineWidth(1.0f);
    ctx.stroke();

    // Draw gain reduction indicator
    float grHeight = (1.0f - module->gainReduction) * meterH;
    ctx.fillStyle(Colours::Red.dim(0.5));
    ctx.beginPath();
    ctx.rect(meterX + meterW + 10, meterY + meterH - grHeight, 10, grHeight);
    ctx.fill();

    ctx.restore();
  }

} // namespace top1::modules
