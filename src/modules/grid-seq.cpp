#include "grid-seq.hpp"

#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <cmath>

namespace top1::modules {

  class GridSeqScreen : public ui::ModuleScreen<GridSeq> {

    void draw_text(ui::drawing::Canvas& ctx);
    void draw_grid(ui::drawing::Canvas& ctx);

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;

  public:
    using ui::ModuleScreen<GridSeq>::ModuleScreen;
  };

  GridSeq::GridSeq() :
    SequencerModule(&props),
    screen (new GridSeqScreen(this)) {
    
    // Initialize grid
    for (auto& row : grid) {
      row.fill(false);
    }
    for (auto& row : velocities) {
      row.fill(0.8f);
    }
    activeNotes.fill(-1);
    
    // Create simple pattern
    for (int i = 0; i < 16; i++) {
      if (i % 4 == 0) {
        grid[0][i] = true;
        grid[4][i] = true;
      }
    }
  }

  void GridSeq::display() {
    Globals::ui.display(*screen);
  }

  void GridSeq::process(const audio::ProcessData& data) {
    float bpm = Globals::metronome.props.bpm;
    float beatsPerDiv = 4.0f / props.division;
    float secondsPerDiv = (60.0f / bpm) * beatsPerDiv;
    
    // Apply swing
    if (swingOffset) {
      secondsPerDiv *= (1.0f + props.swing);
    }
    
    int samplesPerDiv = static_cast<int>(secondsPerDiv * Globals::samplerate);
    
    for (uint f = 0; f < data.nframes; f++) {
      samplesAccumulator++;
      
      if (samplesAccumulator >= samplesPerDiv) {
        samplesAccumulator = 0;
        swingOffset = !swingOffset;
        
        // Note offs for previous column
        for (int row = 0; row < props.rows; row++) {
          if (activeNotes[row] >= 0) {
            auto* noteOffEvt = new midi::NoteOffEvent(midi::MidiEvent{
              midi::MidiEvent::NOTE_OFF, 0, nullptr, 0
            });
            noteOffEvt->key = activeNotes[row];
            noteOffEvt->velocity = 64;
            const_cast<audio::ProcessData&>(data).midi.push_back(noteOffEvt);
            activeNotes[row] = -1;
          }
        }
        
        // Note ons for current column
        for (int row = 0; row < props.rows; row++) {
          if (grid[row][currentColumn]) {
            int note = 36 + (props.rows - 1 - row) * 2;
            float velocity = velocities[row][currentColumn];
            
            auto* noteOnEvt = new midi::NoteOnEvent(midi::MidiEvent{
              midi::MidiEvent::NOTE_ON, 0, nullptr, 0
            });
            noteOnEvt->key = note;
            noteOnEvt->velocity = static_cast<uint8_t>(velocity * 127);
            const_cast<audio::ProcessData&>(data).midi.push_back(noteOnEvt);
            activeNotes[row] = note;
          }
        }
        
        currentColumn = (currentColumn + 1) % props.columns;
      }
    }
  }

  using namespace ui::drawing;

  bool GridSeqScreen::keypress(ui::Key key) {
    return false;
  }

  void GridSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.rows.step(e.clicks);
      break;
    case ui::Rotary::Green:
      module->props.columns.step(e.clicks);
      break;
    case ui::Rotary::White:
      module->props.division.step(e.clicks);
      break;
    case ui::Rotary::Red:
      module->props.swing.step(e.clicks);
      break;
    }
  }

  void GridSeqScreen::draw(Canvas &ctx) {
    draw_text(ctx);
    draw_grid(ctx);
  }

  void GridSeqScreen::draw_text(Canvas& ctx) {
    ctx.save();
    ctx.font(Fonts::Norm);
    ctx.font(16.f);

    ctx.font(Fonts::Bold);
    ctx.font(20.f);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("GRID", 160, 20);

    ctx.font(Fonts::Norm);
    ctx.font(14.f);
    ctx.textAlign(TextAlign::Left, TextAlign::Top);

    ctx.fillStyle(Colours::Blue);
    ctx.fillText(fmt::format("Rows: {}", module->props.rows.get()), 10, 50);

    ctx.fillStyle(Colours::Green);
    ctx.fillText(fmt::format("Columns: {}", module->props.columns.get()), 10, 70);

    ctx.fillStyle(Colours::White);
    ctx.fillText(fmt::format("Division: 1/{}", module->props.division.get()), 10, 90);
    ctx.fillText(fmt::format("Swing: {:.0f}%", module->props.swing.get() * 100), 10, 110);

    ctx.restore();
  }

  void GridSeqScreen::draw_grid(Canvas& ctx) {
    ctx.save();

    float gridX = 160;
    float gridY = 50;
    float gridW = 150;
    float gridH = 170;
    
    int rows = module->props.rows;
    int cols = module->props.columns;
    float cellW = gridW / cols;
    float cellH = gridH / rows;

    // Draw cells
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        float x = gridX + col * cellW;
        float y = gridY + row * cellH;
        
        // Highlight current column
        if (col == module->currentColumn) {
          ctx.fillStyle(Colours::Blue.dim(0.3));
          ctx.beginPath();
          ctx.rect(x, y, cellW, cellH);
          ctx.fill();
        }
        
        // Draw active cells
        if (module->grid[row][col]) {
          float velocity = module->velocities[row][col];
          ctx.fillStyle(Colours::Green.dim(0.3 + velocity * 0.7));
          ctx.beginPath();
          ctx.rect(x + 1, y + 1, cellW - 2, cellH - 2);
          ctx.fill();
        }
        
        // Draw grid lines
        ctx.strokeStyle(Colours::White.dim(0.2));
        ctx.lineWidth(1.0f);
        ctx.beginPath();
        ctx.rect(x, y, cellW, cellH);
        ctx.stroke();
      }
    }

    ctx.restore();
  }

} // namespace top1::modules
