#include "sketch-seq.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>
#include <algorithm>

namespace top1::modules {

  class SketchSeqScreen : public ui::ModuleScreen<SketchSeq> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_sketch(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<SketchSeq>::ModuleScreen;
  };

  SketchSeq::SketchSeq() :
    SequencerModule(&props),
    screen (new SketchSeqScreen(this)) {
    
    // Create simple demo pattern
    for (int i = 0; i < 8; i++) {
      DrawnNote note;
      note.position = i / 8.0f;
      note.note = props.baseNote + (i % 12);
      note.duration = 0.1f;
      note.velocity = 0.7f + (i % 3) * 0.1f;
      drawnNotes.push_back(note);
    }
  }

  void SketchSeq::display() {
    Globals::ui.display(*screen);
  }

  void SketchSeq::clearSketch() {
    drawnNotes.clear();
    activeNotes.clear();
  }

  void SketchSeq::process(const audio::ProcessData& data) {
    float bpm = Globals::metronome.props.bpm;
    float beatsInLoop = props.loopLength / 4.0f;
    float secondsInLoop = (60.0f / bpm) * beatsInLoop;
    int samplesInLoop = static_cast<int>(secondsInLoop * Globals::samplerate);
    
    for (uint f = 0; f < data.nframes; f++) {
      loopSampleCount++;
      
      // Check loop boundary
      if (loopSampleCount >= samplesInLoop) {
        loopSampleCount = 0;
        
        // Release all notes at loop boundary
        for (int note : activeNotes) {
          auto* noteOffEvt = new midi::NoteOffEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_OFF, 0, nullptr, 0
          });
          noteOffEvt->key = note;
          noteOffEvt->velocity = 64;
          const_cast<audio::ProcessData&>(data).midi.push_back(noteOffEvt);
        }
        activeNotes.clear();
      }
      
      lastLoopPosition = loopPosition;
      loopPosition = static_cast<float>(loopSampleCount) / samplesInLoop;
      
      // Check for note triggers
      for (auto& drawnNote : drawnNotes) {
        // Trigger note
        if (lastLoopPosition <= drawnNote.position && loopPosition > drawnNote.position) {
          auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_ON, 0, nullptr, 0
          });
          noteOnEvt->key = drawnNote.note;
          noteOnEvt->velocity = static_cast<uint8_t>(drawnNote.velocity * 127);
          const_cast<audio::ProcessData&>(data).midi.push_back(noteOnEvt);
          activeNotes.push_back(drawnNote.note);
        }
        
        // Release note
        float endPos = drawnNote.position + drawnNote.duration;
        if (endPos > 1.0f) endPos -= 1.0f;
        
        bool shouldRelease = false;
        if (drawnNote.position + drawnNote.duration <= 1.0f) {
          shouldRelease = (lastLoopPosition <= endPos && loopPosition > endPos);
        } else {
          shouldRelease = (lastLoopPosition <= endPos || loopPosition > endPos);
        }
        
        if (shouldRelease) {
          auto* noteOffEvt = new midi::NoteOffEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_OFF, 0, nullptr, 0
          });
          noteOffEvt->key = drawnNote.note;
          noteOffEvt->velocity = 64;
          const_cast<audio::ProcessData&>(data).midi.push_back(noteOffEvt);
          activeNotes.erase(std::remove(activeNotes.begin(), activeNotes.end(), drawnNote.note), 
                           activeNotes.end());
        }
      }
    }
  }

  using namespace ui::drawing;

  bool SketchSeqScreen::keypress(ui::Key key) {
    return false;
  }

  void SketchSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.loopLength.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.quantize.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.noteRange.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.baseNote.step(e.clicks);
      break;
    }
  }

  void SketchSeqScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_sketch(ctx);
  }

  void SketchSeqScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("SKETCH", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Loop: {} beats", module->props.loopLength.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    const char* quantStr = "OFF";
    if (module->props.quantize == 0.25f) quantStr = "1/16";
    else if (module->props.quantize == 0.5f) quantStr = "1/8";
    else if (module->props.quantize == 1.0f) quantStr = "1/4";
    ctx.fillText(fmt::format("Quantize: {}", quantStr), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Range: {} st", module->props.noteRange.get()), 10, 90);
    ctx.fillText(fmt::format("Notes: {}", module->drawnNotes.size()), 10, 110);

    ctx.restore();
  }

  void SketchSeqScreen::draw_sketch(Canvas& ctx) {
    ctx.save();

    float sketchX = 160;
    float sketchY = 50;
    float sketchW = 150;
    float sketchH = 170;

    // Draw background
    ctx.fillStyle(Colours::White.dim(0.05));
    ctx.beginPath();
    ctx.rect(sketchX, sketchY, sketchW, sketchH);
    ctx.fill();

    // Draw note range lines
    int noteRange = module->props.noteRange;
    for (int i = 0; i <= noteRange; i += 12) {
      float y = sketchY + sketchH - (i / static_cast<float>(noteRange)) * sketchH;
      ctx.beginPath();
      ctx.moveTo(sketchX, y);
      ctx.lineTo(sketchX + sketchW, y);
      ctx.strokeStyle(Colours::White.dim(0.1));
      ctx.lineWidth(1.0f);
      ctx.stroke();
    }

    // Draw drawn notes
    for (auto& note : module->drawnNotes) {
      float x = sketchX + note.position * sketchW;
      float noteY = sketchY + sketchH - 
                    ((note.note - module->props.baseNote) / static_cast<float>(noteRange)) * sketchH;
      float width = note.duration * sketchW;
      
      ctx.fillStyle(Colours::Green.dim(0.3 + note.velocity * 0.7));
      ctx.beginPath();
      ctx.rect(x, noteY - 2, std::max(width, 2.0f), 4);
      ctx.fill();
    }

    // Draw playhead
    float playheadX = sketchX + module->loopPosition * sketchW;
    ctx.strokeStyle(Colours::Blue);
    ctx.lineWidth(2.0f);
    ctx.beginPath();
    ctx.moveTo(playheadX, sketchY);
    ctx.lineTo(playheadX, sketchY + sketchH);
    ctx.stroke();

    ctx.restore();
  }

} // namespace top1::modules
