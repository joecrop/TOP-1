#include "phaser.hpp"
#include "core/globals.hpp"
#include <cmath>

namespace top1::modules {

  PhaserEffect::PhaserEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_phaser>(), props),
    screen(std::make_unique<PhaserScreen>(this)) {}

  void PhaserEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) {
      return;
    }
    // Update LFO phase for animation
    float lfoIncrement = (props.rate / 48000.0f) * data.nframes;
    lfoPhase += lfoIncrement;
    if (lfoPhase > 1.0f) lfoPhase -= 1.0f;
    
    faust.process(data.audio.proc);
  }

  void PhaserEffect::display() {
    Globals::ui.display(*screen);
  }

  // === Screen Implementation ===

  using namespace ui::drawing;

  void PhaserScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;
    
    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("PHASER", 160, 20);

    // Bypass indicator
    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(14);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Middle);
      ctx.fillText("BYPASS", 300, 20);
    }
    
    // Visual: Rotating phase circles
    float centerX = 160;
    float centerY = 110;

    int numStages = module->props.stages;
    float baseRadius = 40;
    
    // Draw each allpass stage as a phase-shifted ring
    for (int s = 0; s < numStages; s++) {
      float stageOffset = (float)s / numStages;
      float phase = module->lfoPhase + stageOffset;
      
      // Create phase-shifted appearance
      float radius = baseRadius + s * 8;
      float alpha = 0.3f + 0.7f * module->props.depth;
      
      // Draw arc showing phase
      ctx.beginPath();
      ctx.strokeStyle(Colours::White.dim(alpha));
      ctx.lineWidth(3.0f);
      
      float startAngle = phase * 2 * M_PI;
      float endAngle = startAngle + M_PI * (1 + module->props.depth * 0.5f);
      
      ctx.arc(centerX, centerY, radius, startAngle, endAngle);
      ctx.stroke();
    }
    
    // Center indicator showing LFO position
    ctx.beginPath();
    float lfoVal = std::sin(module->lfoPhase * 2 * M_PI) * 0.5f + 0.5f;
    ctx.fillStyle(Colours::White.mix(Colours::Blue, lfoVal));
    ctx.circle(centerX, centerY, 15);
    ctx.fill();
    
    // Parameters
    float py = 185;
    ctx.font(Fonts::Norm);
    ctx.font(14);
    
    // Rate (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("RATE", 40, py);
    ctx.font(18);
    char buf[32];
    snprintf(buf, sizeof(buf), "%.2fHz", module->props.rate.get());
    ctx.fillText(buf, 40, py + 22);
    
    // Depth (Green)
    ctx.font(14);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DEPTH", 120, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.depth.get() * 100);
    ctx.fillText(buf, 120, py + 22);
    
    // Stages (White)
    ctx.font(14);
    ctx.fillStyle(Colours::White);
    ctx.fillText("STAGE", 200, py);
    ctx.font(18);
    ctx.fillText(std::to_string(module->props.stages.get()), 200, py + 22);
    
    // Mix (Red)
    ctx.font(14);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("MIX", 280, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.mix.get() * 100);
    ctx.fillText(buf, 280, py + 22);
  }

  bool PhaserScreen::keypress(ui::Key key) {
    if (key == ui::Key::K_SHIFT) {
      module->toggleBypass();
      return true;
    }
    return false;
  }

  void PhaserScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
      case ui::Rotary::Blue:
        module->props.rate.step(e.clicks);
        break;
      case ui::Rotary::Green:
        module->props.depth.step(e.clicks);
        break;
      case ui::Rotary::White:
        module->props.stages.step(e.clicks);
        break;
      case ui::Rotary::Red:
        module->props.mix.step(e.clicks);
        break;
    }
  }

} // namespace top1::modules
