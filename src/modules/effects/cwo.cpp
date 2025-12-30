#include "cwo.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  CWOEffect::CWOEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_cwo>(), props),
    screen(std::make_unique<CWOScreen>(this)) {}

  void CWOEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) {
      return;
    }
    faust.process(data.audio.proc);
  }

  void CWOEffect::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* CWOScreen Implementation             */
  /****************************************/

  using namespace ui::drawing;

  void CWOScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(22);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("CWO", 160, 25);

    // Bypass indicator
    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(12);
      ctx.fillStyle(Colours::Gray60);
      ctx.fillText("[BYPASSED]", 160, 45);
    }

    // Animated wobble visualization
    float vizX = 40;
    float vizY = 60;
    float vizW = 240;
    float vizH = 60;

    // Draw tape wobble wave
    ctx.beginPath();
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(2);

    float rate = module->props.rate.get();
    float depth = module->props.depth.get();
    float flutter = module->props.flutter.get();

    phase += 0.02f * rate;
    if (phase > 2 * M_PI) phase -= 2 * M_PI;

    for (int i = 0; i <= 80; i++) {
      float t = (float)i / 80;
      float x = vizX + t * vizW;
      
      // Slow wow wave
      float wow = std::sin(phase + t * 2 * M_PI) * (1 - flutter);
      
      // Fast flutter
      float flut = (std::sin(phase * 6.3f + t * 4 * M_PI) +
                   std::sin(phase * 7.1f + t * 5 * M_PI) * 0.7f) * flutter;
      
      float wave = (wow + flut) * depth;
      float y = vizY + vizH/2 - wave * vizH * 0.4f;
      
      if (i == 0) {
        ctx.moveTo(x, y);
      } else {
        ctx.lineTo(x, y);
      }
    }
    ctx.stroke();

    // Center line
    ctx.beginPath();
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(1);
    ctx.moveTo(vizX, vizY + vizH/2);
    ctx.lineTo(vizX + vizW, vizY + vizH/2);
    ctx.stroke();

    // Tape reel indicators
    float reelY = vizY + vizH/2;
    float reelR = 15;
    
    // Left reel
    ctx.beginPath();
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(2);
    ctx.arc(vizX - 5, reelY, reelR, 0, 2 * M_PI);
    ctx.stroke();
    
    // Right reel
    ctx.beginPath();
    ctx.arc(vizX + vizW + 5, reelY, reelR, 0, 2 * M_PI);
    ctx.stroke();

    // Parameters
    float py = 145;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    char buf[32];

    // Rate (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("RATE", 50, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.1fHz", module->props.rate.get());
    ctx.fillText(buf, 50, py + 15);
    
    // Depth (Green)
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DEPTH", 120, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.depth.get() * 100);
    ctx.fillText(buf, 120, py + 15);
    
    // Flutter (White)
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("FLUTTER", 200, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.flutter.get() * 100);
    ctx.fillText(buf, 200, py + 15);
    
    // Mix (Red)
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("MIX", 270, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.mix.get() * 100);
    ctx.fillText(buf, 270, py + 15);
  }

  bool CWOScreen::keypress(ui::Key key) {
    if (key == ui::Key::K_SHIFT) {
      module->toggleBypass();
      return true;
    }
    return false;
  }

  void CWOScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
      case ui::Rotary::Blue:
        module->props.rate.step(e.clicks);
        break;
      case ui::Rotary::Green:
        module->props.depth.step(e.clicks);
        break;
      case ui::Rotary::White:
        module->props.flutter.step(e.clicks);
        break;
      case ui::Rotary::Red:
        module->props.mix.step(e.clicks);
        break;
    }
  }

} // namespace top1::modules
