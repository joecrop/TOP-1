#include "bitcrusher.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  class BitcrusherEffectScreen : public ui::ModuleScreen<BitcrusherEffect> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_waveform(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<BitcrusherEffect>::ModuleScreen;
  };

  BitcrusherEffect::BitcrusherEffect() :
    EffectModule(&props),
    screen (new BitcrusherEffectScreen(this)) {}

  void BitcrusherEffect::display() {
    Globals::ui.display(*screen);
  }

  void BitcrusherEffect::process(const audio::ProcessData& data) {
    if (props.bypassed) return;

    float samplerate = Globals::samplerate;
    float crushRate = props.sampleRate;
    int bits = props.bitDepth;
    float levels = std::pow(2.0f, bits);
    float step = 2.0f / levels;

    for (uint i = 0; i < data.nframes; i++) {
      float input = data.audio.proc[i];
      
      // Sample rate reduction
      phaseAccumulator += crushRate / samplerate;
      if (phaseAccumulator >= 1.0f) {
        phaseAccumulator -= 1.0f;
        
        // Bit depth reduction
        float quantized = std::floor(input / step) * step;
        heldSample = quantized;
      }
      
      // Mix
      data.audio.proc[i] = input * (1.0f - props.mix) + heldSample * props.mix;
    }
  }

  using namespace ui::drawing;

  bool BitcrusherEffectScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_RED_CLICK:
      module->props.bypassed.step(1);
      return true;
    default:
      return false;
    }
  }

  void BitcrusherEffectScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.bitDepth.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.sampleRate.step(e.clicks * 100);
      break;
    case ui::Rotary::White:
      module->props.mix.step(e.clicks);
      break;
    case ui::Rotary::Red:
      break;
    }
  }

  void BitcrusherEffectScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_waveform(ctx);
  }

  void BitcrusherEffectScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("BITCRUSHER", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Bit Depth: {} bits", module->props.bitDepth.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Sample Rate: {:.0f}Hz", module->props.sampleRate.get()), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Mix: {:.0f}%", module->props.mix.get() * 100), 10, 90);

    if (module->props.bypassed) {
      ctx.fillStyle(Colours::Red);
      ctx.font(Fonts::Bold);
      ctx.font(18.f);
      ctx.textAlign(TextAlign::Right, TextAlign::Bottom);
      ctx.fillText("BYPASSED", 310, 230);
    }

    ctx.restore();
  }

  void BitcrusherEffectScreen::draw_waveform(Canvas& ctx) {
    ctx.save();

    float waveX = 180;
    float waveY = 80;
    float waveW = 120;
    float waveH = 120;

    // Draw quantization levels
    int bits = module->props.bitDepth;
    int levels = std::min(16, (int)std::pow(2, bits));
    
    ctx.strokeStyle(Colours::White.dim(0.2));
    ctx.lineWidth(1.0f);
    for (int i = 0; i <= levels; i++) {
      float y = waveY + (i * waveH / levels);
      ctx.beginPath();
      ctx.moveTo(waveX, y);
      ctx.lineTo(waveX + waveW, y);
      ctx.stroke();
    }

    // Draw crushed waveform visualization
    ctx.beginPath();
    int numSteps = 32;
    float stepW = waveW / numSteps;
    
    for (int i = 0; i < numSteps; i++) {
      float phase = (i / (float)numSteps) * 2.0f * M_PI;
      float sineVal = std::sin(phase * 2);
      
      // Quantize
      float levelHeight = 2.0f / levels;
      float quantized = std::floor(sineVal / levelHeight) * levelHeight;
      
      float x = waveX + i * stepW;
      float y = waveY + waveH/2 - quantized * waveH/2;
      
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
      ctx.lineTo(x + stepW, y);
    }
    
    ctx.strokeStyle(Colours::Green);
    ctx.lineWidth(2.0f);
    ctx.stroke();

    ctx.restore();
  }

} // namespace top1::modules
