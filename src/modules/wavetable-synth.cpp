#include "wavetable-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  /*
   * Declarations
   */

  class WavetableSynthScreen : public ui::ModuleScreen<WavetableSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_wavetable(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<WavetableSynth>::ModuleScreen;
  };

  /*
   * WavetableSynth - Wavetable synthesizer
   */

  WavetableSynth::WavetableSynth() :
    SynthModule(&props),
    screen (new WavetableSynthScreen(this)) {
    initWavetables();
  }

  void WavetableSynth::initWavetables() {
    // Wavetable 1: Sine
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float phase = 2.0f * M_PI * i / WAVETABLE_SIZE;
      wavetables[0][i] = std::sin(phase);
    }
    
    // Wavetable 2: Triangle
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float t = static_cast<float>(i) / WAVETABLE_SIZE;
      wavetables[1][i] = (t < 0.25f) ? (4.0f * t) :
                         (t < 0.75f) ? (2.0f - 4.0f * t) :
                         (4.0f * t - 4.0f);
    }
    
    // Wavetable 3: Saw
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float t = static_cast<float>(i) / WAVETABLE_SIZE;
      wavetables[2][i] = 2.0f * t - 1.0f;
    }
    
    // Wavetable 4: Square
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float t = static_cast<float>(i) / WAVETABLE_SIZE;
      wavetables[3][i] = (t < 0.5f) ? 1.0f : -1.0f;
    }
    
    // Wavetable 5: Harmonics (multiple sines)
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float phase = 2.0f * M_PI * i / WAVETABLE_SIZE;
      wavetables[4][i] = std::sin(phase) * 0.5f +
                         std::sin(phase * 2) * 0.3f +
                         std::sin(phase * 3) * 0.2f;
    }
    
    // Wavetable 6: Organ-like
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float phase = 2.0f * M_PI * i / WAVETABLE_SIZE;
      wavetables[5][i] = std::sin(phase) * 0.4f +
                         std::sin(phase * 2) * 0.3f +
                         std::sin(phase * 4) * 0.2f +
                         std::sin(phase * 8) * 0.1f;
    }
    
    // Wavetable 7: Bell-like
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float phase = 2.0f * M_PI * i / WAVETABLE_SIZE;
      wavetables[6][i] = std::sin(phase) * 0.5f +
                         std::sin(phase * 1.414f) * 0.3f +
                         std::sin(phase * 2.718f) * 0.2f;
    }
    
    // Wavetable 8: Digital/Metallic
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float phase = 2.0f * M_PI * i / WAVETABLE_SIZE;
      wavetables[7][i] = std::sin(phase * 7) * 0.4f +
                         std::sin(phase * 11) * 0.3f +
                         std::sin(phase * 13) * 0.3f;
    }
  }

  void WavetableSynth::display() {
    Globals::ui.display(*screen);
  }

  void WavetableSynth::triggerVoice(int note, float velocity) {
    // Find free voice or steal oldest
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
    voice->frequency = midi::freqTable[note + props.octave * 12];
    voice->envStage = Voice::Attack;
    voice->envValue = 0;
    voice->envSamples = 0;
    voice->envDuration = static_cast<int>(props.attack * Globals::samplerate);
  }

  void WavetableSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note && v.envStage != Voice::Release) {
        v.envStage = Voice::Release;
        v.envSamples = 0;
        v.envDuration = static_cast<int>(props.release * Globals::samplerate);
      }
    }
  }

  float WavetableSynth::sampleWavetable(int tableIndex, float phase, float morph) {
    // Clamp table index
    tableIndex = std::clamp(tableIndex, 0, NUM_WAVETABLES - 1);
    
    // Calculate position in wavetable
    float floatIndex = phase * WAVETABLE_SIZE;
    int index1 = static_cast<int>(floatIndex) % WAVETABLE_SIZE;
    int index2 = (index1 + 1) % WAVETABLE_SIZE;
    float frac = floatIndex - std::floor(floatIndex);
    
    // Linear interpolation within wavetable
    float sample1 = wavetables[tableIndex][index1];
    float sample2 = wavetables[tableIndex][index2];
    float sample = sample1 + frac * (sample2 - sample1);
    
    // Morph between adjacent wavetables
    if (morph > 0 && tableIndex < NUM_WAVETABLES - 1) {
      float nextSample1 = wavetables[tableIndex + 1][index1];
      float nextSample2 = wavetables[tableIndex + 1][index2];
      float nextSample = nextSample1 + frac * (nextSample2 - nextSample1);
      sample = sample + morph * (nextSample - sample);
    }
    
    return sample;
  }

  void WavetableSynth::updateEnvelope(Voice& voice) {
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

  void WavetableSynth::process(const audio::ProcessData& data) {
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
      float sample = 0;
      
      for (auto& voice : voices) {
        if (voice.active) {
          updateEnvelope(voice);
          
          // Apply detune
          float freq = voice.frequency * (1.0f + props.detune * 0.1f);
          
          // Update phase
          voice.phase += freq / samplerate;
          if (voice.phase >= 1.0f) voice.phase -= 1.0f;
          
          // Sample wavetable
          int waveIndex = props.waveform - 1;
          float oscOut = sampleWavetable(waveIndex, voice.phase, props.morph);
          
          // Apply filter
          voice.filter.setParams(props.cutoff, props.resonance);
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
   * WavetableSynthScreen
   */

  using namespace ui::drawing;

  bool WavetableSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void WavetableSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.waveform.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.morph.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.cutoff.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.attack.step(e.clicks);
      break;
    }
  }

  void WavetableSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_wavetable(ctx);
  }

  void WavetableSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.fillText("WAVETABLE", 160, 20);

    // Parameters
    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.fillStyle(Colours::Blue);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);
    
    const char* waveNames[] = {"Sine", "Tri", "Saw", "Square", "Harm", "Organ", "Bell", "Metal"};
    int waveIdx = std::clamp(module->props.waveform.get() - 1, 0, 7);
    ctx.fillText(fmt::format("Wave: {}", waveNames[waveIdx]), 10, 45);
    
    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Morph: {:.2f}", module->props.morph.get()), 10, 65);
    
    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Cutoff: {:.2f}", module->props.cutoff.get()), 10, 85);
    
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

  void WavetableSynthScreen::draw_wavetable(Canvas& ctx) {
    ctx.save();
    
    // Draw current wavetable visualization
    float waveX = 180;
    float waveY = 100;
    float waveW = 120;
    float waveH = 80;
    
    int waveIndex = std::clamp(module->props.waveform.get() - 1, 0, 7);
    float morph = module->props.morph;
    
    ctx.beginPath();
    
    int numSamples = 256;
    for (int i = 0; i <= numSamples; i++) {
      float phase = static_cast<float>(i) / numSamples;
      float sample = module->sampleWavetable(waveIndex, phase, morph);
      
      float x = waveX + phase * waveW;
      float y = waveY + waveH/2 - sample * waveH/2;
      
      if (i == 0) {
        ctx.moveTo(x, y);
      } else {
        ctx.lineTo(x, y);
      }
    }
    
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

} // namespace top1::modules
