#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"
#include <cmath>

namespace top1::modules {

  class LFOScreen;  // Forward declaration

  /**
   * LFO Module - Low Frequency Oscillator for modulation
   * Inspired by OP-1's LFO types (tremolo, random, envelope, etc.)
   */
  class LFO : public Module {
  public:
    // LFO waveform types
    enum class Waveform {
      Sine = 0,
      Triangle,
      Square,
      Saw,
      Random,
      SampleHold
    };

    struct Props : public Properties {
      Property<float> rate      = {this, "rate", 1.0f, {0.1f, 20.0f, 0.1f}};     // Blue: Hz
      Property<float> depth     = {this, "depth", 0.5f, {0.0f, 1.0f, 0.01f}};    // Green: Amount
      Property<int> waveform    = {this, "wave", 0, {0, 5, 1}};                   // White: Shape
      Property<float> phase     = {this, "phase", 0.0f, {0.0f, 1.0f, 0.01f}};    // Red: Phase offset
      Property<bool> sync       = {this, "sync", false};                          // Sync to tempo
      Property<bool> retrigger  = {this, "retrig", true};                        // Retrigger on note
    } props;

    LFO();
    ~LFO() = default;

    void process(float deltaTime);  // Called each audio block
    float getValue() const { return currentValue; }  // Get current LFO value (-1 to 1)
    float getValueUnipolar() const { return (currentValue + 1.0f) * 0.5f; }  // Get value (0 to 1)
    void retrigger();  // Reset phase to 0
    void display();

    float currentPhase = 0.0f;  // 0 to 1 (public for screen access)

    float calculateWaveform(float phase, Waveform wave);

  private:
    float currentValue = 0.0f;  // -1 to 1
    float lastRandomValue = 0.0f;
    float nextRandomValue = 0.0f;
    std::unique_ptr<LFOScreen> screen;
  };

  /**
   * LFO Screen
   */
  class LFOScreen : public ui::ModuleScreen<LFO> {
  public:
    using ui::ModuleScreen<LFO>::ModuleScreen;

    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
