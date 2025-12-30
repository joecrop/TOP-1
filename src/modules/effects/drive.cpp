#include "drive.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  DriveEffect::DriveEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_drive>(), props),
    screen(std::make_unique<DriveScreen>(this)) {}

  void DriveEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) return;
    faust.process(data.audio.proc);
  }

  void DriveEffect::display() {
    Globals::ui.display(*screen);
  }

  using namespace ui::drawing;

  void DriveScreen::draw(Canvas& ctx) {
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("DRIVE", 160, 20);

    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(14);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText("BYPASS", 300, 20);
    }

    // Draw distortion transfer curve
    float drive = module->props.drive.get();
    float baseX = 80, baseY = 130, size = 80;

    // Background
    ctx.beginPath();
    ctx.rect({baseX, baseY - size}, {size, size});
    ctx.fillStyle(Colours::Gray70);
    ctx.fill();

    // Axes
    ctx.beginPath();
    ctx.moveTo(baseX, baseY);
    ctx.lineTo(baseX + size, baseY);
    ctx.moveTo(baseX, baseY);
    ctx.lineTo(baseX, baseY - size);
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(1);
    ctx.stroke();

    // Transfer curve (tanh-based soft clipping)
    float gain = 1.0f + drive * 20.0f;
    ctx.beginPath();
    for (int i = 0; i <= 50; i++) {
      float input = (float)i / 50.0f * 2.0f - 1.0f;  // -1 to 1
      float output = std::tanh(input * gain);
      float x = baseX + (input + 1.0f) / 2.0f * size;
      float y = baseY - (output + 1.0f) / 2.0f * size;
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.strokeStyle(Colours::Red);
    ctx.lineWidth(2);
    ctx.stroke();

    // Linear reference line
    ctx.beginPath();
    ctx.moveTo(baseX, baseY);
    ctx.lineTo(baseX + size, baseY - size);
    ctx.strokeStyle(Colours::Gray60.dim(0.5f));
    ctx.lineWidth(1);
    ctx.stroke();

    // Draw tube glow effect
    float glowX = 220, glowY = 90, glowR = 30;
    ctx.beginPath();
    ctx.arc(glowX, glowY, glowR, 0, 2 * M_PI);
    Colour glowCol = Colours::Red.mix(Colours::White, drive * 0.3f);
    ctx.fillStyle(glowCol.dim(0.3f + drive * 0.5f));
    ctx.fill();
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(2);
    ctx.stroke();

    // Filament
    ctx.beginPath();
    ctx.moveTo(glowX - 10, glowY + 10);
    ctx.lineTo(glowX, glowY - 10);
    ctx.lineTo(glowX + 10, glowY + 10);
    ctx.strokeStyle(glowCol);
    ctx.lineWidth(2 + drive * 2);
    ctx.stroke();

    // Parameters
    float paramY = 160;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText("DRIVE", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", drive * 100), 50, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("TONE", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.tone.get() * 100), 120, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("OUTPUT", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.output.get() * 100), 200, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("MIX", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.mix.get() * 100), 270, paramY + 14);
  }

  bool DriveScreen::keypress(ui::Key key) {
    if (key == ui::K_BLUE_CLICK || key == ui::K_GREEN_CLICK ||
        key == ui::K_WHITE_CLICK || key == ui::K_RED_CLICK) {
      module->toggleBypass();
      return true;
    }
    return false;
  }

  void DriveScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue: module->props.drive.step(e.clicks); break;
    case ui::Rotary::Green: module->props.tone.step(e.clicks); break;
    case ui::Rotary::White: module->props.output.step(e.clicks); break;
    case ui::Rotary::Red: module->props.mix.step(e.clicks); break;
    }
  }

} // namespace top1::modules
