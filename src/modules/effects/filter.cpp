#include "filter.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  FilterEffect::FilterEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_filter>(), props),
    screen(std::make_unique<FilterScreen>(this)) {}

  void FilterEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) return;
    faust.process(data.audio.proc);
  }

  void FilterEffect::display() {
    Globals::ui.display(*screen);
  }

  using namespace ui::drawing;

  void FilterScreen::draw(Canvas& ctx) {
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("FILTER", 160, 20);

    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(14);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText("BYPASS", 300, 20);
    }

    // Draw filter response curve
    float cutoff = module->props.cutoff.get();
    float reso = module->props.resonance.get();
    float baseX = 40, baseY = 130, width = 240, height = 80;

    // Background
    ctx.beginPath();
    ctx.rect({baseX, baseY - height}, {width, height});
    ctx.fillStyle(Colours::Gray70);
    ctx.fill();

    // Frequency labels
    ctx.font(Fonts::Norm);
    ctx.font(8);
    ctx.fillStyle(Colours::Gray60);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    ctx.fillText("100", baseX, baseY + 2);
    ctx.fillText("1k", baseX + width/2, baseY + 2);
    ctx.fillText("10k", baseX + width, baseY + 2);

    // Filter curve
    ctx.beginPath();
    for (int i = 0; i <= 100; i++) {
      // Log scale from 100Hz to 10000Hz
      float freq = 100.0f * std::pow(100.0f, (float)i / 100.0f);
      float x = baseX + (float)i / 100.0f * width;
      
      // Simple lowpass response approximation
      float response = 1.0f / std::sqrt(1.0f + std::pow(freq / cutoff, 4.0f));
      // Add resonance peak near cutoff
      float resoBoost = reso * 2.0f * std::exp(-std::pow((freq - cutoff) / (cutoff * 0.3f), 2.0f));
      response = std::min(1.0f + resoBoost, response + resoBoost);
      
      float y = baseY - response * (height - 10);
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(2);
    ctx.stroke();

    // Cutoff indicator line
    float cutoffX = baseX + std::log10(cutoff / 100.0f) / 2.0f * width;
    ctx.beginPath();
    ctx.moveTo(cutoffX, baseY - height);
    ctx.lineTo(cutoffX, baseY);
    ctx.strokeStyle(Colours::Green.dim(0.5f));
    ctx.lineWidth(1);
    ctx.stroke();

    // Parameters
    float paramY = 160;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText("CUTOFF", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{:.0f}Hz", cutoff), 50, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("RESO", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", reso * 100), 120, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("ENV", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    float env = module->props.envAmount.get();
    ctx.fillText(fmt::format("{:+.0f}%", env * 100), 200, paramY + 14);

    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("MIX", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.mix.get() * 100), 270, paramY + 14);
  }

  bool FilterScreen::keypress(ui::Key key) {
    if (key == ui::K_BLUE_CLICK || key == ui::K_GREEN_CLICK ||
        key == ui::K_WHITE_CLICK || key == ui::K_RED_CLICK) {
      module->toggleBypass();
      return true;
    }
    return false;
  }

  void FilterScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue: module->props.cutoff.step(e.clicks); break;
    case ui::Rotary::Green: module->props.resonance.step(e.clicks); break;
    case ui::Rotary::White: module->props.envAmount.step(e.clicks); break;
    case ui::Rotary::Red: module->props.mix.step(e.clicks); break;
    }
  }

} // namespace top1::modules
