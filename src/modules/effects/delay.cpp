#include "delay.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  DelayEffect::DelayEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_delay>(), props),
    screen(std::make_unique<DelayScreen>(this)) {}

  void DelayEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) {
      return;  // Bypass - audio passes through unchanged
    }
    faust.process(data.audio.proc);
  }

  void DelayEffect::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* DelayScreen Implementation           */
  /****************************************/

  using namespace ui::drawing;

  void DelayScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("DELAY", 160, 20);

    // Bypass indicator
    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(14);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText("BYPASS", 300, 20);
    }

    // Draw delay visualization - a series of fading echoes
    float time = module->props.time.get();
    float feedback = module->props.feedback.get();
    float mix = module->props.mix.get();
    
    // Draw echo bars
    float baseX = 40;
    float baseY = 180;
    float maxWidth = 240;
    float barHeight = 80;
    
    // Original signal bar
    ctx.beginPath();
    ctx.rect({baseX, baseY - barHeight}, {20, barHeight});
    ctx.fillStyle(Colours::White);
    ctx.fill();
    
    // Echo bars with decreasing amplitude
    float amplitude = mix;
    float echoSpacing = time * 100; // Scale time to pixels
    for (int i = 1; i <= 5 && amplitude > 0.05; i++) {
      float x = baseX + i * echoSpacing;
      if (x > baseX + maxWidth) break;
      
      float height = barHeight * amplitude;
      ctx.beginPath();
      ctx.rect({x, baseY - height}, {15, height});
      
      // Color based on feedback amount - blue to green gradient
      Colour echoColor = Colours::Blue.mix(Colours::Green, (float)i / 5.0f);
      ctx.fillStyle(echoColor.dim(1.0f - amplitude * 0.5f));
      ctx.fill();
      
      amplitude *= feedback;
    }
    
    // Draw baseline
    ctx.beginPath();
    ctx.moveTo(baseX - 10, baseY);
    ctx.lineTo(baseX + maxWidth + 30, baseY);
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(2);
    ctx.stroke();

    // Parameter labels with values
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    
    // Time (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("TIME", 50, 200);
    ctx.font(Fonts::Bold);
    ctx.font(24);
    ctx.fillText(fmt::format("{:.2f}s", time), 50, 215);
    
    // Feedback (Green)
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("FEEDBACK", 130, 200);
    ctx.font(Fonts::Bold);
    ctx.font(24);
    ctx.fillText(fmt::format("{:.0f}%", feedback * 100), 130, 215);
    
    // Mix (White)
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.fillStyle(Colours::White);
    ctx.fillText("MIX", 210, 200);
    ctx.font(Fonts::Bold);
    ctx.font(24);
    ctx.fillText(fmt::format("{:.0f}%", mix * 100), 210, 215);
    
    // Tone (Red)
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("TONE", 280, 200);
    ctx.font(Fonts::Bold);
    ctx.font(24);
    ctx.fillText(fmt::format("{:.0f}%", module->props.tone.get() * 100), 280, 215);
  }

  bool DelayScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_BLUE_CLICK:
    case ui::K_GREEN_CLICK:
    case ui::K_WHITE_CLICK:
    case ui::K_RED_CLICK:
      module->toggleBypass();
      return true;
    default:
      return false;
    }
  }

  void DelayScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.time.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.feedback.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.mix.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.tone.step(e.clicks);
      break;
    }
  }

} // namespace top1::modules
