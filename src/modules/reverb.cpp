#include "reverb.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"

namespace top1::modules {

  /*
   * Declarations
   */

  class ReverbEffectScreen : public ui::ModuleScreen<ReverbEffect> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_room_viz(ui::drawing::Canvas& ctx);
    void draw_bypass_indicator(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<ReverbEffect>::ModuleScreen;
  };

  /*
   * ReverbEffect - Freeverb algorithm implementation
   */

  // Freeverb constants
  constexpr int numCombs = 8;
  constexpr int numAllpasses = 4;
  constexpr float muted = 0;
  constexpr float fixedGain = 0.015f;
  constexpr float scaleWet = 3.0f;
  constexpr float scaleDry = 2.0f;
  constexpr float scaleDamp = 0.4f;
  constexpr float scaleRoom = 0.28f;
  constexpr float offsetRoom = 0.7f;
  constexpr float initialRoom = 0.5f;
  constexpr float initialDamp = 0.5f;
  constexpr float initialWet = 1.0f / scaleWet;
  constexpr float initialDry = 0;
  constexpr float initialWidth = 1;
  constexpr float initialMode = 0;
  constexpr float freezeMode = 0.5f;

  // Comb filter
  class Comb {
    std::vector<float> buffer;
    int bufsize;
    int bufidx;
    float feedback;
    float filterstore;
    float damp1;
    float damp2;

  public:
    Comb() : bufsize(0), bufidx(0), feedback(0), filterstore(0), damp1(0), damp2(0) {}

    void setbuffer(int size) {
      bufsize = size;
      buffer.resize(size);
      std::fill(buffer.begin(), buffer.end(), 0);
      bufidx = 0;
    }

    void mute() {
      std::fill(buffer.begin(), buffer.end(), 0);
    }

    void setdamp(float val) {
      damp1 = val;
      damp2 = 1 - val;
    }

    void setfeedback(float val) {
      feedback = val;
    }

    float process(float input) {
      float output = buffer[bufidx];
      filterstore = (output * damp2) + (filterstore * damp1);
      buffer[bufidx] = input + (filterstore * feedback);
      if (++bufidx >= bufsize) bufidx = 0;
      return output;
    }
  };

  // Allpass filter
  class Allpass {
    std::vector<float> buffer;
    int bufsize;
    int bufidx;

  public:
    Allpass() : bufsize(0), bufidx(0) {}

    void setbuffer(int size) {
      bufsize = size;
      buffer.resize(size);
      std::fill(buffer.begin(), buffer.end(), 0);
      bufidx = 0;
    }

    void mute() {
      std::fill(buffer.begin(), buffer.end(), 0);
    }

    float process(float input) {
      float bufout = buffer[bufidx];
      float output = -input + bufout;
      buffer[bufidx] = input + (bufout * 0.5f);
      if (++bufidx >= bufsize) bufidx = 0;
      return output;
    }
  };

  // Reverb engine
  class ReverbEngine {
  public:
    float gain;
    float roomsize, roomsize1;
    float damp, damp1;
    float wet, wet1, wet2;
    float dry;
    float width;
    float mode;

    std::array<Comb, numCombs> combL;
    std::array<Comb, numCombs> combR;
    std::array<Allpass, numAllpasses> allpassL;
    std::array<Allpass, numAllpasses> allpassR;

    ReverbEngine() {
      // Tuning values (buffer sizes)
      int combtuningL[] = {1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617};
      int combtuningR[] = {1116+23, 1188+23, 1277+23, 1356+23, 1422+23, 1491+23, 1557+23, 1617+23};
      int allpasstuningL[] = {556, 441, 341, 225};
      int allpasstuningR[] = {556+23, 441+23, 341+23, 225+23};

      for (int i = 0; i < numCombs; i++) {
        combL[i].setbuffer(combtuningL[i]);
        combR[i].setbuffer(combtuningR[i]);
      }
      for (int i = 0; i < numAllpasses; i++) {
        allpassL[i].setbuffer(allpasstuningL[i]);
        allpassR[i].setbuffer(allpasstuningR[i]);
      }

      setWet(initialWet);
      setRoomSize(initialRoom);
      setDry(initialDry);
      setDamp(initialDamp);
      setWidth(initialWidth);
      setMode(initialMode);

      mute();
    }

    void mute() {
      for (auto& c : combL) c.mute();
      for (auto& c : combR) c.mute();
      for (auto& a : allpassL) a.mute();
      for (auto& a : allpassR) a.mute();
    }

    void setRoomSize(float value) {
      roomsize = (value * scaleRoom) + offsetRoom;
      roomsize1 = roomsize;
      update();
    }

    void setDamp(float value) {
      damp = value * scaleDamp;
      damp1 = damp;
      update();
    }

    void setWet(float value) {
      wet = value * scaleWet;
      update();
    }

    void setDry(float value) {
      dry = value * scaleDry;
    }

    void setWidth(float value) {
      width = value;
      update();
    }

    void setMode(float value) {
      mode = value;
      update();
    }

    void update() {
      wet1 = wet * (width / 2.0f + 0.5f);
      wet2 = wet * ((1.0f - width) / 2.0f);

      if (mode >= freezeMode) {
        roomsize1 = 1;
        damp1 = 0;
        gain = muted;
      } else {
        roomsize1 = roomsize;
        damp1 = damp;
        gain = fixedGain;
      }

      for (auto& c : combL) {
        c.setfeedback(roomsize1);
        c.setdamp(damp1);
      }
      for (auto& c : combR) {
        c.setfeedback(roomsize1);
        c.setdamp(damp1);
      }
    }

    void process(float inputL, float inputR, float& outputL, float& outputR) {
      float outL = 0, outR = 0;
      float input = (inputL + inputR) * gain;

      // Parallel comb filters
      for (int i = 0; i < numCombs; i++) {
        outL += combL[i].process(input);
        outR += combR[i].process(input);
      }

      // Series allpass filters
      for (int i = 0; i < numAllpasses; i++) {
        outL = allpassL[i].process(outL);
        outR = allpassR[i].process(outR);
      }

      outputL = outL * wet1 + outR * wet2 + inputL * dry;
      outputR = outR * wet1 + outL * wet2 + inputR * dry;
    }
  };

  ReverbEngine reverbEngineL;
  ReverbEngine reverbEngineR;

  ReverbEffect::ReverbEffect() :
    EffectModule(&props),
    screen (new ReverbEffectScreen(this)) {}

  void ReverbEffect::init() {
    reverbEngineL = ReverbEngine();
    reverbEngineR = ReverbEngine();
  }

  void ReverbEffect::display() {
    Globals::ui.display(*screen);
  }

  void ReverbEffect::process(const audio::ProcessData& data) {
    if (props.bypassed) {
      for (uint f = 0; f < data.nframes; f++) {
        data.audio.outL[f] = data.audio.proc[f];
        data.audio.outR[f] = data.audio.proc[f];
      }
      return;
    }

    // Update reverb parameters
    reverbEngineL.setRoomSize(props.size);
    reverbEngineR.setRoomSize(props.size);
    reverbEngineL.setDamp(props.damping);
    reverbEngineR.setDamp(props.damping);
    reverbEngineL.setWidth(props.width);
    reverbEngineR.setWidth(props.width);
    
    float wet = props.wetDry;
    float dry = 1.0f - wet;
    reverbEngineL.setWet(wet);
    reverbEngineR.setWet(wet);
    reverbEngineL.setDry(dry);
    reverbEngineR.setDry(dry);

    for (uint f = 0; f < data.nframes; f++) {
      float inL = data.audio.proc[f];
      float inR = data.audio.proc[f];
      float outL, outR;

      reverbEngineL.process(inL, inR, outL, outR);

      data.audio.outL[f] = outL;
      data.audio.outR[f] = outR;
    }
  }

  /*
   * ReverbEffectScreen
   */

  using namespace ui::drawing;

  bool ReverbEffectScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_RED_CLICK:
      module->props.bypassed.step();
      return true;
    default:
      return false;
    }
  }

  void ReverbEffectScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.size.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.damping.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.wetDry.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.width.step(e.clicks);
      break;
    }
  }

  void ReverbEffectScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_room_viz(ctx);
    draw_bypass_indicator(ctx);
  }

  void ReverbEffectScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18.f);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("REVERB", 160, 20);

    // Parameter labels
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("SIZE", 60, 100);
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText("DAMPING", 120, 180);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText("WET/DRY", 200, 180);
    
    ctx.fillStyle(Colours::Red);
    ctx.fillText("WIDTH", 260, 100);

    // Values
    ctx.font(Fonts::Bold);
    ctx.font(32.f);
    
    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("{:.0f}%", module->props.size.get() * 100), 60, 140);
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("{:.0f}%", module->props.damping.get() * 100), 120, 220);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("{:.0f}%", module->props.wetDry.get() * 100), 200, 220);
    
    ctx.fillStyle(Colours::Red);
    ctx.fillText(fmt::format("{:.0f}%", module->props.width.get() * 100), 260, 140);

    ctx.restore();
  }

  void ReverbEffectScreen::draw_room_viz(Canvas& ctx) {
    ctx.save();
    
    // Draw room visualization - expanding circles representing reverb
    float size = module->props.size;
    float damping = module->props.damping;
    int maxRings = 8;
    
    for (int i = 0; i < maxRings; i++) {
      float t = i / float(maxRings - 1);
      float radius = 20 + t * size * 50;
      float alpha = (1.0f - t) * (1.0f - damping * 0.7f);
      
      if (alpha < 0.05f) break;
      
      ctx.globalAlpha(alpha);
      ctx.strokeStyle(Colours::Green);
      ctx.lineWidth(2.0f);
      ctx.beginPath();
      ctx.circle(160, 60, radius);
      ctx.stroke();
    }
    
    ctx.restore();
  }

  void ReverbEffectScreen::draw_bypass_indicator(Canvas& ctx) {
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
