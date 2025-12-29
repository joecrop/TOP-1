#include "finger-seq.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>
#include <algorithm>

namespace top1::modules {

  class FingerSeqScreen : public ui::ModuleScreen<FingerSeq> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_timeline(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<FingerSeq>::ModuleScreen;
  };

  FingerSeq::FingerSeq() :
    SequencerModule(&props),
    screen (new FingerSeqScreen(this)) {}

  void FingerSeq::display() {
    Globals::ui.display(*screen);
  }

  void FingerSeq::startRecording() {
    if (!props.overdub) {
      clearLoop();
    }
    loopPosition = 0;
    loopSampleCount = 0;
  }

  void FingerSeq::stopRecording() {
    // Release all active notes
    activeNotes.clear();
  }

  void FingerSeq::clearLoop() {
    recordedNotes.clear();
    activeNotes.clear();
  }

  float FingerSeq::quantizeTime(float time) {
    if (props.quantize == 0) return time;
    
    float gridSize = props.quantize;
    return std::round(time / gridSize) * gridSize;
  }

  void FingerSeq::process(const audio::ProcessData& data) {
    // Handle recording state changes
    if (props.recording && !wasRecording) {
      startRecording();
    } else if (!props.recording && wasRecording) {
      stopRecording();
    }
    wasRecording = props.recording;
    
    // Update loop position based on tempo
    float bpm = Globals::metronome.props.bpm;
    float beatsInLoop = props.loopLength / 4.0f;
    float secondsInLoop = (60.0f / bpm) * beatsInLoop;
    int samplesInLoop = static_cast<int>(secondsInLoop * Globals::samplerate);
    
    for (uint f = 0; f < data.nframes; f++) {
      loopSampleCount++;
      
      // Check loop boundary
      if (loopSampleCount >= samplesInLoop) {
        loopSampleCount = 0;
        loopPosition = 0;
        
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
      
      loopPosition = static_cast<float>(loopSampleCount) / samplesInLoop * beatsInLoop;
    }
    
    // Record incoming MIDI
    if (props.recording) {
      for (auto& evt : data.midi) {
        evt.match([&] (midi::NoteOnEvent& e) {
            if (e.channel == 0) {
              RecordedNote note;
              note.timeOffset = quantizeTime(loopPosition);
              note.note = e.key;
              note.velocity = e.velocity;
              note.duration = 0.25f; // Default duration
              recordedNotes.push_back(note);
              activeNotes.push_back(e.key);
            }
          },
          [&] (midi::NoteOffEvent& e) {
            if (e.channel == 0) {
              // Find the most recent note with this key and update duration
              for (auto it = recordedNotes.rbegin(); it != recordedNotes.rend(); ++it) {
                if (it->note == e.key && it->duration == 0.25f) {
                  float endTime = quantizeTime(loopPosition);
                  it->duration = endTime - it->timeOffset;
                  if (it->duration < 0) {
                    it->duration += beatsInLoop;
                  }
                  break;
                }
              }
              activeNotes.erase(std::remove(activeNotes.begin(), activeNotes.end(), e.key), 
                                activeNotes.end());
            }
          },
          [] (auto) {});
      }
    } else {
      // Playback recorded notes
      float lastPos = loopPosition - (1.0f / Globals::samplerate) * (bpm / 60.0f) * beatsInLoop;
      if (lastPos < 0) lastPos += beatsInLoop;
      
      for (auto& note : recordedNotes) {
        // Check if note should trigger
        if (lastPos <= note.timeOffset && loopPosition > note.timeOffset) {
          auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_ON, 0, nullptr, 0
          });
          noteOnEvt->key = note.note;
          noteOnEvt->velocity = note.velocity;
          const_cast<audio::ProcessData&>(data).midi.push_back(noteOnEvt);
          activeNotes.push_back(note.note);
        }
        
        // Check if note should end
        float endTime = note.timeOffset + note.duration;
        if (lastPos <= endTime && loopPosition > endTime) {
          auto* noteOffEvt = new midi::NoteOffEvent(midi::MidiEvent{
            midi::MidiEvent::NOTE_OFF, 0, nullptr, 0
          });
          noteOffEvt->key = note.note;
          noteOffEvt->velocity = 64;
          const_cast<audio::ProcessData&>(data).midi.push_back(noteOffEvt);
          activeNotes.erase(std::remove(activeNotes.begin(), activeNotes.end(), note.note), 
                           activeNotes.end());
        }
      }
    }
  }

  using namespace ui::drawing;

  bool FingerSeqScreen::keypress(ui::Key key) {
    return false;
  }

  void FingerSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      if (e.clicks != 0) {
        module->props.recording.set(!module->props.recording);
      }
      break;
    case ui::Rotary::Green:
      if (e.clicks != 0) {
        module->props.overdub.set(!module->props.overdub);
      }
      break;
    case ui::Rotary::White:
      module->props.loopLength.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.quantize.step(e.clicks);
      break;
    }
  }

  void FingerSeqScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_timeline(ctx);
  }

  void FingerSeqScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("FINGER", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(module->props.recording ? Colours::Red : Colours::Blue);
    ctx.fillText(module->props.recording ? "RECORDING" : "PLAYING", 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(module->props.overdub ? "OVERDUB: ON" : "OVERDUB: OFF", 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Loop: {} beats", module->props.loopLength.get()), 10, 90);
    
    const char* quantStr = "OFF";
    if (module->props.quantize == 0.25f) quantStr = "1/16";
    else if (module->props.quantize == 0.5f) quantStr = "1/8";
    else if (module->props.quantize == 1.0f) quantStr = "1/4";
    ctx.fillText(fmt::format("Quantize: {}", quantStr), 10, 110);

    ctx.fillStyle(Colours::Blue.dim(0.7));
    ctx.fillText(fmt::format("Notes: {}", module->recordedNotes.size()), 10, 130);

    ctx.restore();
  }

  void FingerSeqScreen::draw_timeline(Canvas& ctx) {
    ctx.save();

    float timelineX = 160;
    float timelineY = 60;
    float timelineW = 150;
    float timelineH = 160;
    
    float loopBeats = module->props.loopLength / 4.0f;

    // Draw timeline background
    ctx.fillStyle(Colours::White.dim(0.1));
    ctx.beginPath();
    ctx.rect(timelineX, timelineY, timelineW, timelineH);
    ctx.fill();

    // Draw recorded notes
    for (auto& note : module->recordedNotes) {
      float x = timelineX + (note.timeOffset / loopBeats) * timelineW;
      float width = (note.duration / loopBeats) * timelineW;
      float noteY = timelineY + (1.0f - (note.note % 48) / 48.0f) * timelineH;
      
      ctx.fillStyle(Colours::Green.dim(note.velocity / 127.0f));
      ctx.beginPath();
      ctx.rect(x, noteY, std::max(width, 2.0f), 3);
      ctx.fill();
    }

    // Draw playhead
    float playheadX = timelineX + (module->loopPosition / loopBeats) * timelineW;
    ctx.strokeStyle(module->props.recording ? Colours::Red : Colours::Blue);
    ctx.lineWidth(2.0f);
    ctx.beginPath();
    ctx.moveTo(playheadX, timelineY);
    ctx.lineTo(playheadX, timelineY + timelineH);
    ctx.stroke();

    ctx.restore();
  }

} // namespace top1::modules
