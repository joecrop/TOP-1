#pragma once

#include "core/modules/module.hpp"
#include "core/ui/canvas.hpp"

namespace top1::modules {

  /**
   * Input Selector - Selects the audio source for recording
   * 
   * Sources:
   * - INTERNAL: Internal synth output (default)
   * - EXTERNAL: External audio input from JACK
   * - MIXER: Mixer output (for resampling/bouncing)
   */
  class InputSelector {
  public:
    enum class Source {
      INTERNAL = 0,  // Internal synth
      EXTERNAL,      // External audio input
      MIXER,         // Mixer output
      COUNT
    };
    
    Source currentSource = Source::INTERNAL;
    Source previousSource = Source::INTERNAL;  // For auto-switching back
    bool autoSwitched = false;  // Track if we auto-switched for sequencer
    
    InputSelector() = default;
    
    // Cycle to next input source
    void nextSource() {
      int next = (static_cast<int>(currentSource) + 1) % static_cast<int>(Source::COUNT);
      currentSource = static_cast<Source>(next);
      autoSwitched = false;  // Manual selection clears auto-switch flag
    }
    
    // Get the current source
    Source source() const { return currentSource; }
    
    // Set source directly
    void setSource(Source s) {
      currentSource = s;
    }
    
    // Auto-switch to internal when sequencer starts playing
    void onSequencerStart() {
      if (currentSource != Source::INTERNAL) {
        previousSource = currentSource;
        currentSource = Source::INTERNAL;
        autoSwitched = true;
      }
    }
    
    // Auto-switch back when sequencer stops
    void onSequencerStop() {
      if (autoSwitched) {
        currentSource = previousSource;
        autoSwitched = false;
      }
    }
    
    // Get source name for display
    const char* sourceName() const {
      switch (currentSource) {
        case Source::INTERNAL: return "INT";
        case Source::EXTERNAL: return "EXT";
        case Source::MIXER: return "MIX";
        default: return "???";
      }
    }
    
    // Get icon character for display
    const char* sourceIcon() const {
      switch (currentSource) {
        case Source::INTERNAL: return "♪";   // Musical note for synth
        case Source::EXTERNAL: return "◉";   // Circle for external
        case Source::MIXER: return "≡";      // Bars for mixer
        default: return "?";
      }
    }
  };

} // top1::modules
