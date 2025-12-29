#include "chorus.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  /*
   * Declarations
   */

  class ChorusEffectScreen : public ui::ModuleScreen<ChorusEffect> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_chorus_viz(ui::drawing::Canvas& ctx);
    void draw_bypass_indicator(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<ChorusEffect>::ModuleScreen;
  };

  /*
   * ChorusEffect - Modulated delay line
   */

  struct ChorusLine {
    std::vector<float> buffer;
    size_t writePos = 0;
    size_t maxDelay = 0;
    float phase = 0.0f;
    float lastOutput = 0.0f;

    void init(size_t maxDelaySamples) {
      maxDelay = maxDelaySamples;
      buffer.resize(maxDelay);
      std::fill(buffer.begin(), buffer.end(), 0.0f);
      writePos = 0;
      phase = 0.0f;
    }

    void write(float sample) {
      buffer[writePos] = sample;
      writePos = (writePos + 1) % maxDelay;
    }

    float read(float delaySamples) {
      if (maxDelay == 0) return 0.0f;
      
      // Linear interpolation for fractional delay
      float readPosFloat = writePos - delaySamples;
      if (readPosFloat < 0) readPosFloat += maxDelay;
      
      size_t readPos1 = static_cast<size_t>(readPosFloat) % maxDelay;
      size_t readPos2 = (readPos1 + 1) % maxDelay;
      float frac = readPosFloat - std::floor(readPosFloat);
      
      return buffer[readPos1] * (1.0f - frac) + buffer[readPos2] * frac;
    }
  };

  ChorusLine chorusL;
  ChorusLine chorusR;
  float lfoPhase = 0.0f;

  ChorusEffect::ChorusEffect() :
    EffectModule(&props),
    screen (new ChorusEffectScreen(this)) {}

  void ChorusEffect::init() {
    // Max 50ms delay
    size_t maxDelay = static_cast<size_t>(0.05f * Globals::samplerate);
    chorusL.init(maxDelay);
    chorusR.init(maxDelay);
    lfoPhase = 0.0f;
  }

  void ChorusEffect::display() {
    Globals::ui.display(*screen);
  }

  void ChorusEffect::process(const audio::ProcessData& data) {
    if (props.bypassed) {
      for (uint f = 0; f < data.nframes; f++) {
        data.audio.outL[f] = data.audio.proc[f];
        data.audio.outR[f] = data.audio.proc[f];
      }
      return;
    }

    float rate = props.rate;
    float depth = props.depth;
    float wet = props.wetDry;
    float dry = 1.0f - wet;
    float feedback = props.feedback;
    
    // Base delay time: 7-15ms range
    float baseDelayMs = 10.0f;
    float baseDelaySamples = (baseDelayMs / 1000.0f) * Globals::samplerate;
    
    // Modulation depth in milliseconds
    float modulationMs = 5.0f * depth;
    float modulationSamples = (modulationMs / 1000.0f) * Globals::samplerate;
    
    // LFO rate in Hz to phase increment per sample
    float phaseIncrement = (2.0f * M_PI * rate) / Globals::samplerate;

    for (uint f = 0; f < data.nframes; f++) {
      float input = data.audio.proc[f];
      
      // Generate LFO (slightly different phase for L/R for stereo width)
      float lfoL = std::sin(lfoPhase);
      float lfoR = std::sin(lfoPhase + M_PI * 0.25f); // 45° offset
      
      // Calculate modulated delay times
      float delayL = baseDelaySamples + lfoL * modulationSamples;
      float delayR = baseDelaySamples + lfoR * modulationSamples;
      
      // Read from delay lines
      float delayedL = chorusL.read(delayL);
      float delayedR = chorusR.read(delayR);
      
      // Write to delay lines with feedback
      chorusL.write(input + delayedL * feedback);
      chorusR.write(input + delayedR * feedback);
      
      // Mix dry and wet
      data.audio.outL[f] = dry * input + wet * delayedL;
      data.audio.outR[f] = dry * input + wet * delayedR;
      
      // Advance LFO phase
      lfoPhase += phaseIncrement;
      if (lfoPhase > 2.0f * M_PI) {
        lfoPhase -= 2.0f * M_PI;
      }
    }
  }

  /*
   * ChorusEffectScreen
   */

  using namespace ui::drawing;

  bool ChorusEffectScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_RED_CLICK:
      module->props.bypassed.step();
      return true;
    default:
      return false;
    }
  }

  void ChorusEffectScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.rate.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.depth.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.wetDry.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.feedback.step(e.clicks);
      break;
    }
  }

  void ChorusEffectScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_chorus_viz(ctx);
    draw_bypass_indicator(ctx);
  }

  void ChorusEffectScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18.f);
    ctx.fillStyle(Colours::White);
    ctx.fillText("CHORUS", 160, 20);

    // Parameter labels
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("RATE", 60, 100);
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DEPTH", 120, 180);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText("WET/DRY", 200, 180);
    
    ctx.fillStyle(Colours::Red);
    ctx.fillText("FEEDBACK", 260, 100);

    // Values
    ctx.font(Fonts::Bold);
    ctx.font(32.f);
    
    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("{:.1f}Hz", module->props.rate.get()), 60, 140);
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("{:.0f}%", module->props.depth.get() * 100), 120, 220);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("{:.0f}%", module->props.wetDry.get() * 100), 200, 220);
    
    ctx.fillStyle(Colours::Red);
    int fbPercent = static_cast<int>(module->props.feedback.get() * 100);
    ctx.fillText(fmt::format("{:+.0f}%", fbPercent), 260, 140);

    ctx.restore();
  }

  void ChorusEffectScreen::draw_chorus_viz(Canvas& ctx) {
    ctx.save();
    
    // Draw rotating 3D visualization inspired by reference image
    float rate = module->props.rate;
    float depth = module->props.depth;
    
    // Animate rotation based on LFO phase
    static float vizPhase = 0.0f;
    vizPhase += 0.02f * rate;
    if (vizPhase > 2.0f * M_PI) vizPhase -= 2.0f * M_PI;
    
    float centerX = 160;
    float centerY = 60;
    
    // Draw two 3D rotating shapes (representing L/R channels)
    for (int side = 0; side < 2; side++) {
      float xOffset = (side == 0) ? -35 : 35;
      float phaseOffset = (side == 0) ? 0 : M_PI * 0.25f;
      
      ctx.save();
      ctx.translate(centerX + xOffset, centerY);
      
      // Outer ring
      for (int i = 0; i < 3; i++) {
        float ringPhase = vizPhase + phaseOffset + i * (2.0f * M_PI / 3.0f);
        float scale = 0.5f + 0.5f * std::sin(ringPhase) * depth;
        float alpha = 0.3f + 0.4f * scale;
        
        ctx.globalAlpha(alpha);
        ctx.strokeStyle((side == 0) ? Colours::Blue : Colours::Green);
        ctx.lineWidth(3.0f);
        ctx.beginPath();
        
        // Draw ellipse for 3D effect
        float rx = 25 * scale;
        float ry = 15;
        ctx.ellipse(0, 0, rx, ry);
        ctx.stroke();
      }
      
      ctx.restore();
    }
    
    ctx.restore();
  }

  void ChorusEffectScreen::draw_bypass_indicator(Canvas& ctx) {
    ctx.save();
    
    // Bypass indicator
    if (module->props.bypassed) {
      ctx.font(Fonts::Bold);
      ctx.font(16.f);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Top);
      ctx.fillText("BYPASSED", 310, 10);
    }
    
    ctx.restore();
  }

} // namespace top1::modules
