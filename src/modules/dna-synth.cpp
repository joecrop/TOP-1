#include "dna-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>
#include <random>

namespace top1::modules {

  class DNASynthScreen : public ui::ModuleScreen<DNASynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_waveform(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<DNASynth>::ModuleScreen;
  };

  DNASynth::DNASynth() :
    SynthModule(&props),
    screen (new DNASynthScreen(this)) {
    initWavetables();
  }

  void DNASynth::display() {
    Globals::ui.display(*screen);
  }

  void DNASynth::initWavetables() {
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
      float phase = static_cast<float>(i) / WAVETABLE_SIZE;
      
      // Table 1: Sine
      wavetables[0][i] = std::sin(phase * 2.0f * M_PI);
      
      // Table 2: Sawtooth
      wavetables[1][i] = phase * 2.0f - 1.0f;
      
      // Table 3: Square
      wavetables[2][i] = (phase < 0.5f) ? 1.0f : -1.0f;
      
      // Table 4: Triangle
      wavetables[3][i] = (phase < 0.5f) ? (phase * 4.0f - 1.0f) : (3.0f - phase * 4.0f);
      
      // Table 5: Pulse
      wavetables[4][i] = (phase < 0.25f) ? 1.0f : -1.0f;
      
      // Table 6: Stepped
      int steps = 8;
      wavetables[5][i] = std::floor(phase * steps) / steps * 2.0f - 1.0f;
      
      // Table 7: Noise-like
      std::mt19937 gen(i);
      std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
      wavetables[6][i] = dist(gen) * 0.5f + std::sin(phase * 2.0f * M_PI) * 0.5f;
      
      // Table 8: Harmonic rich
      float harmonic = 0;
      for (int h = 1; h <= 5; h++) {
        harmonic += std::sin(phase * 2.0f * M_PI * h) / h;
      }
      wavetables[7][i] = harmonic / 2.0f;
    }
  }

  float DNASynth::interpolateWavetable(int tableIdx, float phase) {
    if (tableIdx < 0 || tableIdx >= NUM_TABLES) return 0;
    
    float pos = phase * WAVETABLE_SIZE;
    int idx0 = static_cast<int>(pos) % WAVETABLE_SIZE;
    int idx1 = (idx0 + 1) % WAVETABLE_SIZE;
    float frac = pos - std::floor(pos);
    
    return wavetables[tableIdx][idx0] * (1.0f - frac) + wavetables[tableIdx][idx1] * frac;
  }

  float DNASynth::morphTables(float phase, int tableA, int tableB, float morph, float mutate) {
    float sampleA = interpolateWavetable(tableA - 1, phase);
    float sampleB = interpolateWavetable(tableB - 1, phase);
    
    // Morph between tables
    float morphed = sampleA * (1.0f - morph) + sampleB * morph;
    
    // Apply mutation (phase distortion)
    if (mutate > 0) {
      float mutatedPhase = phase + std::sin(phase * 2.0f * M_PI * 3.0f) * mutate * 0.3f;
      mutatedPhase = mutatedPhase - std::floor(mutatedPhase);
      float mutatedSample = sampleA * (1.0f - morph) + sampleB * morph;
      morphed = morphed * (1.0f - mutate * 0.5f) + mutatedSample * (mutate * 0.5f);
    }
    
    return morphed;
  }

  void DNASynth::triggerVoice(int note, float velocity) {
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
    voice->envStage = Voice::Attack;
    voice->envValue = 0;
    voice->envSamples = 0;
    voice->envDuration = static_cast<int>(props.attack * Globals::samplerate);
  }

  void DNASynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note && v.envStage != Voice::Release) {
        v.envStage = Voice::Release;
        v.envSamples = 0;
        v.envDuration = static_cast<int>(props.release * Globals::samplerate);
      }
    }
  }

  void DNASynth::updateEnvelope(Voice& voice) {
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

  void DNASynth::process(const audio::ProcessData& data) {
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
          
          // Generate sample with morphing and mutation
          float output = morphTables(voice.phase, props.tableA, props.tableB, 
                                     props.morph, props.mutate);
          
          sample += output * voice.envValue * voice.velocity;
          
          // Update phase
          voice.phase += voice.frequency / samplerate;
          if (voice.phase >= 1.0f) voice.phase -= 1.0f;
        }
      }
      
      data.audio.proc[f] += std::clamp(sample * 0.4f, -1.0f, 1.0f);
    }
  }

  using namespace ui::drawing;

  bool DNASynthScreen::keypress(ui::Key key) {
    return false;
  }

  void DNASynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.tableA.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.tableB.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.morph.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.mutate.step(e.clicks);
      break;
    }
  }

  void DNASynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_waveform(ctx);
  }

  void DNASynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("DNA", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Table A: {}", module->props.tableA.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Table B: {}", module->props.tableB.get()), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Morph: {:.0f}%", module->props.morph.get() * 100), 10, 90);
    ctx.fillText(fmt::format("Mutate: {:.0f}%", module->props.mutate.get() * 100), 10, 110);

    ctx.restore();
  }

  void DNASynthScreen::draw_waveform(Canvas& ctx) {
    ctx.save();

    float waveX = 180;
    float waveY = 100;
    float waveW = 120;
    float waveH = 100;

    // Draw morphed waveform
    ctx.beginPath();
    for (int i = 0; i <= 100; i++) {
      float phase = i / 100.0f;
      float output = module->morphTables(phase, module->props.tableA, module->props.tableB,
                                         module->props.morph, module->props.mutate);
      
      float x = waveX + (i / 100.0f) * waveW;
      float y = waveY + waveH/2 - output * waveH/2;
      
      if (i == 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.strokeStyle(Colours::Green);
    ctx.lineWidth(2.0f);
    ctx.stroke();

    // Draw table A indicator
    ctx.fillStyle(Colours::Blue.dim(0.5));
    ctx.font(Fonts::Norm);
    ctx.font(10.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);
    ctx.fillText(fmt::format("A:{}", module->props.tableA.get()), waveX, waveY - 15);

    // Draw table B indicator
    ctx.fillStyle(Colours::Green.dim(0.5));
    ctx.textAlign(TextAlign::Right, TextAlign::Top);
    ctx.fillText(fmt::format("B:{}", module->props.tableB.get()), waveX + waveW, waveY - 15);

    ctx.restore();
  }

} // namespace top1::modules
