#pragma once

#include "core/modules/module.hpp"
#include "core/ui/module-ui.hpp"
#include "core/audio/processor.hpp"

namespace top1::modules {

  /**
   * Base class for audio effects
   * Each effect can be used per-track or as master effect
   */
  class EffectBase : public EffectModule {
  public:
    struct Props : public Properties {
      Property<bool> bypassed = {this, "bypassed", false};
    };

    EffectBase(Props* props) : EffectModule(props), effectProps(props) {}
    virtual ~EffectBase() = default;

    // Toggle bypass state
    void toggleBypass() {
      effectProps->bypassed = !effectProps->bypassed.get();
    }

    bool isBypassed() const {
      return effectProps->bypassed.get();
    }

    // Process audio - subclasses implement processEffect
    void process(const audio::ProcessData& data) override {
      if (!effectProps->bypassed.get()) {
        processEffect(data);
      }
      // If bypassed, audio passes through unchanged
    }

    // Subclasses implement this
    virtual void processEffect(const audio::ProcessData& data) = 0;

  protected:
    Props* effectProps;
  };

  /**
   * Per-track effect slot
   * Each track has one of these to hold its effect
   */
  struct TrackEffectSlot {
    std::unique_ptr<EffectBase> effect;
    bool bypassed = false;

    void process(gsl::span<float> audio, uint nframes) {
      if (effect && !bypassed) {
        // Create minimal ProcessData for single-track processing
        audio::ProcessData data;
        data.nframes = nframes;
        data.audio.proc = audio;
        effect->process(data);
      }
    }

    void toggleBypass() {
      bypassed = !bypassed;
    }
  };

} // namespace top1::modules
