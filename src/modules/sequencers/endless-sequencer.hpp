#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"
#include "core/audio/midi.hpp"
#include <array>
#include <random>

namespace top1::modules {

  class EndlessSeqScreen;

  /**
   * Endless Sequencer - Generative pattern sequencer inspired by OP-1's Endless
   * Generates evolving patterns based on probability and rules
   */
  class EndlessSequencer : public SequencerModule {
  public:
    static constexpr int NUM_STEPS = 16;
    static constexpr int NUM_VOICES = 4;

    struct Voice {
      int note = 60;
      int velocity = 100;
      float probability = 0.5f;
      bool active = false;
    };

    struct Props : public Properties {
      Property<int> density     = {this, "density", 50, {0, 100, 5}};      // Blue: How many notes
      Property<int> length      = {this, "length", 8, {1, 16, 1}};         // Green: Pattern length
      Property<int> variance    = {this, "variance", 30, {0, 100, 5}};     // White: Randomness
      Property<int> noteRange   = {this, "range", 12, {1, 36, 1}};         // Red: Note range (semitones)
      Property<int> rootNote    = {this, "root", 48, {24, 84, 1}};
      Property<int> scale       = {this, "scale", 0, {0, 5, 1}};           // Scale type
      Property<bool> running    = {this, "running", false};
      Property<bool> evolve     = {this, "evolve", true};                  // Pattern evolves over time
    } props;

    // Current pattern state
    std::array<std::array<Voice, NUM_VOICES>, NUM_STEPS> pattern;
    int currentStep = 0;
    
    EndlessSequencer();
    ~EndlessSequencer() = default;

    void process(const audio::ProcessData& data) override;
    void display() override;
    
    void generatePattern();
    void evolvePattern();
    void mutateStep(int step);
    int quantizeToScale(int note);

  private:
    std::unique_ptr<EndlessSeqScreen> screen;
    float accumulator = 0.0f;
    int evolutionCounter = 0;
    std::mt19937 rng;
    
    // Scale definitions
    static constexpr int scaleMajor[7] = {0, 2, 4, 5, 7, 9, 11};
    static constexpr int scaleMinor[7] = {0, 2, 3, 5, 7, 8, 10};
    static constexpr int scalePentatonic[5] = {0, 2, 4, 7, 9};
    static constexpr int scaleBlues[6] = {0, 3, 5, 6, 7, 10};
    static constexpr int scaleDorian[7] = {0, 2, 3, 5, 7, 9, 10};
    static constexpr int scaleChromatic[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  };

  class EndlessSeqScreen : public ui::ModuleScreen<EndlessSequencer> {
  public:
    using ui::ModuleScreen<EndlessSequencer>::ModuleScreen;
    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
