#include "fm-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  /*
   * Declarations
   */

  class FMSynthScreen : public ui::ModuleScreen<FMSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_algorithm(ui::drawing::Canvas& ctx);
    void draw_envelopes(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<FMSynth>::ModuleScreen;
  };

  /*
   * FMSynth - 4-operator FM synthesizer
   */

  FMSynth::FMSynth() :
    SynthModule(&props),
    screen (new FMSynthScreen(this)) {}

  void FMSynth::display() {
    Globals::ui.display(*screen);
  }

  void FMSynth::triggerVoice(int note, float velocity) {
    // Find free voice or steal oldest
    Voice* voice = nullptr;
    for (auto& v : voices) {
      if (!v.active) {
        voice = &v;
        break;
      }
    }
    if (!voice) {
      voice = &voices[0]; // Steal first voice
    }
    
    voice->active = true;
    voice->note = note;
    voice->velocity = velocity;
    voice->baseFreq = midi::freqTable[note];
    voice->envStage = Voice::Attack;
    voice->envValue = 0;
    voice->envSamples = 0;
    voice->envDuration = static_cast<int>(props.attack * Globals::samplerate);
  }

  void FMSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note && v.envStage != Voice::Release) {
        v.envStage = Voice::Release;
        v.envSamples = 0;
        v.envDuration = static_cast<int>(props.release * Globals::samplerate);
      }
    }
  }

  float FMSynth::processOperator(float phase, float modulation) {
    return std::sin(phase + modulation);
  }

  float FMSynth::processAlgorithm(Voice& voice) {
    float samplerate = Globals::samplerate;
    
    // Calculate operator frequencies
    float freq1 = voice.baseFreq * props.op1Ratio;
    float freq2 = voice.baseFreq * props.op2Ratio;
    float freq3 = voice.baseFreq * props.op3Ratio;
    float freq4 = voice.baseFreq * props.op4Ratio;
    
    // Update operator phases
    voice.op1Phase += 2.0f * M_PI * freq1 / samplerate;
    voice.op2Phase += 2.0f * M_PI * freq2 / samplerate;
    voice.op3Phase += 2.0f * M_PI * freq3 / samplerate;
    voice.op4Phase += 2.0f * M_PI * freq4 / samplerate;
    
    // Wrap phases
    if (voice.op1Phase > 2.0f * M_PI) voice.op1Phase -= 2.0f * M_PI;
    if (voice.op2Phase > 2.0f * M_PI) voice.op2Phase -= 2.0f * M_PI;
    if (voice.op3Phase > 2.0f * M_PI) voice.op3Phase -= 2.0f * M_PI;
    if (voice.op4Phase > 2.0f * M_PI) voice.op4Phase -= 2.0f * M_PI;
    
    float out = 0;
    float modIndex = props.modIndex;
    
    // FM algorithms (inspired by classic DX7 topologies)
    switch (props.algorithm.get()) {
    case 1: // Serial: 4->3->2->1
      {
        float op4Out = processOperator(voice.op4Phase, 0) * props.op4Level;
        float op3Out = processOperator(voice.op3Phase, op4Out * modIndex) * props.op3Level;
        float op2Out = processOperator(voice.op2Phase, op3Out * modIndex) * props.op2Level;
        out = processOperator(voice.op1Phase, op2Out * modIndex) * props.op1Level;
      }
      break;
    case 2: // Parallel: 4+3+2+1
      out = processOperator(voice.op1Phase, 0) * props.op1Level +
            processOperator(voice.op2Phase, 0) * props.op2Level +
            processOperator(voice.op3Phase, 0) * props.op3Level +
            processOperator(voice.op4Phase, 0) * props.op4Level;
      break;
    case 3: // (4->3)+(2->1)
      {
        float op4Out = processOperator(voice.op4Phase, 0) * props.op4Level;
        float op3Out = processOperator(voice.op3Phase, op4Out * modIndex) * props.op3Level;
        float op2Out = processOperator(voice.op2Phase, 0) * props.op2Level;
        float op1Out = processOperator(voice.op1Phase, op2Out * modIndex) * props.op1Level;
        out = (op3Out + op1Out) * 0.5f;
      }
      break;
    case 4: // (4->3->2)+1
      {
        float op4Out = processOperator(voice.op4Phase, 0) * props.op4Level;
        float op3Out = processOperator(voice.op3Phase, op4Out * modIndex) * props.op3Level;
        float op2Out = processOperator(voice.op2Phase, op3Out * modIndex) * props.op2Level;
        float op1Out = processOperator(voice.op1Phase, 0) * props.op1Level;
        out = (op2Out + op1Out) * 0.5f;
      }
      break;
    case 5: // (4+3)->2->1
      {
        float op4Out = processOperator(voice.op4Phase, 0) * props.op4Level;
        float op3Out = processOperator(voice.op3Phase, 0) * props.op3Level;
        float mod = (op4Out + op3Out) * modIndex * 0.5f;
        float op2Out = processOperator(voice.op2Phase, mod) * props.op2Level;
        out = processOperator(voice.op1Phase, op2Out * modIndex) * props.op1Level;
      }
      break;
    case 6: // (4->3)+2+1
      {
        float op4Out = processOperator(voice.op4Phase, 0) * props.op4Level;
        float op3Out = processOperator(voice.op3Phase, op4Out * modIndex) * props.op3Level;
        float op2Out = processOperator(voice.op2Phase, 0) * props.op2Level;
        float op1Out = processOperator(voice.op1Phase, 0) * props.op1Level;
        out = (op3Out + op2Out + op1Out) / 3.0f;
      }
      break;
    case 7: // 4->(3+2)->1
      {
        float op4Out = processOperator(voice.op4Phase, 0) * props.op4Level;
        float op3Out = processOperator(voice.op3Phase, op4Out * modIndex) * props.op3Level;
        float op2Out = processOperator(voice.op2Phase, op4Out * modIndex) * props.op2Level;
        float mod = (op3Out + op2Out) * 0.5f * modIndex;
        out = processOperator(voice.op1Phase, mod) * props.op1Level;
      }
      break;
    case 8: // 4->(3+2+1)
      {
        float op4Out = processOperator(voice.op4Phase, 0) * props.op4Level;
        float op3Out = processOperator(voice.op3Phase, op4Out * modIndex) * props.op3Level;
        float op2Out = processOperator(voice.op2Phase, op4Out * modIndex) * props.op2Level;
        float op1Out = processOperator(voice.op1Phase, op4Out * modIndex) * props.op1Level;
        out = (op3Out + op2Out + op1Out) / 3.0f;
      }
      break;
    }
    
    return out;
  }

  void FMSynth::updateEnvelope(Voice& voice) {
    voice.envSamples++;
    
    switch (voice.envStage) {
    case Voice::Attack:
      if (voice.envDuration > 0) {
        voice.envValue = static_cast<float>(voice.envSamples) / voice.envDuration;
      } else {
        voice.envValue = 1.0f;
      }
      if (voice.envSamples >= voice.envDuration) {
        voice.envStage = Voice::Decay;
        voice.envSamples = 0;
        voice.envDuration = static_cast<int>(props.decay * Globals::samplerate);
      }
      break;
    case Voice::Decay:
      {
        float decayProgress = static_cast<float>(voice.envSamples) / voice.envDuration;
        voice.envValue = 1.0f - decayProgress * (1.0f - props.sustain);
        if (voice.envSamples >= voice.envDuration) {
          voice.envStage = Voice::Sustain;
          voice.envValue = props.sustain;
        }
      }
      break;
    case Voice::Sustain:
      voice.envValue = props.sustain;
      break;
    case Voice::Release:
      {
        float releaseStart = voice.envValue;
        if (voice.envDuration > 0) {
          float releaseProgress = static_cast<float>(voice.envSamples) / voice.envDuration;
          voice.envValue = releaseStart * (1.0f - releaseProgress);
        } else {
          voice.envValue = 0;
        }
        if (voice.envSamples >= voice.envDuration) {
          voice.envStage = Voice::Off;
          voice.active = false;
        }
      }
      break;
    case Voice::Off:
      voice.active = false;
      break;
    }
  }

  void FMSynth::process(const audio::ProcessData& data) {
    // Handle MIDI events
    for (auto& evt : data.midi) {
      evt.match([&] (midi::NoteOnEvent& e) {
          if (e.channel == 0) {
            triggerVoice(e.key, e.velocity / 127.0f);
          }
        }, [] (auto) {});
    }
    
    for (auto& evt : data.midi) {
      evt.match([&] (midi::NoteOffEvent& e) {
          if (e.channel == 0) {
            releaseVoice(e.key);
          }
        }, [] (auto) {});
    }
    
    // Process audio
    for (uint f = 0; f < data.nframes; f++) {
      float sample = 0;
      
      for (auto& voice : voices) {
        if (voice.active) {
          updateEnvelope(voice);
          float voiceSample = processAlgorithm(voice);
          sample += voiceSample * voice.envValue * voice.velocity;
        }
      }
      
      // Normalize and apply to output
      sample = std::clamp(sample * 0.3f, -1.0f, 1.0f);
      data.audio.proc[f] += sample;
    }
  }

  /*
   * FMSynthScreen
   */

  using namespace ui::drawing;

  bool FMSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void FMSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.algorithm.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.modIndex.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.attack.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.release.step(e.clicks);
      break;
    }
  }

  void FMSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_algorithm(ctx);
    draw_envelopes(ctx);
  }

  void FMSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.fillText("FM SYNTH", 160, 20);

    // Algorithm
    ctx.font(Fonts::Bold);
    ctx.font(18.f);
    ctx.fillStyle(Colours::Blue);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);
    ctx.fillText(fmt::format("ALG: {}", module->props.algorithm.get()), 10, 45);
    
    // Modulation index
    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("MOD: {:.1f}", module->props.modIndex.get()), 10, 70);
    
    // Envelope
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Left, TextAlign::Bottom);
    ctx.fillText(fmt::format("A:{:.3f} R:{:.3f}", 
      module->props.attack.get(), module->props.release.get()), 10, 230);

    ctx.restore();
  }

  void FMSynthScreen::draw_algorithm(Canvas& ctx) {
    ctx.save();
    
    float centerX = 240;
    float centerY = 120;
    float opRadius = 20;
    float spacing = 50;
    
    int algorithm = module->props.algorithm;
    
    // Draw operators as circles
    auto drawOp = [&](float x, float y, int opNum, float level) {
      ctx.beginPath();
      ctx.circle(x, y, opRadius);
      ctx.fillStyle(Colours::Blue.dim(0.3 + level * 0.7));
      ctx.fill();
      ctx.strokeStyle(Colours::Blue);
      ctx.lineWidth(2.0f);
      ctx.stroke();
      
      ctx.font(Fonts::Bold);
      ctx.font(16.f);
      ctx.fillStyle(Colours::White);
      ctx.textAlign(TextAlign::Center, TextAlign::Middle);
      ctx.fillText(std::to_string(opNum), x, y);
    };
    
    // Draw connection line
    auto drawConnection = [&](float x1, float y1, float x2, float y2) {
      ctx.beginPath();
      ctx.moveTo(x1, y1);
      ctx.lineTo(x2, y2);
      ctx.strokeStyle(Colours::Green);
      ctx.lineWidth(2.0f);
      ctx.stroke();
    };
    
    // Draw different algorithm topologies
    switch (algorithm) {
    case 1: // Serial: 4->3->2->1
      drawConnection(centerX, centerY - spacing * 1.5, centerX, centerY - spacing * 0.5);
      drawConnection(centerX, centerY - spacing * 0.5, centerX, centerY + spacing * 0.5);
      drawConnection(centerX, centerY + spacing * 0.5, centerX, centerY + spacing * 1.5);
      drawOp(centerX, centerY + spacing * 1.5, 1, module->props.op1Level);
      drawOp(centerX, centerY + spacing * 0.5, 2, module->props.op2Level);
      drawOp(centerX, centerY - spacing * 0.5, 3, module->props.op3Level);
      drawOp(centerX, centerY - spacing * 1.5, 4, module->props.op4Level);
      break;
    case 2: // Parallel: 4+3+2+1
      drawOp(centerX - spacing * 1.5, centerY, 1, module->props.op1Level);
      drawOp(centerX - spacing * 0.5, centerY, 2, module->props.op2Level);
      drawOp(centerX + spacing * 0.5, centerY, 3, module->props.op3Level);
      drawOp(centerX + spacing * 1.5, centerY, 4, module->props.op4Level);
      break;
    case 3: // (4->3)+(2->1)
      drawConnection(centerX - spacing, centerY - spacing, centerX - spacing, centerY);
      drawConnection(centerX + spacing, centerY - spacing, centerX + spacing, centerY);
      drawOp(centerX - spacing, centerY, 1, module->props.op1Level);
      drawOp(centerX + spacing, centerY, 3, module->props.op3Level);
      drawOp(centerX - spacing, centerY - spacing, 2, module->props.op2Level);
      drawOp(centerX + spacing, centerY - spacing, 4, module->props.op4Level);
      break;
    default:
      // Simple visualization for other algorithms
      drawOp(centerX, centerY - spacing, 4, module->props.op4Level);
      drawOp(centerX - spacing * 0.7, centerY, 3, module->props.op3Level);
      drawOp(centerX, centerY, 2, module->props.op2Level);
      drawOp(centerX + spacing * 0.7, centerY, 1, module->props.op1Level);
      break;
    }
    
    ctx.restore();
  }

  void FMSynthScreen::draw_envelopes(Canvas& ctx) {
    ctx.save();
    
    // Draw simple envelope visualization
    float envX = 200;
    float envY = 200;
    float envW = 100;
    float envH = 30;
    
    float attack = module->props.attack / 2.0; // Normalize
    float decay = module->props.decay / 2.0;
    float sustain = module->props.sustain;
    float release = module->props.release / 3.0;
    
    float totalTime = attack + decay + 0.2f + release;
    float scale = envW / totalTime;
    
    ctx.beginPath();
    ctx.moveTo(envX, envY + envH);
    ctx.lineTo(envX + attack * scale, envY);
    ctx.lineTo(envX + (attack + decay) * scale, envY + envH * (1.0f - sustain));
    ctx.lineTo(envX + (attack + decay + 0.2f) * scale, envY + envH * (1.0f - sustain));
    ctx.lineTo(envX + envW, envY + envH);
    ctx.strokeStyle(Colours::White);
    ctx.lineWidth(2.0f);
    ctx.stroke();
    
    ctx.restore();
  }

} // namespace top1::modules
