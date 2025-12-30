#include <mutex>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

#include "core/audio/jack.hpp"
#include "core/audio/midi.hpp"
#include "core/ui/mainui.hpp"
#include "modules/tapedeck.hpp"
#include "modules/mixer.hpp"
#include "modules/super-saw-synth.hpp"
#include "modules/simple-drums.hpp"
#include "modules/drum-sampler.hpp"
#include "modules/synth-sampler.hpp"
#include "modules/nuke.hpp"
#include "modules/fm-synth.hpp"
#include "modules/pulse-synth.hpp"
#include "modules/wavetable-synth.hpp"
#include "modules/string-synth.hpp"
#include "modules/cluster-synth.hpp"
#include "modules/digital-synth.hpp"
#include "modules/phase-synth.hpp"
#include "modules/effects/delay.hpp"
#include "modules/effects/reverb.hpp"
#include "modules/effects/chorus.hpp"
#include "modules/effects/filter.hpp"
#include "modules/effects/drive.hpp"
#include "modules/effects/phaser.hpp"
#include "modules/effects/cwo.hpp"
#include "modules/effects/nitro.hpp"
#include "modules/effects/spring.hpp"
#include "modules/sequencers/pattern-sequencer.hpp"
#include "modules/sequencers/arpeggiator.hpp"
#include "modules/sequencers/endless-sequencer.hpp"
#include "core/globals.hpp"

int main(int argc, char *argv[]) {
  using namespace top1;
  try {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, (top1::Globals::data_dir / "log.txt").c_str())
      .addAppender(&consoleAppender);
    LOGI << "LOGGING NOW";

    midi::generateFreqTable(440);
    std::mutex mut;
    std::unique_lock lock (mut);

    using namespace modules;

    Globals::drums.registerModule<DrumSampler>("Sampler");
    Globals::drums.registerModule<SimpleDrumsModule>("Additive Drums");

    // Synth engines
    Globals::synth.registerModule<NukeSynth>("Nuke");
    Globals::synth.registerModule<SuperSawSynth>("Super Saw");
    Globals::synth.registerModule<SynthSampler>("Sampler");
    Globals::synth.registerModule<FMSynth>("FM");
    Globals::synth.registerModule<PulseSynth>("Pulse");
    Globals::synth.registerModule<WavetableSynth>("Wavetable");
    Globals::synth.registerModule<StringSynth>("String");
    Globals::synth.registerModule<ClusterSynth>("Cluster");
    Globals::synth.registerModule<DigitalSynth>("Digital");
    Globals::synth.registerModule<PhaseSynth>("Phase");

    // Register effects for master bus
    Globals::effect.registerModule<DelayEffect>("Delay");
    Globals::effect.registerModule<ReverbEffect>("Reverb");
    Globals::effect.registerModule<ChorusEffect>("Chorus");
    Globals::effect.registerModule<FilterEffect>("Filter");
    Globals::effect.registerModule<DriveEffect>("Drive");
    Globals::effect.registerModule<PhaserEffect>("Phaser");
    Globals::effect.registerModule<CWOEffect>("CWO");
    Globals::effect.registerModule<NitroEffect>("Nitro");
    Globals::effect.registerModule<SpringEffect>("Spring");

    // Register effects for each track (1 effect slot per track)
    for (int i = 0; i < 4; i++) {
      Globals::trackEffects[i].registerModule<DelayEffect>("Delay");
      Globals::trackEffects[i].registerModule<ReverbEffect>("Reverb");
      Globals::trackEffects[i].registerModule<ChorusEffect>("Chorus");
      Globals::trackEffects[i].registerModule<FilterEffect>("Filter");
      Globals::trackEffects[i].registerModule<DriveEffect>("Drive");
      Globals::trackEffects[i].registerModule<PhaserEffect>("Phaser");
      Globals::trackEffects[i].registerModule<CWOEffect>("CWO");
      Globals::trackEffects[i].registerModule<NitroEffect>("Nitro");
      Globals::trackEffects[i].registerModule<SpringEffect>("Spring");
    }

    // Register sequencers
    Globals::sequencer.registerModule<PatternSequencer>("Pattern");
    Globals::sequencer.registerModule<Arpeggiator>("Arpeggiator");
    Globals::sequencer.registerModule<EndlessSequencer>("Endless");

    Globals::events.preInit.runAll();
    Globals::init();
    Globals::events.postInit.runAll();

    Globals::jackAudio.startProcess();

    Globals::notifyExit.wait(lock);

  } catch (const char* e) {
    LOGF << e;
    LOGI << "Exitting";
    Globals::events.preExit.runAll();
    Globals::jackAudio.exit();  // Stop audio processing first
    Globals::ui.exit();
    Globals::mixer.exit();
    Globals::tapedeck.exit();
    Globals::dataFile.write();
    Globals::events.postExit.runAll();
  }

  LOGI << "Exitting";
  Globals::events.preExit.runAll();
  Globals::jackAudio.exit();  // Stop audio processing first
  Globals::ui.exit();
  Globals::mixer.exit();
  Globals::tapedeck.exit();
  Globals::dataFile.write();
  Globals::events.postExit.runAll();
  return 0;
}
