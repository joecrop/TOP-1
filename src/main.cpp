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
#include "modules/delay.hpp"
#include "modules/reverb.hpp"
#include "modules/chorus.hpp"
#include "modules/filter.hpp"
#include "modules/euclidean-seq.hpp"
#include "modules/pattern-seq.hpp"
#include "modules/arpeggiator-seq.hpp"
#include "modules/endless-seq.hpp"
#include "modules/tombola-seq.hpp"
#include "modules/finger-seq.hpp"
#include "modules/fm-synth.hpp"
#include "modules/pulse-synth.hpp"
#include "modules/wavetable-synth.hpp"
#include "modules/compressor.hpp"
#include "modules/bitcrusher.hpp"
#include "modules/distortion.hpp"
#include "modules/phaser.hpp"
#include "modules/string-synth.hpp"
#include "modules/vocoder-synth.hpp"
#include "modules/phase-synth.hpp"
#include "modules/digital-synth.hpp"
#include "modules/cluster-synth.hpp"
#include "modules/voltage-synth.hpp"
#include "modules/dna-synth.hpp"
#include "modules/grid-seq.hpp"
#include "modules/sketch-seq.hpp"
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

    Globals::synth.registerModule<NukeSynth>("Nuke");
    Globals::synth.registerModule<SuperSawSynth>("Super Saw");
    Globals::synth.registerModule<SynthSampler>("Sampler");
    Globals::synth.registerModule<FMSynth>("FM");
    Globals::synth.registerModule<PulseSynth>("Pulse");
    Globals::synth.registerModule<WavetableSynth>("Wavetable");
    Globals::synth.registerModule<StringSynth>("String");
    Globals::synth.registerModule<VocoderSynth>("Vocoder");
    Globals::synth.registerModule<PhaseSynth>("Phase");
    Globals::synth.registerModule<DigitalSynth>("Digital");
    Globals::synth.registerModule<ClusterSynth>("Cluster");
    Globals::synth.registerModule<VoltageSynth>("Voltage");
    Globals::synth.registerModule<DNASynth>("DNA");

    Globals::sequencer.registerModule<EuclideanSeq>("Euclidean");
    Globals::sequencer.registerModule<PatternSeq>("Pattern");
    Globals::sequencer.registerModule<ArpeggiatorSeq>("Arpeggiator");
    Globals::sequencer.registerModule<EndlessSeq>("Endless");
    Globals::sequencer.registerModule<TombolaSeq>("Tombola");
    Globals::sequencer.registerModule<FingerSeq>("Finger");
    Globals::sequencer.registerModule<GridSeq>("Grid");
    Globals::sequencer.registerModule<SketchSeq>("Sketch");

    Globals::effect.registerModule<DelayEffect>("Delay");
    Globals::effect.registerModule<ReverbEffect>("Reverb");
    Globals::effect.registerModule<ChorusEffect>("Chorus");
    Globals::effect.registerModule<FilterEffect>("Filter");
    Globals::effect.registerModule<CompressorEffect>("Compressor");
    Globals::effect.registerModule<BitcrusherEffect>("Bitcrusher");
    Globals::effect.registerModule<DistortionEffect>("Distortion");
    Globals::effect.registerModule<PhaserEffect>("Phaser");
    
    // Register effects for each track
    for (int i = 0; i < 4; i++) {
      Globals::trackEffects[i].registerModule<DelayEffect>("Delay");
      Globals::trackEffects[i].registerModule<ReverbEffect>("Reverb");
      Globals::trackEffects[i].registerModule<ChorusEffect>("Chorus");
      Globals::trackEffects[i].registerModule<FilterEffect>("Filter");
      Globals::trackEffects[i].registerModule<CompressorEffect>("Compressor");
      Globals::trackEffects[i].registerModule<BitcrusherEffect>("Bitcrusher");
      Globals::trackEffects[i].registerModule<DistortionEffect>("Distortion");
      Globals::trackEffects[i].registerModule<PhaserEffect>("Phaser");
    }

    Globals::events.preInit.runAll();
    Globals::init();
    Globals::events.postInit.runAll();

    Globals::jackAudio.startProcess();

    Globals::notifyExit.wait(lock);

  } catch (const char* e) {
    LOGF << e;
    LOGI << "Exitting";
    Globals::events.preExit.runAll();
    Globals::ui.exit();
    Globals::mixer.exit();
    Globals::tapedeck.exit();
    Globals::jackAudio.exit();
    Globals::dataFile.write();
    Globals::events.postExit.runAll();
  }

  LOGI << "Exitting";
  Globals::events.preExit.runAll();
  Globals::ui.exit();
  Globals::mixer.exit();
  Globals::tapedeck.exit();
  Globals::jackAudio.exit();
  Globals::dataFile.write();
  Globals::events.postExit.runAll();
  return 0;
}
