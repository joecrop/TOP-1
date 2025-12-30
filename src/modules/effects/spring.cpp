#include "spring.hpp"
#include "core/globals.hpp"

namespace top1::modules {

  SpringEffect::SpringEffect() :
    EffectModule(&props),
    faust(std::make_unique<faust_spring>(), props),
    screen(std::make_unique<SpringScreen>(this)) {}

  void SpringEffect::process(const audio::ProcessData& data) {
    if (props.bypassed.get()) {
      return;
    }
    faust.process(data.audio.proc);
  }

  void SpringEffect::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* SpringScreen Implementation          */
  /****************************************/

  using namespace ui::drawing;

  void SpringScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(22);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("SPRING", 160, 25);

    // Bypass indicator
    if (module->isBypassed()) {
      ctx.font(Fonts::Norm);
      ctx.font(12);
      ctx.fillStyle(Colours::Gray60);
      ctx.fillText("[BYPASSED]", 160, 45);
    }

    // Spring visualization
    float springX = 80;
    float springY = 60;
    float springW = 160;
    float baseH = 30;
    
    // Animate spring bob based on tension
    float tension = module->props.tension.get();
    float decay = module->props.decay.get();
    
    // Simple physics simulation
    float target = 0;
    float stiffness = 0.1f + tension * 0.3f;
    float damping = 0.92f - decay * 0.1f;
    
    springVel += (target - springBob) * stiffness;
    springVel *= damping;
    springBob += springVel;
    
    // Add some random excitation
    if (std::rand() % 20 == 0) {
      springVel += (std::rand() % 100 - 50) / 100.0f * 2.0f;
    }

    // Draw spring coils
    int numCoils = 12;
    float coilWidth = springW / numCoils;
    
    ctx.beginPath();
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(2);
    
    // Left anchor
    ctx.moveTo(springX, springY + baseH);
    
    for (int i = 0; i < numCoils; i++) {
      float x = springX + (i + 0.5f) * coilWidth;
      float phase = i * 0.8f + springBob;
      float y = springY + baseH + sin(phase) * (5 + tension * 10);
      
      if (i == 0) {
        ctx.lineTo(x, y);
      } else {
        // Draw coil with control points
        float prevX = springX + (i - 0.5f) * coilWidth;
        float cpY = (i % 2 == 0) ? -15 - tension * 10 : 15 + tension * 10;
        ctx.quadraticCurveTo(
          (prevX + x) / 2, springY + baseH + cpY + sin(phase) * 3,
          x, y
        );
      }
    }
    
    // Right anchor
    ctx.lineTo(springX + springW, springY + baseH);
    ctx.stroke();

    // Draw anchors
    ctx.beginPath();
    ctx.fillStyle(Colours::Gray60);
    ctx.rect(springX - 10, springY + baseH - 10, 10, 20);
    ctx.rect(springX + springW, springY + baseH - 10, 10, 20);
    ctx.fill();

    // Reverb tail visualization
    float tailX = springX;
    float tailY = springY + 70;
    float tailW = springW;
    float tailH = 30;

    ctx.beginPath();
    ctx.fillStyle(Colours::Green.dim(0.3f));
    
    ctx.moveTo(tailX, tailY + tailH);
    for (int i = 0; i <= 50; i++) {
      float t = i / 50.0f;
      float x = tailX + t * tailW;
      
      // Exponential decay envelope
      float env = std::exp(-t * (3 - decay * 2.5f)) * (1 - t * 0.2f);
      // Add some oscillation based on tone
      float osc = sin(t * 20 + springBob * 0.5f) * (0.1f + module->props.tone.get() * 0.2f);
      float y = tailY + tailH - (env + env * osc) * tailH;
      
      ctx.lineTo(x, y);
    }
    ctx.lineTo(tailX + tailW, tailY + tailH);
    ctx.closePath();
    ctx.fill();

    // Parameters
    float py = 160;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    char buf[32];

    // Tension (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("TENSION", 50, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.tension.get() * 100);
    ctx.fillText(buf, 50, py + 15);
    
    // Mix (Green)
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("MIX", 120, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.mix.get() * 100);
    ctx.fillText(buf, 120, py + 15);
    
    // Decay (White)
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("DECAY", 200, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.1fs", module->props.decay.get() * 2);
    ctx.fillText(buf, 200, py + 15);
    
    // Tone (Red)
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("TONE", 270, py);
    ctx.font(18);
    snprintf(buf, sizeof(buf), "%.0f%%", module->props.tone.get() * 100);
    ctx.fillText(buf, 270, py + 15);
  }

  bool SpringScreen::keypress(ui::Key key) {
    if (key == ui::Key::K_SHIFT) {
      module->toggleBypass();
      return true;
    }
    return false;
  }

  void SpringScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
      case ui::Rotary::Blue:
        module->props.tension.step(e.clicks);
        // Trigger spring animation
        springVel += e.clicks * 0.5f;
        break;
      case ui::Rotary::Green:
        module->props.mix.step(e.clicks);
        break;
      case ui::Rotary::White:
        module->props.decay.step(e.clicks);
        break;
      case ui::Rotary::Red:
        module->props.tone.step(e.clicks);
        break;
    }
  }

} // namespace top1::modules
