#include "digital-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  class DigitalSynthScreen : public ui::ModuleScreen<DigitalSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_harmonics(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<DigitalSynth>::ModuleScreen;
  };

  DigitalSynth::DigitalSynth() :
    SynthModule(&props),
    screen (new DigitalSynthScreen(this)) {}

  void DigitalSynth::display() {
    Globals::ui.display(*screen);
  }

  void DigitalSynth::triggerVoice(int note, float velocity) {
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
    voice->phases.fill(0);
    voice->envStage = Voice::Attack;
    voice->envValue = 0;
    voice->envSamples = 0;
    voice->envDuration = static_cast<int>(props.attack * Globals::samplerate);
  }

  void DigitalSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note && v.envStage != Voice::Release) {
        v.envStage = Voice::Release;
        v.envSamples = 0;
        v.envDuration = static_cast<int>(props.release * Globals::samplerate);
      }
    }
  }

  void DigitalSynth::updateEnvelope(Voice& voice) {
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

  void DigitalSynth::process(const audio::ProcessData& data) {
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
    int numHarmonics = props.harmonics;
    
    for (uint f = 0; f < data.nframes; f++) {
      float sample = 0;
      
      for (auto& voice : voices) {
        if (voice.active) {
          updateEnvelope(voice);
          
          // Additive synthesis - sum harmonics
          float voiceSample = 0;
          for (int h = 0; h < numHarmonics; h++) {
            float harmonicNum = h + 1;
            float freq = voice.frequency * harmonicNum;
            
            // Apply detune per harmonic
            float detune = (h > 0) ? (props.detune * (h * 0.1f)) : 0;
            freq *= (1.0f + detune);
            
            // Brightness - attenuate higher harmonics
            float amplitude = 1.0f / harmonicNum;
            float brightnessFilter = 1.0f - (h / static_cast<float>(numHarmonics)) * (1.0f - props.brightness);
            amplitude *= brightnessFilter;
            
            // Generate sine wave
            voiceSample += std::sin(voice.phases[h] * 2.0f * M_PI) * amplitude;
            
            // Update phase
            voice.phases[h] += freq / samplerate;
            if (voice.phases[h] >= 1.0f) voice.phases[h] -= 1.0f;
          }
          
          // Normalize by number of harmonics
          voiceSample /= std::sqrt(numHarmonics);
          
          sample += voiceSample * voice.envValue * voice.velocity;
        }
      }
      
      data.audio.proc[f] += std::clamp(sample * 0.5f, -1.0f, 1.0f);
    }
  }

  using namespace ui::drawing;

  bool DigitalSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void DigitalSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.harmonics.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.detune.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.brightness.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.attack.step(e.clicks);
      break;
    }
  }

  void DigitalSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_harmonics(ctx);
  }

  void DigitalSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("DIGITAL", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Harmonics: {}", module->props.harmonics.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Detune: {:.0f}%", module->props.detune.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Brightness: {:.0f}%", module->props.brightness.get() * 100), 10, 90);

    ctx.restore();
  }

  void DigitalSynthScreen::draw_harmonics(Canvas& ctx) {
    ctx.save();

    float spectrumX = 160;
    float spectrumY = 60;
    float spectrumW = 150;
    float spectrumH = 160;
    
    int numHarmonics = module->props.harmonics;
    float barWidth = spectrumW / numHarmonics;

    // Draw harmonic spectrum
    for (int i = 0; i < numHarmonics; i++) {
      float harmonicNum = i + 1;
      float amplitude = 1.0f / harmonicNum;
      
      // Apply brightness filter
      float brightnessFilter = 1.0f - (i / static_cast<float>(numHarmonics)) * 
                                      (1.0f - module->props.brightness.get());
      amplitude *= brightnessFilter;
      
      float height = amplitude * spectrumH;
      float x = spectrumX + i * barWidth;
      
      ctx.fillStyle(Colours::Green.dim(0.3 + amplitude * 0.7));
      ctx.beginPath();
      ctx.rect(x + 1, spectrumY + spectrumH - height, barWidth - 2, height);
      ctx.fill();
      
      ctx.strokeStyle(Colours::Blue.dim(0.5));
      ctx.lineWidth(1.0f);
      ctx.stroke();
    }

    ctx.restore();
  }

} // namespace top1::modules
