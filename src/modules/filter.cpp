#include "filter.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  /*
   * Declarations
   */

  class FilterEffectScreen : public ui::ModuleScreen<FilterEffect> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_filter_response(ui::drawing::Canvas& ctx);
    void draw_bypass_indicator(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<FilterEffect>::ModuleScreen;
  };

  /*
   * FilterEffect - State Variable Filter
   */

  struct SVFilter {
    float sampleRate;
    float cutoff;
    float resonance;
    
    // State variables
    float lowpass = 0.0f;
    float bandpass = 0.0f;
    float highpass = 0.0f;
    
    // Filter coefficients
    float f, q;

    SVFilter() : sampleRate(44100), cutoff(1000), resonance(0.5) {
      updateCoefficients();
    }

    void setSampleRate(float sr) {
      sampleRate = sr;
      updateCoefficients();
    }

    void setCutoff(float freq) {
      cutoff = std::clamp(freq, 20.0f, 20000.0f);
      updateCoefficients();
    }

    void setResonance(float res) {
      resonance = std::clamp(res, 0.0f, 1.0f);
      updateCoefficients();
    }

    void updateCoefficients() {
      f = 2.0f * std::sin(M_PI * cutoff / sampleRate);
      q = 1.0f - resonance * 0.99f; // Prevent instability at high resonance
      if (q < 0.01f) q = 0.01f;
    }

    void process(float input) {
      // State variable filter topology
      lowpass = lowpass + f * bandpass;
      highpass = input - lowpass - q * bandpass;
      bandpass = f * highpass + bandpass;
      
      // Prevent denormals
      if (std::abs(lowpass) < 1e-10f) lowpass = 0.0f;
      if (std::abs(bandpass) < 1e-10f) bandpass = 0.0f;
      if (std::abs(highpass) < 1e-10f) highpass = 0.0f;
    }

    float getLowpass() const { return lowpass; }
    float getHighpass() const { return highpass; }
    float getBandpass() const { return bandpass; }
  };

  SVFilter filterL;
  SVFilter filterR;

  FilterEffect::FilterEffect() :
    EffectModule(&props),
    screen (new FilterEffectScreen(this)) {}

  void FilterEffect::init() {
    filterL.setSampleRate(Globals::samplerate);
    filterR.setSampleRate(Globals::samplerate);
  }

  void FilterEffect::display() {
    Globals::ui.display(*screen);
  }

  void FilterEffect::process(const audio::ProcessData& data) {
    if (props.bypassed) {
      for (uint f = 0; f < data.nframes; f++) {
        data.audio.outL[f] = data.audio.proc[f];
        data.audio.outR[f] = data.audio.proc[f];
      }
      return;
    }

    // Update filter parameters
    filterL.setCutoff(props.cutoff);
    filterL.setResonance(props.resonance);
    filterR.setCutoff(props.cutoff);
    filterR.setResonance(props.resonance);

    float wet = props.wetDry;
    float dry = 1.0f - wet;
    int filterType = props.type;

    for (uint f = 0; f < data.nframes; f++) {
      float inputL = data.audio.proc[f];
      float inputR = data.audio.proc[f];

      // Process filters
      filterL.process(inputL);
      filterR.process(inputR);

      // Select filter type output
      float filteredL, filteredR;
      switch (filterType) {
        case 0: // Lowpass
          filteredL = filterL.getLowpass();
          filteredR = filterR.getLowpass();
          break;
        case 1: // Highpass
          filteredL = filterL.getHighpass();
          filteredR = filterR.getHighpass();
          break;
        case 2: // Bandpass
          filteredL = filterL.getBandpass();
          filteredR = filterR.getBandpass();
          break;
        default:
          filteredL = inputL;
          filteredR = inputR;
      }

      // Mix dry/wet
      data.audio.outL[f] = dry * inputL + wet * filteredL;
      data.audio.outR[f] = dry * inputR + wet * filteredR;
    }
  }

  /*
   * FilterEffectScreen
   */

  using namespace ui::drawing;

  bool FilterEffectScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_RED_CLICK:
      module->props.bypassed.step();
      return true;
    case ui::K_WHITE_CLICK:
      module->props.type.step();
      return true;
    default:
      return false;
    }
  }

  void FilterEffectScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.cutoff.step(e.clicks * 10);
      break;
    case ui::Rotary::Green:
      module->props.resonance.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.wetDry.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.type.step(e.clicks);
      break;
    }
  }

  void FilterEffectScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_filter_response(ctx);
    draw_bypass_indicator(ctx);
  }

  void FilterEffectScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18.f);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("FILTER", 160, 20);

    // Parameter labels
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("CUTOFF", 60, 100);
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText("RESONANCE", 120, 180);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText("WET/DRY", 200, 180);
    
    ctx.fillStyle(Colours::Red);
    const char* typeNames[] = {"LOWPASS", "HIGHPASS", "BANDPASS"};
    ctx.fillText(typeNames[module->props.type.get()], 260, 90);

    // Values
    ctx.font(Fonts::Bold);
    ctx.font(32.f);
    
    ctx.fillStyle(Colours::Blue);
    float cutoff = module->props.cutoff.get();
    if (cutoff >= 1000) {
      ctx.fillText(fmt::format("{:.1f}k", cutoff / 1000.0f), 60, 140);
    } else {
      ctx.fillText(fmt::format("{:.0f}", cutoff), 60, 140);
    }
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("{:.0f}%", module->props.resonance.get() * 100), 120, 220);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("{:.0f}%", module->props.wetDry.get() * 100), 200, 220);

    ctx.restore();
  }

  void FilterEffectScreen::draw_filter_response(Canvas& ctx) {
    ctx.save();
    
    // Draw filter frequency response curve
    float cutoff = module->props.cutoff.get();
    float resonance = module->props.resonance.get();
    int filterType = module->props.type.get();
    
    float graphX = 80;
    float graphY = 50;
    float graphW = 160;
    float graphH = 40;
    
    ctx.strokeStyle(Colours::Gray60);
    ctx.lineWidth(1.0f);
    ctx.beginPath();
    ctx.rect(graphX, graphY, graphW, graphH);
    ctx.stroke();
    
    // Draw response curve
    ctx.beginPath();
    ctx.strokeStyle(Colours::Red);
    ctx.lineWidth(2.0f);
    
    int numPoints = 80;
    for (int i = 0; i < numPoints; i++) {
      float t = i / float(numPoints - 1);
      // Logarithmic frequency scale 20Hz to 20kHz
      float freq = 20.0f * std::pow(1000.0f, t);
      float freqNorm = freq / 20000.0f;
      
      // Simplified frequency response approximation
      float response = 0.5f;
      float cutoffNorm = cutoff / 20000.0f;
      
      switch (filterType) {
        case 0: // Lowpass
          if (freqNorm < cutoffNorm) {
            response = 0.9f;
          } else {
            float slope = (freqNorm - cutoffNorm) / (1.0f - cutoffNorm);
            response = 0.9f * (1.0f - slope);
          }
          // Add resonance peak
          if (std::abs(freqNorm - cutoffNorm) < 0.05f) {
            response += resonance * 0.3f;
          }
          break;
          
        case 1: // Highpass
          if (freqNorm > cutoffNorm) {
            response = 0.9f;
          } else {
            float slope = freqNorm / cutoffNorm;
            response = 0.9f * slope;
          }
          if (std::abs(freqNorm - cutoffNorm) < 0.05f) {
            response += resonance * 0.3f;
          }
          break;
          
        case 2: // Bandpass
          {
            float distance = std::abs(freqNorm - cutoffNorm);
            response = std::exp(-distance * 8.0f) * (0.5f + resonance * 0.5f);
          }
          break;
      }
      
      response = std::clamp(response, 0.0f, 1.0f);
      float x = graphX + t * graphW;
      float y = graphY + graphH - (response * graphH);
      
      if (i == 0) {
        ctx.moveTo(x, y);
      } else {
        ctx.lineTo(x, y);
      }
    }
    
    ctx.stroke();
    
    // Draw cutoff frequency indicator
    float cutoffNorm = std::log(cutoff / 20.0f) / std::log(1000.0f);
    float cutoffX = graphX + cutoffNorm * graphW;
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(2.0f);
    ctx.beginPath();
    ctx.moveTo(cutoffX, graphY);
    ctx.lineTo(cutoffX, graphY + graphH);
    ctx.stroke();
    
    ctx.restore();
  }

  void FilterEffectScreen::draw_bypass_indicator(Canvas& ctx) {
    ctx.save();
    
    // Bypass indicator
    if (module->props.bypassed) {
      ctx.font(Fonts::Bold);
      ctx.font(16.f);
      ctx.fillStyle(Colours::Red);
      ctx.textAlign(TextAlign::Right, TextAlign::Top);
      ctx.fillText("BYPASSED", 310, 10);
    }
    
    ctx.restore();
  }

} // namespace top1::modules
