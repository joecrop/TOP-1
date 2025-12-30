#include "nitro.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  NitroEffect::NitroEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_nitro>(), props),
    screen(std::make_unique<NitroScreen>(this)) {}

  void NitroEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) {
      return;
    }
    faust.process(data.audio.proc);
    
    // Calculate simple gain reduction visualization
    float peak = 0;
    for (uint i = 0; i < data.nframes; i++) {
      float s = std::abs(data.audio.proc[i]);
      if (s > peak) peak = s;
    }
    gainReduction = peak * props.ratio.get() * 0.1f;
  }

  void NitroEffect::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* NitroScreen Implementation           */
  /****************************************/

  using namespace ui::drawing;

  void NitroScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(22);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("NITRO", 160, 25);

    // Bypass indicator
    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(12);
      ctx.fillStyle(Colours::Gray60);
      ctx.fillText("[BYPASSED]", 160, 45);
    }

    // Compression curve visualization
    float vizX = 60;
    float vizY = 55;
    float vizW = 100;
    float vizH = 80;

    // Grid
    ctx.beginPath();
    ctx.strokeStyle(Colours::Gray70);
    ctx.lineWidth(1);
    ctx.rect(vizX, vizY, vizW, vizH);
    ctx.stroke();

    // 1:1 reference line
    ctx.beginPath();
    ctx.strokeStyle(Colours::Gray60);
    ctx.moveTo(vizX, vizY + vizH);
    ctx.lineTo(vizX + vizW, vizY);
    ctx.stroke();

    // Compression curve
    ctx.beginPath();
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(2);

    float ratio = module->props.ratio.get();
    float threshold = -20 + module->props.drive.get() * 10;
    float thresholdNorm = (threshold + 40) / 40;  // Normalize to 0-1

    for (int i = 0; i <= 40; i++) {
      float inputDb = -40 + i;  // -40 to 0 dB
      float inputNorm = (inputDb + 40) / 40;
      
      float outputDb;
      if (inputDb < threshold) {
        outputDb = inputDb;
      } else {
        outputDb = threshold + (inputDb - threshold) / ratio;
      }
      float outputNorm = (outputDb + 40) / 40;
      
      float x = vizX + inputNorm * vizW;
      float y = vizY + vizH - outputNorm * vizH;
      
      if (i == 0) {
        ctx.moveTo(x, y);
      } else {
        ctx.lineTo(x, y);
      }
    }
    ctx.stroke();

    // Threshold marker
    float threshX = vizX + thresholdNorm * vizW;
    ctx.beginPath();
    ctx.strokeStyle(Colours::Red);
    ctx.lineWidth(1);
    ctx.moveTo(threshX, vizY);
    ctx.lineTo(threshX, vizY + vizH);
    ctx.stroke();

    // Gain reduction meter
    float meterX = vizX + vizW + 30;
    float meterW = 20;
    float meterH = vizH;

    ctx.beginPath();
    ctx.strokeStyle(Colours::Gray60);
    ctx.rect(meterX, vizY, meterW, meterH);
    ctx.stroke();

    // GR level
    float gr = std::min(1.0f, module->gainReduction);
    ctx.beginPath();
    ctx.fillStyle(gr > 0.7f ? Colours::Red : Colours::Green);
    ctx.rect(meterX + 1, vizY + 1, meterW - 2, gr * (meterH - 2));
    ctx.fill();

    ctx.font(10);
    ctx.fillStyle(Colours::Gray60);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    ctx.fillText("GR", meterX + meterW/2, vizY + meterH + 5);

    // Parameters
    float py = 150;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    char buf[32];

    // Drive (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("DRIVE", 50, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.drive.get() * 100);
    ctx.fillText(buf, 50, py + 15);
    
    // Ratio (Green)
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("RATIO", 120, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.1f:1", module->props.ratio.get());
    ctx.fillText(buf, 120, py + 15);
    
    // Attack (White)
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("ATTACK", 200, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0fms", module->props.attack.get() * 1000);
    ctx.fillText(buf, 200, py + 15);
    
    // Release (Red)
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("RELEASE", 270, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0fms", module->props.release.get() * 1000);
    ctx.fillText(buf, 270, py + 15);
  }

  bool NitroScreen::keypress(ui::Key key) {
    if (key == ui::Key::K_SHIFT) {
      module->toggleBypass();
      return true;
    }
    return false;
  }

  void NitroScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
      case ui::Rotary::Blue:
        module->props.drive.step(e.clicks);
        break;
      case ui::Rotary::Green:
        module->props.ratio.step(e.clicks);
        break;
      case ui::Rotary::White:
        module->props.attack.step(e.clicks);
        break;
      case ui::Rotary::Red:
        module->props.release.step(e.clicks);
        break;
    }
  }

} // namespace top1::modules
