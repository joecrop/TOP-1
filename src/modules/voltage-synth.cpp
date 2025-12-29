#include "voltage-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  class VoltageSynthScreen : public ui::ModuleScreen<VoltageSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_waveform(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<VoltageSynth>::ModuleScreen;
  };

  VoltageSynth::VoltageSynth() :
    SynthModule(&props),
    screen (new VoltageSynthScreen(this)) {}

  void VoltageSynth::display() {
    Globals::ui.display(*screen);
  }

  void VoltageSynth::triggerVoice(int note, float velocity) {
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
    voice->filter.ic1eq = 0;
    voice->filter.ic2eq = 0;
    
    voice->ampEnvStage = Voice::Attack;
    voice->filterEnvStage = Voice::Attack;
    voice->ampEnvValue = 0;
    voice->filterEnvValue = 0;
    voice->ampEnvSamples = 0;
    voice->filterEnvSamples = 0;
    voice->ampEnvDuration = static_cast<int>(props.attack * Globals::samplerate);
    voice->filterEnvDuration = static_cast<int>(props.attack * 0.5f * Globals::samplerate);
  }

  void VoltageSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note) {
        if (v.ampEnvStage != Voice::Release) {
          v.ampEnvStage = Voice::Release;
          v.ampEnvSamples = 0;
          v.ampEnvDuration = static_cast<int>(props.release * Globals::samplerate);
        }
        if (v.filterEnvStage != Voice::Release) {
          v.filterEnvStage = Voice::Release;
          v.filterEnvSamples = 0;
          v.filterEnvDuration = static_cast<int>(props.release * Globals::samplerate);
        }
      }
    }
  }

  void VoltageSynth::updateEnvelope(Voice& voice, Voice::EnvStage& stage, float& value, 
                                     int& samples, int& duration, bool isFilter) {
    samples++;
    
    switch (stage) {
    case Voice::Attack:
      if (duration > 0) {
        value = static_cast<float>(samples) / duration;
      } else {
        value = 1.0f;
      }
      if (samples >= duration) {
        stage = Voice::Decay;
        samples = 0;
        duration = static_cast<int>(props.decay * Globals::samplerate);
      }
      break;
    case Voice::Decay:
      {
        float decayProgress = static_cast<float>(samples) / duration;
        value = 1.0f - decayProgress * (1.0f - props.sustain);
        if (samples >= duration) {
          stage = Voice::Sustain;
          value = props.sustain;
        }
      }
      break;
    case Voice::Sustain:
      value = props.sustain;
      break;
    case Voice::Release:
      {
        float releaseStart = value;
        if (duration > 0) {
          float releaseProgress = static_cast<float>(samples) / duration;
          value = releaseStart * (1.0f - releaseProgress);
        } else {
          value = 0;
        }
        if (samples >= duration) {
          stage = Voice::Off;
          if (!isFilter) voice.active = false;
        }
      }
      break;
    case Voice::Off:
      if (!isFilter) voice.active = false;
      break;
    }
  }

  void VoltageSynth::process(const audio::ProcessData& data) {
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
          updateEnvelope(voice, voice.ampEnvStage, voice.ampEnvValue, 
                        voice.ampEnvSamples, voice.ampEnvDuration, false);
          updateEnvelope(voice, voice.filterEnvStage, voice.filterEnvValue, 
                        voice.filterEnvSamples, voice.filterEnvDuration, true);
          
          // Generate waveform
          float osc = 0;
          switch (props.waveform.get()) {
          case 1: // Sawtooth
            osc = voice.phase * 2.0f - 1.0f;
            break;
          case 2: // Square
            osc = (voice.phase < 0.5f) ? 1.0f : -1.0f;
            break;
          case 3: // Triangle
            osc = (voice.phase < 0.5f) ? (voice.phase * 4.0f - 1.0f) : (3.0f - voice.phase * 4.0f);
            break;
          }
          
          // Update phase
          voice.phase += voice.frequency / samplerate;
          if (voice.phase >= 1.0f) voice.phase -= 1.0f;
          
          // Apply filter with envelope modulation
          float modCutoff = props.cutoff + voice.filterEnvValue * props.envAmount;
          modCutoff = std::clamp(modCutoff, 0.0f, 1.0f);
          float cutoffFreq = 20.0f + modCutoff * (samplerate * 0.45f - 20.0f);
          float normalizedCutoff = cutoffFreq / samplerate;
          
          float lp, bp, hp;
          voice.filter.process(osc, normalizedCutoff, props.resonance, lp, bp, hp);
          
          sample += lp * voice.ampEnvValue * voice.velocity;
        }
      }
      
      data.audio.proc[f] += std::clamp(sample * 0.4f, -1.0f, 1.0f);
    }
  }

  using namespace ui::drawing;

  bool VoltageSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void VoltageSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.waveform.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.cutoff.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.resonance.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.envAmount.step(e.clicks);
      break;
    }
  }

  void VoltageSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_waveform(ctx);
  }

  void VoltageSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("VOLTAGE", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    const char* waveName[] = {"", "SAW", "SQUARE", "TRI"};
    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Wave: {}", waveName[module->props.waveform.get()]), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Cutoff: {:.0f}%", module->props.cutoff.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Resonance: {:.0f}%", module->props.resonance.get() * 100), 10, 90);
    ctx.fillText(fmt::format("Env: {:.0f}%", module->props.envAmount.get() * 100), 10, 110);

    ctx.restore();
  }

  void VoltageSynthScreen::draw_waveform(Canvas& ctx) {
    ctx.save();

    float waveX = 180;
    float waveY = 100;
    float waveW = 120;
    float waveH = 100;

    // Draw waveform
    ctx.beginPath();
    for (int i = 0; i <= 100; i++) {
      float phase = i / 100.0f;
      float output = 0;
      
      switch (module->props.waveform.get()) {
      case 1: // Sawtooth
        output = phase * 2.0f - 1.0f;
        break;
      case 2: // Square
        output = (phase < 0.5f) ? 1.0f : -1.0f;
        break;
      case 3: // Triangle
        output = (phase < 0.5f) ? (phase * 4.0f - 1.0f) : (3.0f - phase * 4.0f);
        break;
      }
      
      float x = waveX + (i / 100.0f) * waveW;
      float y = waveY + waveH/2 - output * waveH/2;
      
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.strokeStyle(Colours::Green);
    ctx.lineWidth(2.0f);
    ctx.stroke();

    // Draw cutoff indicator
    float cutoffY = waveY + waveH * (1.0f - module->props.cutoff.get());
    ctx.beginPath();
    ctx.moveTo(waveX, cutoffY);
    ctx.lineTo(waveX + waveW, cutoffY);
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(1.0f);
    ctx.stroke();

    ctx.restore();
  }

} // namespace top1::modules
