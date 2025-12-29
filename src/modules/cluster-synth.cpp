#include "cluster-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  class ClusterSynthScreen : public ui::ModuleScreen<ClusterSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_grains(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<ClusterSynth>::ModuleScreen;
  };

  ClusterSynth::ClusterSynth() :
    SynthModule(&props),
    screen (new ClusterSynthScreen(this)),
    gen(rd()),
    dist(0.0f, 1.0f) {}

  void ClusterSynth::display() {
    Globals::ui.display(*screen);
  }

  void ClusterSynth::triggerVoice(int note, float velocity) {
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
    voice->baseFrequency = midi::freqTable[note];
    voice->envValue = 0;
    voice->releasing = false;
    voice->envSamples = 0;
    
    for (auto& grain : voice->grains) {
      grain.active = false;
    }
  }

  void ClusterSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note) {
        v.releasing = true;
        v.envSamples = 0;
      }
    }
  }

  void ClusterSynth::spawnGrain(Voice& voice) {
    // Find inactive grain
    Grain* grain = nullptr;
    for (auto& g : voice.grains) {
      if (!g.active) {
        grain = &g;
        break;
      }
    }
    if (!grain) return;
    
    grain->active = true;
    grain->phase = 0;
    grain->grainPhase = 0;
    grain->grainDuration = props.grainSize * Globals::samplerate;
    
    // Pitch variation
    float pitchShift = std::pow(2.0f, props.pitch / 12.0f);
    float randomSpread = (dist(gen) - 0.5f) * props.spread * 2.0f;
    grain->frequency = voice.baseFrequency * pitchShift * std::pow(2.0f, randomSpread);
    
    // Amplitude variation
    grain->amplitude = 0.5f + dist(gen) * 0.5f;
  }

  void ClusterSynth::process(const audio::ProcessData& data) {
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
    float spawnRate = props.density * 50.0f; // Grains per second
    int samplesPerGrain = static_cast<int>(samplerate / spawnRate);
    
    for (uint f = 0; f < data.nframes; f++) {
      float sample = 0;
      
      for (auto& voice : voices) {
        if (voice.active) {
          voice.envSamples++;
          
          // Update envelope
          if (!voice.releasing) {
            float attackSamples = props.attack * samplerate;
            voice.envValue = std::min(1.0f, voice.envSamples / attackSamples);
          } else {
            float releaseSamples = props.release * samplerate;
            voice.envValue = std::max(0.0f, 1.0f - voice.envSamples / releaseSamples);
            if (voice.envValue <= 0) {
              voice.active = false;
              continue;
            }
          }
          
          // Spawn new grains
          if (!voice.releasing && (voice.envSamples % samplesPerGrain) == 0) {
            spawnGrain(voice);
          }
          
          // Process grains
          float voiceSample = 0;
          for (auto& grain : voice.grains) {
            if (grain.active) {
              // Hann window envelope
              float grainEnv = 0.5f * (1.0f - std::cos(2.0f * M_PI * grain.grainPhase / grain.grainDuration));
              
              // Generate sine wave
              float grainSample = std::sin(grain.phase * 2.0f * M_PI) * grain.amplitude * grainEnv;
              voiceSample += grainSample;
              
              // Update grain
              grain.phase += grain.frequency / samplerate;
              if (grain.phase >= 1.0f) grain.phase -= 1.0f;
              
              grain.grainPhase++;
              if (grain.grainPhase >= grain.grainDuration) {
                grain.active = false;
              }
            }
          }
          
          sample += voiceSample * voice.envValue * voice.velocity * 0.1f;
        }
      }
      
      data.audio.proc[f] += std::clamp(sample, -1.0f, 1.0f);
    }
  }

  using namespace ui::drawing;

  bool ClusterSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void ClusterSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.grainSize.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.density.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.pitch.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.spread.step(e.clicks);
      break;
    }
  }

  void ClusterSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_grains(ctx);
  }

  void ClusterSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("CLUSTER", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Grain: {:.0f}ms", module->props.grainSize.get() * 1000), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Density: {:.0f}%", module->props.density.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Pitch: {:+.1f}st", module->props.pitch.get()), 10, 90);
    ctx.fillText(fmt::format("Spread: {:.0f}%", module->props.spread.get() * 100), 10, 110);

    ctx.restore();
  }

  void ClusterSynthScreen::draw_grains(Canvas& ctx) {
    ctx.save();

    float grainsX = 160;
    float grainsY = 60;
    float grainsW = 150;
    float grainsH = 160;

    // Draw active grains
    int grainCount = 0;
    for (auto& voice : module->voices) {
      if (voice.active) {
        for (auto& grain : voice.grains) {
          if (grain.active) {
            float progress = grain.grainPhase / grain.grainDuration;
            float x = grainsX + (grainCount % 10) * (grainsW / 10);
            float y = grainsY + (grainCount / 10) * (grainsH / 3);
            float size = 8 * (1.0f - progress) * grain.amplitude;
            
            ctx.fillStyle(Colours::Green.dim(0.5 + grain.amplitude * 0.5));
            ctx.beginPath();
            ctx.circle(x, y, size);
            ctx.fill();
            
            grainCount++;
            if (grainCount >= 30) break;
          }
        }
        if (grainCount >= 30) break;
      }
    }

    ctx.restore();
  }

} // namespace top1::modules
