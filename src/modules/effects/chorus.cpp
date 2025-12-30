#include "chorus.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  ChorusEffect::ChorusEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_chorus>(), props),
    screen(std::make_unique<ChorusScreen>(this)) {}

  void ChorusEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) return;
    faust.process(data.audio.proc);
  }

  void ChorusEffect::display() {
    Globals::ui.display(*screen);
  }

  using namespace ui::drawing;

  void ChorusScreen::draw(Canvas& ctx) {
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("CHORUS", 160, 20);

    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(14);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText("BYPASS", 300, 20);
    }

    // Draw chorus visualization - overlapping waves
    float centerY = 100;
    int voices = module->props.voices.get();
    float rate = module->props.rate.get();
    float depth = module->props.depth.get();

    for (int v = 0; v < voices; v++) {
      float phase = (float)v / voices * M_PI;
      ctx.beginPath();
      for (int x = 40; x <= 280; x++) {
        float t = (float)(x - 40) / 240 * 4 * M_PI;
        float y = centerY + std::sin(t + phase) * (10 + depth * 2);
        if (x == 40) ctx.moveTo(x, y);
        else ctx.lineTo(x, y);
      }
      Colour waveCol = v == 0 ? Colours::Blue : 
                       v == 1 ? Colours::Green :
                       v == 2 ? Colours::White : Colours::Red;
      ctx.strokeStyle(waveCol.dim(0.3f + 0.2f * v));
      ctx.lineWidth(2);
      ctx.stroke();
    }

    // Parameters
    float paramY = 160;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText("RATE", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.1f}Hz", rate), 50, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DEPTH", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.1f}ms", depth), 120, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("MIX", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.mix.get() * 100), 200, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("VOICES", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{}", voices), 270, paramY + 14);
  }

  bool ChorusScreen::keypress(ui::Key key) {
    if (key == ui::K_BLUE_CLICK || key == ui::K_GREEN_CLICK ||
        key == ui::K_WHITE_CLICK || key == ui::K_RED_CLICK) {
      module->toggleBypass();
      return true;
    }
    return false;
  }

  void ChorusScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue: module->props.rate.step(e.clicks); break;
    case ui::Rotary::Green: module->props.depth.step(e.clicks); break;
    case ui::Rotary::White: module->props.mix.step(e.clicks); break;
    case ui::Rotary::Red: module->props.voices.step(e.clicks); break;
    }
  }

} // namespace top1::modules
