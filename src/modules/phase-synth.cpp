#include "phase-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  class PhaseSynthScreen : public ui::ModuleScreen<PhaseSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_waveform(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<PhaseSynth>::ModuleScreen;
  };

  PhaseSynth::PhaseSynth() :
    SynthModule(&props),
    screen (new PhaseSynthScreen(this)) {}

  void PhaseSynth::display() {
    Globals::ui.display(*screen);
  }

  void PhaseSynth::triggerVoice(int note, float velocity) {
    Voice* voice = nullptr;
    for (auto& v : voices) {
      if (!v.active) {
        voice = &v;
        break;
      }
    }
    if (!voice) {
      voice = &voices[0];
    }
    
    voice->active = true;
    voice->note = note;
    voice->velocity = velocity;
    voice->frequency = midi::freqTable[note];
    voice->phase = 0;
    voice->feedbackSample = 0;
    voice->envStage = Voice::Attack;
    voice->envValue = 0;
    voice->envSamples = 0;
    voice->envDuration = static_cast<int>(props.attack * Globals::samplerate);
  }

  void PhaseSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note && v.envStage != Voice::Release) {
        v.envStage = Voice::Release;
        v.envSamples = 0;
        v.envDuration = static_cast<int>(props.release * Globals::samplerate);
      }
    }
  }

  void PhaseSynth::updateEnvelope(Voice& voice) {
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

  void PhaseSynth::process(const audio::ProcessData& data) {
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
    
    for (uint f = 0; f < data.nframes; f++) {
      float sample = 0;
      
      for (auto& voice : voices) {
        if (voice.active) {
          updateEnvelope(voice);
          
          // Phase distortion synthesis
          float phaseInc = voice.frequency / samplerate;
          
          // Apply feedback
          float modPhase = voice.phase + voice.feedbackSample * props.feedback;
          
          // Phase distortion with index
          float distortedPhase = modPhase;
          if (modPhase < 0.5f) {
            distortedPhase = std::pow(modPhase * 2.0f, props.resonance) * 0.5f;
          } else {
            distortedPhase = 1.0f - std::pow((1.0f - modPhase) * 2.0f, props.resonance) * 0.5f;
          }
          
          // Apply index modulation
          distortedPhase = modPhase + (distortedPhase - modPhase) * props.index;
          
          // Generate sine wave from distorted phase
          float output = std::sin(distortedPhase * 2.0f * M_PI);
          
          // Brightness filter
          voice.lpFilterState = voice.lpFilterState * (1.0f - props.brightness) + 
                                output * props.brightness;
          
          voice.feedbackSample = voice.lpFilterState;
          
          // Update phase
          voice.phase += phaseInc;
          if (voice.phase >= 1.0f) voice.phase -= 1.0f;
          
          sample += voice.lpFilterState * voice.envValue * voice.velocity;
        }
      }
      
      data.audio.proc[f] += std::clamp(sample * 0.4f, -1.0f, 1.0f);
    }
  }

  using namespace ui::drawing;

  bool PhaseSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void PhaseSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.index.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.feedback.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.resonance.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.brightness.step(e.clicks);
      break;
    }
  }

  void PhaseSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_waveform(ctx);
  }

  void PhaseSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("PHASE", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Index: {:.1f}", module->props.index.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Feedback: {:.0f}%", module->props.feedback.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Resonance: {:.1f}", module->props.resonance.get()), 10, 90);
    ctx.fillText(fmt::format("Brightness: {:.0f}%", module->props.brightness.get() * 100), 10, 110);

    ctx.restore();
  }

  void PhaseSynthScreen::draw_waveform(Canvas& ctx) {
    ctx.save();

    float waveX = 180;
    float waveY = 100;
    float waveW = 120;
    float waveH = 100;

    // Draw phase-distorted waveform
    ctx.beginPath();
    for (int i = 0; i <= 100; i++) {
      float phase = i / 100.0f;
      
      // Apply phase distortion
      float distorted = phase;
      if (phase < 0.5f) {
        distorted = std::pow(phase * 2.0f, module->props.resonance.get()) * 0.5f;
      } else {
        distorted = 1.0f - std::pow((1.0f - phase) * 2.0f, module->props.resonance.get()) * 0.5f;
      }
      
      float mixed = phase + (distorted - phase) * module->props.index;
      float output = std::sin(mixed * 2.0f * M_PI);
      
      float x = waveX + (i / 100.0f) * waveW;
      float y = waveY + waveH/2 - output * waveH/2;
      
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.strokeStyle(Colours::Green);
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

} // namespace top1::modules
