#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <vector>

namespace top1::modules {

  class ArpeggiatorScreen;

  /**
   * Arpeggiator - Note pattern generator synced to metronome
   */
  class Arpeggiator : public SequencerModule {
  public:
    static constexpr int MAX_HELD_NOTES = 16;

    struct Props : public Properties {
      Property<int> rate      = {this, "rate", 4, {1, 16, 1}};       // Blue: Rate (1/N note)
      Property<int> octaves   = {this, "octaves", 1, {1, 4, 1}};     // Green: Octave range
      Property<int> mode      = {this, "mode", 0, {0, 4, 1}};        // White: Arp mode
      Property<int> gate      = {this, "gate", 80, {10, 100, 5}};    // Red: Gate length %
      Property<bool> running  = {this, "running", true};
      Property<bool> hold     = {this, "hold", false};               // Latch mode
    } props;

    Arpeggiator();
    ~Arpeggiator() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;

    // Public for screen access
    std::vector<std::pair<int, int>> heldNotes;  // (note, velocity)
    int currentIndex = 0;
    bool noteIsOn = false;
    int lastTriggeredNote = -1;

  private:
    std::unique_ptr<ArpeggiatorScreen> screen;
    
    bool goingUp = true;  // For up-down mode
    float accumulator = 0.0f;
    
    void noteOn(int note, int velocity);
    void noteOff(int note);
    int getNextNote();
  };

  class ArpeggiatorScreen : public ui::ModuleScreen<Arpeggiator> {
  public:
    using ui::ModuleScreen<Arpeggiator>::ModuleScreen;
    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
