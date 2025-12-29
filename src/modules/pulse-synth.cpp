#include "pulse-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  /*
   * Declarations
   */

  class PulseSynthScreen : public ui::ModuleScreen<PulseSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_waveform(ui::drawing::Canvas& ctx);
    void draw_envelope(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<PulseSynth>::ModuleScreen;
  };

  /*
   * PulseSynth - Analog-style pulse synthesizer
   */

  PulseSynth::PulseSynth() :
    SynthModule(&props),
    screen (new PulseSynthScreen(this)) {}

  void PulseSynth::display() {
    Globals::ui.display(*screen);
  }

  void PulseSynth::triggerVoice(int note, float velocity) {
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
    voice->targetFreq = midi::freqTable[note + props.octave * 12];
    
    // Glide: if glide is 0, jump to target immediately
    if (props.glide == 0 || voice->currentFreq == 0) {
      voice->currentFreq = voice->targetFreq;
    }
    
    voice->envStage = Voice::Attack;
    voice->envValue = 0;
    voice->envSamples = 0;
    voice->envDuration = static_cast<int>(props.attack * Globals::samplerate);
  }

  void PulseSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note && v.envStage != Voice::Release) {
        v.envStage = Voice::Release;
        v.envSamples = 0;
        v.envDuration = static_cast<int>(props.release * Globals::samplerate);
      }
    }
  }

  float PulseSynth::generatePulse(float phase, float width) {
    // Generate pulse wave with variable width
    return (phase < width) ? 1.0f : -1.0f;
  }

  void PulseSynth::updateEnvelope(Voice& voice) {
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

  void PulseSynth::process(const audio::ProcessData& data) {
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
    
    float samplerate = Globals::samplerate;
    
    // Process audio
    for (uint f = 0; f < data.nframes; f++) {
      // Update LFO
      lfoPhase += 2.0f * M_PI * props.lfoRate / samplerate;
      if (lfoPhase > 2.0f * M_PI) lfoPhase -= 2.0f * M_PI;
      float lfoValue = std::sin(lfoPhase);
      
      float sample = 0;
      
      for (auto& voice : voices) {
        if (voice.active) {
          updateEnvelope(voice);
          
          // Apply glide
          if (voice.currentFreq != voice.targetFreq) {
            float glideSpeed = 1.0f - props.glide * 0.999f;
            voice.currentFreq += (voice.targetFreq - voice.currentFreq) * glideSpeed;
          }
          
          // Apply detune and LFO to frequency
          float freq = voice.currentFreq * (1.0f + props.detune * 0.1f);
          freq *= 1.0f + lfoValue * props.lfoDepth * 0.1f;
          
          // Update phase
          voice.phase += freq / samplerate;
          if (voice.phase >= 1.0f) voice.phase -= 1.0f;
          
          // Generate pulse wave
          float oscOut = generatePulse(voice.phase, props.pulseWidth);
          
          // Apply filter
          float filterCutoff = props.cutoff;
          // Filter envelope modulation
          filterCutoff = std::clamp(filterCutoff + voice.envValue * props.filterEnv, 0.0f, 1.0f);
          voice.filter.setParams(filterCutoff, props.resonance);
          float filtered = voice.filter.process(oscOut);
          
          // Apply amplitude envelope
          sample += filtered * voice.envValue * voice.velocity;
        }
      }
      
      // Normalize and apply to output
      sample = std::clamp(sample * 0.3f, -1.0f, 1.0f);
      data.audio.proc[f] += sample;
    }
  }

  /*
   * PulseSynthScreen
   */

  using namespace ui::drawing;

  bool PulseSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void PulseSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.pulseWidth.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.cutoff.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.attack.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.release.step(e.clicks);
      break;
    }
  }

  void PulseSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_waveform(ctx);
    draw_envelope(ctx);
  }

  void PulseSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.fillText("PULSE", 160, 20);

    // Parameters
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.fillStyle(Colours::Blue);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);
    ctx.fillText(fmt::format("Width: {:.2f}", module->props.pulseWidth.get()), 10, 45);
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Cutoff: {:.2f}", module->props.cutoff.get()), 10, 65);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Res: {:.2f}", module->props.resonance.get()), 10, 85);
    
    // Envelope
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Left, TextAlign::Bottom);
    ctx.fillText(fmt::format("A:{:.3f} D:{:.3f} S:{:.2f} R:{:.3f}", 
      module->props.attack.get(), 
      module->props.decay.get(),
      module->props.sustain.get(),
      module->props.release.get()), 10, 230);

    ctx.restore();
  }

  void PulseSynthScreen::draw_waveform(Canvas& ctx) {
    ctx.save();
    
    // Draw pulse waveform visualization
    float waveX = 180;
    float waveY = 80;
    float waveW = 120;
    float waveH = 60;
    
    float pw = module->props.pulseWidth;
    
    ctx.beginPath();
    ctx.moveTo(waveX, waveY + waveH/2);
    ctx.lineTo(waveX, waveY);
    ctx.lineTo(waveX + waveW * pw, waveY);
    ctx.lineTo(waveX + waveW * pw, waveY + waveH);
    ctx.lineTo(waveX + waveW, waveY + waveH);
    ctx.lineTo(waveX + waveW, waveY + waveH/2);
    
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(2.0f);
    ctx.stroke();
    
    // Draw center line
    ctx.beginPath();
    ctx.moveTo(waveX, waveY + waveH/2);
    ctx.lineTo(waveX + waveW, waveY + waveH/2);
    ctx.strokeStyle(Colours::White.dim(0.3));
    ctx.lineWidth(1.0f);
    ctx.stroke();
    
    ctx.restore();
  }

  void PulseSynthScreen::draw_envelope(Canvas& ctx) {
    ctx.save();
    
    // Draw ADSR envelope
    float envX = 180;
    float envY = 180;
    float envW = 120;
    float envH = 50;
    
    float attack = module->props.attack / 2.0;
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
    
    ctx.strokeStyle(Colours::Green);
    ctx.lineWidth(2.0f);
    ctx.stroke();
    
    ctx.restore();
  }

} // namespace top1::modules
