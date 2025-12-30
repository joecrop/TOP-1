#include "reverb.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  ReverbEffect::ReverbEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_reverb>(), props),
    screen(std::make_unique<ReverbScreen>(this)) {}

  void ReverbEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) {
      return;  // Bypass - audio passes through unchanged
    }
    faust.process(data.audio.proc);
  }

  void ReverbEffect::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* ReverbScreen Implementation          */
  /****************************************/

  using namespace ui::drawing;

  void ReverbScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("REVERB", 160, 20);

    // Bypass indicator
    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(14);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText("BYPASS", 300, 20);
    }

    // Draw reverb visualization - spring/chamber representation
    float size = module->props.size.get();
    float damp = module->props.damp.get();
    float decay = module->props.decay.get();
    
    // Draw spring coils visualization
    float centerX = 160;
    float centerY = 110;
    float maxRadius = 60;
    
    // Draw concentric circles representing reverb diffusion
    int numRings = 6;
    for (int i = 1; i <= numRings; i++) {
      float radius = (maxRadius * size) * ((float)i / numRings);
      float alpha = (1.0f - damp) * (1.0f - (float)i / (numRings + 1));
      alpha *= decay;
      
      ctx.beginPath();
      ctx.arc(centerX, centerY, radius, 0, 2 * M_PI);
      
      // Color gradient from Blue (center) to Green (outer)
      Colour ringColor = Colours::Blue.mix(Colours::Green, (float)i / numRings);
      ctx.strokeStyle(ringColor.dim(1.0f - alpha * 0.5f));
      ctx.lineWidth(2 + (numRings - i) * 0.5);
      ctx.stroke();
    }
    
    // Draw input signal dot
    ctx.beginPath();
    ctx.arc(centerX, centerY, 4, 0, 2 * M_PI);
    ctx.fillStyle(Colours::White);
    ctx.fill();
    
    // Draw spring lines connecting rings
    int numSpokes = 8;
    for (int i = 0; i < numSpokes; i++) {
      float angle = (2 * M_PI / numSpokes) * i;
      float innerX = centerX + std::cos(angle) * 10;
      float innerY = centerY + std::sin(angle) * 10;
      float outerX = centerX + std::cos(angle) * (maxRadius * size);
      float outerY = centerY + std::sin(angle) * (maxRadius * size);
      
      ctx.beginPath();
      ctx.moveTo(innerX, innerY);
      ctx.lineTo(outerX, outerY);
      ctx.strokeStyle(Colours::Gray60.dim(0.5f + decay * 0.5f));
      ctx.lineWidth(1);
      ctx.stroke();
    }

    // Parameter labels with values
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    
    // Size (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("SIZE", 50, 200);
    ctx.font(Fonts::Bold);
    ctx.font(24);
    ctx.fillText(fmt::format("{:.0f}%", size * 100), 50, 215);
    
    // Damp (Green)
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DAMP", 130, 200);
    ctx.font(Fonts::Bold);
    ctx.font(24);
    ctx.fillText(fmt::format("{:.0f}%", damp * 100), 130, 215);
    
    // Mix (White)
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.fillStyle(Colours::White);
    ctx.fillText("MIX", 210, 200);
    ctx.font(Fonts::Bold);
    ctx.font(24);
    ctx.fillText(fmt::format("{:.0f}%", module->props.mix.get() * 100), 210, 215);
    
    // Decay (Red)
    ctx.font(Fonts::Norm);
    ctx.font(14);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("DECAY", 280, 200);
    ctx.font(Fonts::Bold);
    ctx.font(24);
    ctx.fillText(fmt::format("{:.0f}%", decay * 100), 280, 215);
  }

  bool ReverbScreen::keypress(ui::Key key) {
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

  void ReverbScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.size.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.damp.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.mix.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.decay.step(e.clicks);
      break;
    }
  }

} // namespace top1::modules
