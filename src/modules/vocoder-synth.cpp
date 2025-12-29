#include "vocoder-synth.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  static constexpr int NUM_BANDS = 16;

  class VocoderSynthScreen : public ui::ModuleScreen<VocoderSynth> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_bands(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<VocoderSynth>::ModuleScreen;
  };

  VocoderSynth::VocoderSynth() :
    SynthModule(&props),
    screen (new VocoderSynthScreen(this)) {}

  void VocoderSynth::display() {
    Globals::ui.display(*screen);
  }

  void VocoderSynth::triggerVoice(int note, float velocity) {
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
    voice->envValue = 0;
    voice->envSamples = 0;
    voice->releasing = false;
    
    // Initialize filter banks
    float samplerate = Globals::samplerate;
    for (int i = 0; i < NUM_BANDS; i++) {
      float freq = 80.0f * std::pow(2.0f, i * 0.5f);
      voice->carrierFilters[i].setParams(freq, freq * props.bandwidth * 0.5f, samplerate);
      voice->modulatorFilters[i].setParams(freq, freq * props.bandwidth * 0.5f, samplerate);
      voice->envelopes[i] = 0;
    }
  }

  void VocoderSynth::releaseVoice(int note) {
    for (auto& v : voices) {
      if (v.active && v.note == note) {
        v.releasing = true;
        v.envSamples = 0;
      }
    }
  }

  void VocoderSynth::process(const audio::ProcessData& data) {
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
          
          // Generate carrier (sawtooth)
          voice.phase += voice.frequency / samplerate;
          if (voice.phase >= 1.0f) voice.phase -= 1.0f;
          float carrier = (voice.phase * 2.0f - 1.0f) * props.carrier;
          
          // Generate modulator (pulse with formant)
          float modPhase = voice.phase * props.formant;
          modPhase = modPhase - std::floor(modPhase);
          float modulator = (modPhase < 0.5f ? 1.0f : -1.0f) * props.modulator;
          
          // Process through filter banks
          float output = 0;
          for (int b = 0; b < NUM_BANDS; b++) {
            // Filter carrier and modulator
            float filteredCarrier = voice.carrierFilters[b].process(carrier);
            float filteredMod = voice.modulatorFilters[b].process(modulator);
            
            // Envelope following on modulator
            float modEnv = std::abs(filteredMod);
            float envCoeff = 0.99f;
            voice.envelopes[b] = voice.envelopes[b] * envCoeff + modEnv * (1.0f - envCoeff);
            
            // Apply modulator envelope to carrier
            output += filteredCarrier * voice.envelopes[b];
          }
          
          sample += output * voice.envValue * voice.velocity * 0.1f;
        }
      }
      
      data.audio.proc[f] += std::clamp(sample, -1.0f, 1.0f);
    }
  }

  using namespace ui::drawing;

  bool VocoderSynthScreen::keypress(ui::Key key) {
    return false;
  }

  void VocoderSynthScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.formant.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.carrier.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.modulator.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.bandwidth.step(e.clicks);
      break;
    }
  }

  void VocoderSynthScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_bands(ctx);
  }

  void VocoderSynthScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("VOCODER", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Formant: {}", module->props.formant.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Carrier: {:.0f}%", module->props.carrier.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Modulator: {:.0f}%", module->props.modulator.get() * 100), 10, 90);
    ctx.fillText(fmt::format("Bandwidth: {:.1f}", module->props.bandwidth.get()), 10, 110);

    ctx.restore();
  }

  void VocoderSynthScreen::draw_bands(Canvas& ctx) {
    ctx.save();

    float bandsX = 160;
    float bandsY = 100;
    float bandsW = 140;
    float bandsH = 100;

    // Draw vocoder bands
    float bandWidth = bandsW / NUM_BANDS;
    
    for (int i = 0; i < NUM_BANDS; i++) {
      float height = 0;
      
      // Get envelope level from first active voice
      for (auto& voice : module->voices) {
        if (voice.active) {
          height = voice.envelopes[i] * bandsH;
          break;
        }
      }
      
      float x = bandsX + i * bandWidth;
      
      ctx.fillStyle(Colours::Green.dim(0.3 + height / bandsH * 0.7));
      ctx.beginPath();
      ctx.rect(x, bandsY + bandsH - height, bandWidth - 2, height);
      ctx.fill();
      
      ctx.strokeStyle(Colours::Blue);
      ctx.lineWidth(1.0f);
      ctx.stroke();
    }

    ctx.restore();
  }

} // namespace top1::modules
