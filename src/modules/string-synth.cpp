#include "string-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>
#include <random>

namespace top1::modules {

  class StringSynthScreen : public ui::ModuleScreen<StringSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_string(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<StringSynth>::ModuleScreen;
  };

  StringSynth::StringSynth() :
    SynthModule(&props),
    screen (new StringSynthScreen(this)) {}

  void StringSynth::display() {
    Globals::ui.display(*screen);
  }

  void StringSynth::triggerVoice(int note, float velocity) {
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
    
    // Calculate delay length for Karplus-Strong
    float frequency = midi::freqTable[note];
    voice->delayLength = static_cast<int>(Globals::samplerate / frequency);
    voice->delayLength = std::clamp(voice->delayLength, 10, MAX_DELAY_SIZE - 1);
    voice->writePos = 0;
    
    // Initialize with noise burst at pick position
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    
    int pickPosition = static_cast<int>(props.pickPos * voice->delayLength);
    
    for (int i = 0; i < voice->delayLength; i++) {
      if (i < pickPosition) {
        voice->delayLine[i] = dis(gen) * velocity;
      } else {
        voice->delayLine[i] = 0;
      }
    }
    
    voice->excitation = 1.0f;
    voice->envValue = 1.0f;
    voice->envSamples = 0;
    voice->lpFilterState = 0;
  }

  void StringSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note) {
        v.excitation = 0.0f;
      }
    }
  }

  void StringSynth::process(const audio::ProcessData& data) {
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
          // Update envelope
          voice.envSamples++;
          float attackSamples = props.attack * samplerate;
          float decaySamples = props.decay * samplerate;
          
          if (voice.envSamples < attackSamples) {
            voice.envValue = voice.envSamples / attackSamples;
          } else {
            float decayProgress = (voice.envSamples - attackSamples) / decaySamples;
            voice.envValue = std::exp(-decayProgress * 5.0f);
          }
          
          if (voice.envValue < 0.001f) {
            voice.active = false;
            continue;
          }
          
          // Read from delay line
          float output = voice.delayLine[voice.writePos];
          
          // Karplus-Strong algorithm with damping
          int readPos = (voice.writePos + 1) % voice.delayLength;
          float delayed1 = voice.delayLine[readPos];
          readPos = (readPos + 1) % voice.delayLength;
          float delayed2 = voice.delayLine[readPos];
          
          // Average and apply damping
          float averaged = (delayed1 + delayed2) * 0.5f;
          float dampingFactor = 1.0f - props.damping * 0.5f;
          averaged *= dampingFactor;
          
          // Brightness filter
          voice.lpFilterState = voice.lpFilterState * (1.0f - props.brightness) + averaged * props.brightness;
          
          // Tension (feedback)
          float feedback = voice.lpFilterState * (0.99f + props.tension * 0.009f);
          
          // Write back to delay line
          voice.delayLine[voice.writePos] = feedback + voice.excitation * 0.1f;
          voice.excitation *= 0.995f;
          
          voice.writePos = (voice.writePos + 1) % voice.delayLength;
          
          sample += output * voice.envValue * voice.velocity;
        }
      }
      
      data.audio.proc[f] += std::clamp(sample * 0.5f, -1.0f, 1.0f);
    }
  }

  using namespace ui::drawing;

  bool StringSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void StringSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.tension.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.damping.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.pickPos.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.brightness.step(e.clicks);
      break;
    }
  }

  void StringSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_string(ctx);
  }

  void StringSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("STRING", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Tension: {:.0f}%", module->props.tension.get() * 100), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Damping: {:.0f}%", module->props.damping.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Pick Pos: {:.0f}%", module->props.pickPos.get() * 100), 10, 90);
    ctx.fillText(fmt::format("Brightness: {:.0f}%", module->props.brightness.get() * 100), 10, 110);

    ctx.restore();
  }

  void StringSynthScreen::draw_string(Canvas& ctx) {
    ctx.save();

    float stringX = 180;
    float stringY = 120;
    float stringW = 120;
    float stringH = 60;

    // Draw string at rest
    ctx.beginPath();
    ctx.moveTo(stringX, stringY + stringH/2);
    ctx.lineTo(stringX + stringW, stringY + stringH/2);
    ctx.strokeStyle(Colours::White.dim(0.3));
    ctx.lineWidth(1.0f);
    ctx.stroke();

    // Draw excited string (first active voice)
    for (auto& voice : module->voices) {
      if (voice.active && voice.delayLength > 0) {
        ctx.beginPath();
        int samples = std::min(voice.delayLength, 128);
        for (int i = 0; i < samples; i++) {
          int idx = (voice.writePos + i) % voice.delayLength;
          float val = voice.delayLine[idx];
          
          float x = stringX + (i / (float)samples) * stringW;
          float y = stringY + stringH/2 + val * stringH * 0.5f;
          
          if (i == 0) ctx.moveTo(x, y);
          else ctx.lineTo(x, y);
        }
        ctx.strokeStyle(Colours::Green);
        ctx.lineWidth(2.0f);
        ctx.stroke();
        break; // Only draw first active voice
      }
    }

    // Draw pick position marker
    float pickX = stringX + module->props.pickPos * stringW;
    ctx.beginPath();
    ctx.moveTo(pickX, stringY);
    ctx.lineTo(pickX, stringY + stringH);
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(2.0f);
    ctx.stroke();

    ctx.restore();
  }

} // namespace top1::modules
