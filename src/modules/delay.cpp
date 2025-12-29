#include "delay.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"

namespace top1::modules {

  /*
   * Declarations
   */

  class DelayEffectScreen : public ui::ModuleScreen<DelayEffect> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_feedback_viz(ui::drawing::Canvas& ctx);
    void draw_time_indicator(ui::drawing::Canvas& ctx);
    void draw_bypass_indicator(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<DelayEffect>::ModuleScreen;
  };

  /*
   * DelayEffect
   */

  // Simple circular buffer for delay
  struct DelayLine {
    std::vector<float> buffer;
    size_t writePos = 0;
    size_t size = 0;

    void resize(size_t newSize) {
      buffer.resize(newSize);
      size = newSize;
      writePos = 0;
    }

    void write(float sample) {
      if (size == 0) return;
      buffer[writePos] = sample;
      writePos = (writePos + 1) % size;
    }

    float read(size_t delaySamples) {
      if (size == 0) return 0.0f;
      delaySamples = std::min(delaySamples, size - 1);
      size_t readPos = (writePos + size - delaySamples) % size;
      return buffer[readPos];
    }
  };

  DelayLine delayLineL;
  DelayLine delayLineR;
  float lastFilteredL = 0.0f;
  float lastFilteredR = 0.0f;

  DelayEffect::DelayEffect() :
    EffectModule(&props),
    screen (new DelayEffectScreen(this)) {}

  void DelayEffect::init() {
    // Allocate 2 seconds max delay
    delayLineL.resize(Globals::samplerate * 2);
    delayLineR.resize(Globals::samplerate * 2);
    lastFilteredL = 0.0f;
    lastFilteredR = 0.0f;
  }

  void DelayEffect::display() {
    Globals::ui.display(*screen);
  }

  void DelayEffect::process(const audio::ProcessData& data) {
    if (props.bypassed) {
      // Bypass: copy input to output
      for (uint f = 0; f < data.nframes; f++) {
        data.audio.outL[f] = data.audio.proc[f];
        data.audio.outR[f] = data.audio.proc[f];
      }
      return;
    }

    float delaySec = props.time;
    if (props.tempoSync) {
      // Sync to metronome - delay time is in beats
      float bpm = Globals::metronome.props.bpm;
      delaySec = (60.0f / bpm) * props.time * 4.0f; // time property as beat fraction
    }
    
    size_t delaySamples = static_cast<size_t>(delaySec * Globals::samplerate);
    float feedback = props.feedback;
    float wet = props.wetDry;
    float dry = 1.0f - wet;
    float filterAmount = props.filter;

    for (uint f = 0; f < data.nframes; f++) {
      float inL = data.audio.proc[f];
      float inR = data.audio.proc[f];

      // Read delayed signal
      float delayedL = delayLineL.read(delaySamples);
      float delayedR = delayLineR.read(delaySamples);

      // Simple lowpass filter on feedback
      float filterCoeff = 0.3f * (1.0f - filterAmount);
      lastFilteredL = lastFilteredL * (1.0f - filterCoeff) + delayedL * filterCoeff;
      lastFilteredR = lastFilteredR * (1.0f - filterCoeff) + delayedR * filterCoeff;

      // Write input + filtered feedback to delay line
      delayLineL.write(inL + lastFilteredL * feedback);
      delayLineR.write(inR + lastFilteredR * feedback);

      // Output: dry + wet
      data.audio.outL[f] = dry * inL + wet * delayedL;
      data.audio.outR[f] = dry * inR + wet * delayedR;
    }
  }

  /*
   * DelayEffectScreen
   */

  using namespace ui::drawing;

  bool DelayEffectScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_RED_CLICK:
      module->props.bypassed.step();
      return true;
    case ui::K_WHITE_CLICK:
      module->props.tempoSync.step();
      return true;
    default:
      return false;
    }
  }

  void DelayEffectScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.time.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.feedback.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.wetDry.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.filter.step(e.clicks);
      break;
    }
  }

  void DelayEffectScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_feedback_viz(ctx);
    draw_time_indicator(ctx);
    draw_bypass_indicator(ctx);
  }

  void DelayEffectScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18.f);
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("DELAY", 160, 20);

    // Parameter labels
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("TIME", 60, 100);
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText("FEEDBACK", 120, 180);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText("WET/DRY", 200, 180);
    
    ctx.fillStyle(Colours::Red);
    ctx.fillText("FILTER", 260, 100);

    // Values
    ctx.font(Fonts::Bold);
    ctx.font(32.f);
    
    ctx.fillStyle(Colours::Blue);
    if (module->props.tempoSync) {
      ctx.fillText(fmt::format("{:.2f}♩", module->props.time.get()), 60, 140);
    } else {
      ctx.fillText(fmt::format("{:.0f}ms", module->props.time.get() * 1000), 60, 140);
    }
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("{:.0f}%", module->props.feedback.get() * 100), 120, 220);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("{:.0f}%", module->props.wetDry.get() * 100), 200, 220);
    
    ctx.fillStyle(Colours::Red);
    ctx.fillText(fmt::format("{:.0f}%", module->props.filter.get() * 100), 260, 140);

    ctx.restore();
  }

  void DelayEffectScreen::draw_feedback_viz(Canvas& ctx) {
    ctx.save();
    
    // Draw feedback loop visualization
    float feedback = module->props.feedback;
    int maxEchoes = 5;
    
    for (int i = 0; i < maxEchoes; i++) {
      float alpha = std::pow(feedback, i);
      if (alpha < 0.05f) break;
      
      float x = 160 + i * 25;
      float y = 60;
      float size = 15 - i * 2;
      
      ctx.globalAlpha(alpha);
      ctx.fillStyle(Colours::Green);
      ctx.beginPath();
      ctx.circle(x, y, size);
      ctx.fill();
    }
    
    ctx.restore();
  }

  void DelayEffectScreen::draw_time_indicator(Canvas& ctx) {
    ctx.save();
    
    // Draw time line
    float time = module->props.time;
    float maxTime = module->props.tempoSync ? 2.0f : 2000.0f;
    float normalizedTime = time / maxTime;
    
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(3.0f);
    ctx.beginPath();
    ctx.moveTo(30, 160);
    ctx.lineTo(30 + normalizedTime * 60, 160);
    ctx.stroke();
    
    ctx.restore();
  }

  void DelayEffectScreen::draw_bypass_indicator(Canvas& ctx) {
    ctx.save();
    
    // Bypass indicator in corner
    if (module->props.bypassed) {
      ctx.font(Fonts::Bold);
      ctx.font(16.f);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Top);
      ctx.fillText("BYPASSED", 310, 10);
    }
    
    // Tempo sync indicator
    if (module->props.tempoSync) {
      ctx.font(Fonts::Norm);
      ctx.font(12.f);
      ctx.fillStyle(Colours::White);
      ctx.textAlign(TextAlign::Left, TextAlign::Top);
      ctx.fillText("SYNC", 10, 10);
    }
    
    ctx.restore();
  }

} // namespace top1::modules
