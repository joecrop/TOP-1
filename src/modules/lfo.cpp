#include "lfo.hpp"
#include "core/globals.hpp"
#include <random>

namespace top1::modules {

  LFO::LFO() :
    Module(&props),
    screen(std::make_unique<LFOScreen>(this)) {
    // Initialize random values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    lastRandomValue = dis(gen);
    nextRandomValue = dis(gen);
  }

  float LFO::calculateWaveform(float phase, Waveform wave) {
    switch (wave) {
      case Waveform::Sine:
        return std::sin(phase * 2.0f * M_PI);
        
      case Waveform::Triangle:
        if (phase < 0.25f) return phase * 4.0f;
        else if (phase < 0.75f) return 1.0f - (phase - 0.25f) * 4.0f;
        else return -1.0f + (phase - 0.75f) * 4.0f;
        
      case Waveform::Square:
        return phase < 0.5f ? 1.0f : -1.0f;
        
      case Waveform::Saw:
        return 2.0f * phase - 1.0f;
        
      case Waveform::Random:
        // Smoothly interpolate between random values
        return lastRandomValue + (nextRandomValue - lastRandomValue) * phase;
        
      case Waveform::SampleHold:
        return lastRandomValue;  // Holds until next cycle
        
      default:
        return 0.0f;
    }
  }

  void LFO::process(float deltaTime) {
    float rate = props.rate.get();
    
    // If synced to tempo, convert rate to tempo-relative
    if (props.sync.get()) {
      float bpm = Globals::metronome.props.bpm.get();
      // Rate becomes beat divisions: 1 = whole note, 4 = quarter, 16 = sixteenth
      float beatsPerSecond = bpm / 60.0f;
      rate = beatsPerSecond / rate;  // Divide by rate setting for musical divisions
    }
    
    // Advance phase
    float phaseIncrement = rate * deltaTime;
    float previousPhase = currentPhase;
    currentPhase += phaseIncrement;
    
    // Handle phase wrap
    while (currentPhase >= 1.0f) {
      currentPhase -= 1.0f;
      
      // Update random values on new cycle
      if (props.waveform.get() == static_cast<int>(Waveform::Random) ||
          props.waveform.get() == static_cast<int>(Waveform::SampleHold)) {
        lastRandomValue = nextRandomValue;
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
        nextRandomValue = dis(gen);
      }
    }
    
    // Apply phase offset
    float adjustedPhase = currentPhase + props.phase.get();
    while (adjustedPhase >= 1.0f) adjustedPhase -= 1.0f;
    
    // Calculate waveform value
    Waveform wave = static_cast<Waveform>(props.waveform.get());
    currentValue = calculateWaveform(adjustedPhase, wave);
    
    // Apply depth
    currentValue *= props.depth.get();
  }

  void LFO::retrigger() {
    if (props.retrigger.get()) {
      currentPhase = 0.0f;
    }
  }

  void LFO::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* LFOScreen Implementation             */
  /****************************************/

  using namespace ui::drawing;

  void LFOScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("LFO", 160, 20);

    // Waveform names
    const char* waveNames[] = {"SINE", "TRI", "SQR", "SAW", "RND", "S&H"};
    int waveType = module->props.waveform.get();

    // Draw waveform preview
    float waveX = 60;
    float waveY = 80;
    float waveW = 200;
    float waveH = 60;

    // Background
    ctx.beginPath();
    ctx.rect({waveX, waveY - waveH/2}, {waveW, waveH});
    ctx.fillStyle(Colours::Gray70);
    ctx.fill();

    // Center line
    ctx.beginPath();
    ctx.moveTo(waveX, waveY);
    ctx.lineTo(waveX + waveW, waveY);
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(1);
    ctx.stroke();

    // Draw waveform shape
    ctx.beginPath();
    ctx.strokeStyle(Colours::White);
    ctx.lineWidth(2);
    
    for (int i = 0; i <= 100; i++) {
      float phase = (float)i / 100.0f;
      float value = module->calculateWaveform(phase, static_cast<LFO::Waveform>(waveType));
      float x = waveX + phase * waveW;
      float y = waveY - value * (waveH / 2 - 5) * module->props.depth.get();
      
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.stroke();

    // Draw current position indicator
    float posX = waveX + (module->currentPhase + module->props.phase.get()) * waveW;
    while (posX >= waveX + waveW) posX -= waveW;
    float posY = waveY - module->getValue() * (waveH / 2 - 5);
    
    ctx.beginPath();
    ctx.circle(posX, posY, 5);
    ctx.fillStyle(Colours::Green);
    ctx.fill();

    // Parameter labels
    float paramY = 150;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    
    // Rate (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("RATE", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    if (module->props.sync.get()) {
      ctx.fillText(fmt::format("1/{:.0f}", module->props.rate.get()), 50, paramY + 14);
    } else {
      ctx.fillText(fmt::format("{:.1f}Hz", module->props.rate.get()), 50, paramY + 14);
    }
    
    // Depth (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DEPTH", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}%", module->props.depth.get() * 100), 120, paramY + 14);
    
    // Waveform (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("WAVE", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(waveNames[waveType], 200, paramY + 14);
    
    // Phase (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("PHASE", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(20);
    ctx.fillText(fmt::format("{:.0f}°", module->props.phase.get() * 360), 270, paramY + 14);

    // Sync indicator
    ctx.font(Fonts::Norm);
    ctx.font(10);
    ctx.textAlign(TextAlign::Left, TextAlign::Bottom);
    if (module->props.sync.get()) {
      ctx.fillStyle(Colours::Green);
      ctx.fillText("TEMPO SYNC", 20, 195);
    }
    if (module->props.retrigger.get()) {
      ctx.fillStyle(Colours::Blue);
      ctx.fillText("RETRIG", 100, 195);
    }
  }

  bool LFOScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_BLUE_CLICK:
      module->props.sync = !module->props.sync.get();
      return true;
    case ui::K_GREEN_CLICK:
      module->props.retrigger = !module->props.retrigger.get();
      return true;
    case ui::K_WHITE_CLICK:
      // Could cycle waveforms
      return true;
    case ui::K_RED_CLICK:
      // Reset phase
      module->retrigger();
      return true;
    default:
      return false;
    }
  }

  void LFOScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.rate.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.depth.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.waveform.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.phase.step(e.clicks);
      break;
    }
  }

} // namespace top1::modules
