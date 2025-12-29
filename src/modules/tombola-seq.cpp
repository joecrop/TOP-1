#include "tombola-seq.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  class TombolaSeqScreen : public ui::ModuleScreen<TombolaSeq> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_steps(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<TombolaSeq>::ModuleScreen;
  };

  TombolaSeq::TombolaSeq() :
    SequencerModule(&props),
    screen (new TombolaSeqScreen(this)),
    gen(rd()),
    dist(0.0f, 1.0f) {
    
    // Initialize pattern
    for (int i = 0; i < 32; i++) {
      notes[i] = 60 + (gen() % 24);
      probabilities[i] = props.probability;
    }
  }

  void TombolaSeq::display() {
    Globals::ui.display(*screen);
  }

  void TombolaSeq::process(const audio::ProcessData& data) {
    float bpm = Globals::metronome.props.bpm;
    float beatsPerDiv = 4.0f / props.division;
    float secondsPerDiv = (60.0f / bpm) * beatsPerDiv;
    int samplesPerDiv = static_cast<int>(secondsPerDiv * Globals::samplerate);
    
    for (uint f = 0; f < data.nframes; f++) {
      samplesAccumulator++;
      
      if (samplesAccumulator >= samplesPerDiv) {
        samplesAccumulator = 0;
        
        // Note off for previous note
        if (currentNote >= 0) {
          auto* noteOffEvt = new midi::NoteOffEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_OFF, 0, nullptr, 0
          });
          noteOffEvt->key = currentNote;
          noteOffEvt->velocity = 64;
          const_cast<audio::ProcessData&>(data).midi.push_back(noteOffEvt);
          currentNote = -1;
        }
        
        // Advance position
        position = (position + 1) % props.steps;
        
        // Randomize probability for this step
        float stepProb = probabilities[position];
        if (props.randomness > 0) {
          float randomOffset = (dist(gen) - 0.5f) * 2.0f * props.randomness;
          stepProb = std::clamp(stepProb + randomOffset, 0.0f, 1.0f);
        }
        
        // Roll the dice
        if (dist(gen) < stepProb) {
          int note = notes[position];
          
          // Randomize note if randomness is high
          if (props.randomness > 0.5f) {
            int noteOffset = (gen() % 7) - 3;
            note = std::clamp(note + noteOffset, 0, 127);
          }
          
          float velocity = 0.7f + dist(gen) * 0.3f;
          
          auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_ON, 0, nullptr, 0
          });
          noteOnEvt->key = note;
          noteOnEvt->velocity = static_cast<uint8_t>(velocity * 127);
          const_cast<audio::ProcessData&>(data).midi.push_back(noteOnEvt);
          currentNote = note;
        }
      }
    }
  }

  using namespace ui::drawing;

  bool TombolaSeqScreen::keypress(ui::Key key) {
    return false;
  }

  void TombolaSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.probability.step(e.clicks);
      // Update all probabilities
      for (int i = 0; i < 32; i++) {
        module->probabilities[i] = module->props.probability;
      }
      break;
    case ui::Rotary::Green:
      module->props.randomness.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.steps.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.division.step(e.clicks);
      break;
    }
  }

  void TombolaSeqScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_steps(ctx);
  }

  void TombolaSeqScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("TOMBOLA", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Probability: {:.0f}%", module->props.probability.get() * 100), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Randomness: {:.0f}%", module->props.randomness.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Steps: {}", module->props.steps.get()), 10, 90);
    ctx.fillText(fmt::format("Division: 1/{}", module->props.division.get()), 10, 110);

    ctx.restore();
  }

  void TombolaSeqScreen::draw_steps(Canvas& ctx) {
    ctx.save();

    float stepsX = 160;
    float stepsY = 80;
    float stepsW = 150;
    float stepsH = 140;
    
    int numSteps = module->props.steps;
    float stepW = stepsW / numSteps;

    // Draw probability bars for each step
    for (int i = 0; i < numSteps; i++) {
      float x = stepsX + i * stepW;
      float prob = module->probabilities[i];
      float height = prob * stepsH;
      
      // Highlight current step
      if (i == module->position) {
        ctx.fillStyle(Colours::Blue.dim(0.5));
      } else {
        ctx.fillStyle(Colours::Green.dim(0.3 + prob * 0.5));
      }
      
      ctx.beginPath();
      ctx.rect(x + 1, stepsY + stepsH - height, stepW - 2, height);
      ctx.fill();
      
      // Draw border
      ctx.strokeStyle(Colours::White.dim(0.3));
      ctx.lineWidth(1.0f);
      ctx.stroke();
    }

    ctx.restore();
  }

} // namespace top1::modules
