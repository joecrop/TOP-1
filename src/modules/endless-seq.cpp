#include "endless-seq.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  class EndlessSeqScreen : public ui::ModuleScreen<EndlessSeq> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_pattern(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<EndlessSeq>::ModuleScreen;
  };

  EndlessSeq::EndlessSeq() :
    SequencerModule(&props),
    screen (new EndlessSeqScreen(this)),
    gen(rd()) {
    
    // Initialize with random pattern
    for (int i = 0; i < 32; i++) {
      currentPattern.notes[i] = generateNoteForStep(i);
      currentPattern.velocities[i] = 0.5f + (gen() % 50) / 100.0f;
      currentPattern.active[i] = (gen() % 100) < (props.density * 100);
    }
    nextPattern = currentPattern;
  }

  void EndlessSeq::display() {
    Globals::ui.display(*screen);
  }

  int EndlessSeq::generateNoteForStep(int step) {
    int baseOctave = 4;
    int octaveRange = props.octaves;
    int octave = baseOctave + (gen() % octaveRange);
    int scaleNote = scaleIntervals[gen() % 7];
    return octave * 12 + scaleNote;
  }

  void EndlessSeq::evolvePattern() {
    // Evolve pattern based on evolution parameter
    for (int i = 0; i < beatsPerBar; i++) {
      float evolveChance = props.evolution;
      
      if ((gen() % 100) / 100.0f < evolveChance) {
        // Mutate this step
        if ((gen() % 2) == 0) {
          // Change note
          nextPattern.notes[i] = generateNoteForStep(i);
        }
        
        if ((gen() % 3) == 0) {
          // Change velocity
          nextPattern.velocities[i] = 0.3f + (gen() % 70) / 100.0f;
        }
        
        if ((gen() % 4) == 0) {
          // Toggle active state based on density
          nextPattern.active[i] = (gen() % 100) < (props.density * 100);
        }
      }
    }
  }

  void EndlessSeq::process(const audio::ProcessData& data) {
    float bpm = Globals::metronome.props.bpm;
    float beatsPerStep = 4.0f / beatsPerBar;
    float secondsPerStep = (60.0f / bpm) * beatsPerStep / props.rate;
    int samplesPerStep = static_cast<int>(secondsPerStep * Globals::samplerate);
    
    for (uint f = 0; f < data.nframes; f++) {
      samplesAccumulator++;
      
      if (samplesAccumulator >= samplesPerStep) {
        samplesAccumulator = 0;
        
        // Evolution at bar boundaries
        if (position == 0) {
          evolvePattern();
          currentPattern = nextPattern;
        }
        
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
        
        // Note on if step is active
        if (currentPattern.active[position]) {
          int note = currentPattern.notes[position];
          float velocity = currentPattern.velocities[position];
          
          auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_ON, 0, nullptr, 0
          });
          noteOnEvt->key = note;
          noteOnEvt->velocity = static_cast<uint8_t>(velocity * 127);
          const_cast<audio::ProcessData&>(data).midi.push_back(noteOnEvt);
          currentNote = note;
        }
        
        position = (position + 1) % beatsPerBar;
      }
    }
  }

  using namespace ui::drawing;

  bool EndlessSeqScreen::keypress(ui::Key key) {
    return false;
  }

  void EndlessSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.density.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.evolution.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.scale.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.rate.step(e.clicks);
      break;
    }
  }

  void EndlessSeqScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_pattern(ctx);
  }

  void EndlessSeqScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("ENDLESS", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Density: {:.0f}%", module->props.density.get() * 100), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Evolution: {:.0f}%", module->props.evolution.get() * 100), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Rate: {:.1f}x", module->props.rate.get()), 10, 90);

    ctx.restore();
  }

  void EndlessSeqScreen::draw_pattern(Canvas& ctx) {
    ctx.save();

    float gridX = 160;
    float gridY = 60;
    float gridW = 150;
    float gridH = 160;
    float stepW = gridW / module->beatsPerBar;

    // Draw grid
    for (int i = 0; i < module->beatsPerBar; i++) {
      float x = gridX + i * stepW;
      
      // Highlight current position
      if (i == module->position) {
        ctx.fillStyle(Colours::Blue.dim(0.3));
        ctx.beginPath();
        ctx.rect(x, gridY, stepW, gridH);
        ctx.fill();
      }
      
      // Draw step if active
      if (module->currentPattern.active[i]) {
        float noteHeight = (module->currentPattern.notes[i] % 24) / 24.0f * gridH;
        float velocity = module->currentPattern.velocities[i];
        
        ctx.fillStyle(Colours::Green.dim(0.5 + velocity * 0.5));
        ctx.beginPath();
        ctx.rect(x + 1, gridY + gridH - noteHeight, stepW - 2, 4);
        ctx.fill();
      }
      
      // Draw grid lines
      ctx.strokeStyle(Colours::White.dim(0.2));
      ctx.lineWidth(1.0f);
      ctx.beginPath();
      ctx.moveTo(x, gridY);
      ctx.lineTo(x, gridY + gridH);
      ctx.stroke();
    }

    ctx.restore();
  }

} // namespace top1::modules
