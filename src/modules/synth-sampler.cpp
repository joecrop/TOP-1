#include "modules/synth-sampler.hpp"

#include <algorithm>

#include "util/math.hpp"
#include "core/ui/drawing.hpp"
#include "core/ui/icons.hpp"
#include "core/globals.hpp"
#include "util/soundfile.hpp"

namespace top1::modules {

  SynthSampler::SynthSampler() :
    SynthModule(&props),
    maxSampleSize (16 * Globals::samplerate),
    sampleData (maxSampleSize),
    recordBuffer (maxSampleSize),
    editScreen (new SynthSampleScreen(this)) {

    Globals::events.samplerateChanged.add([&] (uint sr) {
        maxSampleSize = 16 * sr;
        sampleSpeed = sampleSampleRate / float(sr);
      });

  }

  fs::path SynthSampler::samplePath(std::string name) {
    auto wav_path = Globals::data_dir / "samples" / "synth" / (name + ".wav");
    if (!fs::exists(wav_path)) {
      return Globals::data_dir / "samples" / "synth" / (name + ".aiff");
    }
    return wav_path;
  }

  void SynthSampler::process(const audio::ProcessData& data) {
    // Handle recording from audio input
    if (isRecording) {
      for (uint i = 0; i < data.nframes && recordPosition < maxSampleSize; ++i) {
        recordBuffer[recordPosition++] = data.audio.input[i];
      }
      // Auto-stop if buffer is full
      if (recordPosition >= maxSampleSize) {
        stopRecording();
      }
    }

    // Current played key for pitch calculation (stored per note-on)
    static int currentPlayedKey = 60;
    float pitchRatio = 1.0f;

    for (auto &&nEvent : data.midi) {
      nEvent.match([&] (midi::NoteOnEvent& e) {
          if (e.channel == 0) {
            // If recording is armed, start recording with this key as root
            if (recordArmed) {
              startRecording(e.key);
              return;
            }
            // Otherwise, play the sample
            currentPlayedKey = e.key;
            props.playProgress = (props.fwd()) ? 0 : props.length() - 1;
            props.trigger = true;
          }
        }, [] (auto) {});
    }

    // Calculate pitch ratio: 2^((playedKey - rootKey) / 12)
    pitchRatio = std::pow(2.0f, (currentPlayedKey - props.rootKey.get()) / 12.0f);
    float playSpeed = props.speed * sampleSpeed * pitchRatio;

    // Process audio playback
    if (props.playProgress >= 0 && playSpeed > 0 && !isRecording) {
      if (props.fwd()) {
        if (props.loop() && props.trigger) {
          for(uint i = 0; i < data.nframes; ++i) {
            int idx = props.in + static_cast<int>(props.playProgress);
            if (idx >= 0 && idx < static_cast<int>(sampleData.size())) {
              data.audio.proc[i] += sampleData[idx];
            }
            props.playProgress += playSpeed;
            if (props.playProgress >= props.length()) {
              props.playProgress = 0;
            }
          }
        } else {
          int frms = std::min<int>(data.nframes, props.length() - props.playProgress);
          for(int i = 0; i < frms; ++i) {
            int idx = props.in + static_cast<int>(props.playProgress);
            if (idx >= 0 && idx < static_cast<int>(sampleData.size())) {
              data.audio.proc[i] += sampleData[idx];
            }
            props.playProgress += playSpeed;
          }
          if (props.playProgress >= props.length()) {
            props.playProgress = -1;
          }
        }
      } else {
        if (props.loop() && props.trigger) {
          for(uint i = 0; i < data.nframes; ++i) {
            int idx = props.in + static_cast<int>(props.playProgress);
            if (idx >= 0 && idx < static_cast<int>(sampleData.size())) {
              data.audio.proc[i] += sampleData[idx];
            }
            props.playProgress -= playSpeed;
            if (props.playProgress < 0) {
              props.playProgress = props.length() - 1;
            }
          }
        } else {
          int frms = std::min<int>(data.nframes, static_cast<int>(props.playProgress));
          for(int i = 0; i < frms; ++i) {
            int idx = props.in + static_cast<int>(props.playProgress);
            if (idx >= 0 && idx < static_cast<int>(sampleData.size())) {
              data.audio.proc[i] += sampleData[idx];
            }
            props.playProgress -= playSpeed;
          }
        }
      }
    }

    for (auto &&nEvent : data.midi) {
      nEvent.match([&] (midi::NoteOffEvent& e) {
          if (e.channel == 0) {
            // Stop recording on key release
            if (isRecording) {
              stopRecording();
              return;
            }
            props.trigger = false;
            if (props.stop()) {
              props.playProgress = -1;
            }
          }
        }, [] (auto) {});
    };
  }

  void SynthSampler::display() {
    Globals::ui.display(*editScreen);
  }

  void SynthSampler::load() {

    auto path = samplePath(props.sampleName);
    std::size_t rs = 0;
    if (!(path.empty() || props.sampleName.get().empty())) {
      SoundFile sf;
      sf.open(path);
      rs = std::min<int>(maxSampleSize, sf.length());
      sampleData.resize(rs);
      sf.read_samples(sampleData.data(), rs);

      sampleSampleRate = sf.info.samplerate;
      sampleSpeed = sampleSampleRate / float(Globals::samplerate);
      if (sf.length() == 0) LOGD << "Empty sample file";
    } else {
      sampleData.resize(0);
      LOGI << "Empty sampleName";
    }

    props.in.mode.max = rs;
    props.out.mode.max = rs;

    // Auto assign voices
    if (props.in < 0 || props.out >= rs) {
      props.in = 0;
      props.out = rs;
    }

    auto &mwf = editScreen->mainWF;
    mwf->clear();
    for (auto &&s : sampleData) {
      mwf->addFrame(s);
    }

    auto &wf = editScreen->topWF;
    wf->clear();
    for (auto &&s : sampleData) {
      wf->addFrame(s);
    }
    editScreen->topWFW.viewRange = {0, wf->size() - 1};
  }

  void SynthSampler::init() {
    load();
  }

  void SynthSampler::armRecording() {
    recordArmed = true;
    isRecording = false;
    recordPosition = 0;
    LOGI << "Recording armed - press a key to start recording";
  }

  void SynthSampler::startRecording(int midiKey) {
    if (!recordArmed) return;
    
    recordArmed = false;
    isRecording = true;
    recordPosition = 0;
    props.rootKey = midiKey;  // Store the key pressed as root key
    recordBuffer.resize(maxSampleSize);
    LOGI << "Recording started at root key: " << noteName(midiKey);
  }

  void SynthSampler::stopRecording() {
    if (!isRecording) return;
    
    isRecording = false;
    
    // Copy recorded data to sample data
    sampleData.resize(recordPosition);
    for (size_t i = 0; i < recordPosition; ++i) {
      sampleData[i] = recordBuffer[i];
    }
    
    // Update sample rate to current rate
    sampleSampleRate = Globals::samplerate;
    sampleSpeed = 1.0f;
    
    // Reset in/out points
    props.in.mode.max = recordPosition;
    props.out.mode.max = recordPosition;
    props.in = 0;
    props.out = recordPosition;
    
    // Rebuild waveforms
    auto &mwf = editScreen->mainWF;
    mwf->clear();
    for (size_t i = 0; i < sampleData.size(); ++i) {
      mwf->addFrame(sampleData[i]);
    }

    auto &wf = editScreen->topWF;
    wf->clear();
    for (size_t i = 0; i < sampleData.size(); ++i) {
      wf->addFrame(sampleData[i]);
    }
    editScreen->topWFW.viewRange = {0, wf->size() > 0 ? wf->size() - 1 : 0};
    
    LOGI << "Recording stopped. Captured " << recordPosition << " samples at root key: " << noteName(props.rootKey);
  }

  std::string SynthSampler::noteName(int midiNote) {
    static const char* noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    int octave = (midiNote / 12) - 1;
    int note = midiNote % 12;
    return std::string(noteNames[note]) + std::to_string(octave);
  }

} // top1::module

namespace top1::ui::drawing {

  const static Size topWFsize = {210, 20};
  const static Point topWFpos = {60, 20};
  const static Size arrowSize = {20, 20};
  const static Point arrowPos = {280, 20};
  const static Size pitchSize = {30, 20};
  const static Point pitchPos = {20, 40};
  const static Size mainWFsize = {280, 170};
  const static Point mainWFpos = {20, 50};

  namespace Colours {

    const Colour TopWF = Blue.dim(0.2);
    const Colour TopWFCur = Blue.brighten(0.5);
    const Colour TopWFActive = White;
    const Colour WFGrid = 0x303040;
  }
} // top1::ui::drawing

namespace top1::modules {

  /****************************************/
  /* SampleEditScreen                     */
  /****************************************/

  bool SynthSampleScreen::keypress(ui::Key key) {
    switch (key) {
    case ui::K_REC:
      // Toggle recording armed state
      if (module->isRecording) {
        module->stopRecording();
      } else if (module->recordArmed) {
        module->recordArmed = false;  // Cancel arming
      } else {
        module->armRecording();
      }
      return true;
    default:
      return false;
    }
  }

  void SynthSampleScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
    case ui::Rotary::Blue:
      module->props.in.step(e.clicks); break;
    case ui::Rotary::Green:
      module->props.out.step(e.clicks); break;
    case ui::Rotary::White:
      module->props.speed.step(e.clicks); break;
    case ui::Rotary::Red:
      module->props.mode.step(e.clicks); break;
    }
  }

  SynthSampleScreen::SynthSampleScreen(SynthSampler *m) :
    ui::ModuleScreen<SynthSampler> (m),
    topWF (new audio::Waveform(module->sampleData.size() / ui::drawing::topWFsize.w / 4.0, 1.0)
           ),
    topWFW (topWF, ui::drawing::topWFsize),
    mainWF (new audio::Waveform(50, 1.0)),
    mainWFW (mainWF, ui::drawing::mainWFsize) {}

  void modules::SynthSampleScreen::draw(ui::drawing::Canvas &ctx) {
    using namespace ui::drawing;

    Colour colourCurrent;
    auto& props = module->props;

    ctx.callAt(topWFpos, [&] () {
        topWFW.drawRange(ctx, topWFW.viewRange, Colours::TopWF);
        Colour baseColour = Colours::TopWFCur;
        float mix = props.playProgress / float(props.out - props.in);

        if (mix < 0) mix = 1;
        if (props.fwd()) mix = 1 - mix; //data is not reversed

        colourCurrent = baseColour.mix(Colours::TopWFActive, mix);
        topWFW.drawRange(ctx, {
            std::size_t(std::round(props.in / topWF->ratio)),
              std::size_t(std::round(props.out / topWF->ratio))
              }, colourCurrent);
      });

    icons::Arrow icon;

    if (props.fwd()) {
      icon.dir = icons::Arrow::Right;
    } else {
      icon.dir = icons::Arrow::Left;
    }

    if (props.stop()) {
      icon.stopped = true;
    }

    if (props.loop()) {
      icon.looping = true;
    }

    icon.size = arrowSize;
    icon.colour = Colours::Red;
    ctx.drawAt(arrowPos, icon);

    ctx.beginPath();
    ctx.fillStyle(Colours::White);
    ctx.font(Fonts::Norm);
    ctx.font(15);
    ctx.textAlign(TextAlign::Left, TextAlign::Baseline);
    ctx.fillText(fmt::format("×{:.2F}", props.speed.get()), pitchPos);

    // Show root key and pitch offset info
    ctx.textAlign(TextAlign::Left, TextAlign::Baseline);
    ctx.fillStyle(Colours::Blue);
    ctx.fillText(SynthSampler::noteName(props.rootKey), {pitchPos.x, pitchPos.y + 18});

    // Show recording state
    if (module->isRecording) {
      ctx.fillStyle(Colours::Red);
      ctx.font(Fonts::Bold);
      ctx.font(18);
      ctx.textAlign(TextAlign::Right, TextAlign::Baseline);
      ctx.fillText("● REC", {300, 18});
    } else if (module->recordArmed) {
      ctx.fillStyle(Colours::Red);
      ctx.font(Fonts::Norm);
      ctx.font(14);
      ctx.textAlign(TextAlign::Right, TextAlign::Baseline);
      ctx.fillText("○ ARMED", {300, 18});
    }

    ctx.callAt(
      mainWFpos, [&] () {

        // Draw dimmed waveform for entire sample first (out-of-range sections)
        if (mainWF->size() > 0) {
          mainWFW.viewRange = {0, mainWF->size() - 1};
          mainWFW.lineCol = Colours::TopWF.dim(0.5);  // Dimmed for out-of-range
          mainWFW.minPx = 5;
          mainWFW.draw(ctx);
        }

        // Draw active range on top with brighter color
        mainWFW.lineCol = colourCurrent;
        mainWFW.viewRange = {
          std::size_t(std::round(props.in / mainWF->ratio)),
          std::size_t(std::round(props.out / mainWF->ratio))};
        if (mainWFW.viewRange.in < mainWFW.viewRange.out) {
          mainWFW.drawRange(ctx, mainWFW.viewRange, colourCurrent);
        }

        // Draw playhead
        if (props.playProgress >= 0 && !module->isRecording) {
          float playPos = (props.in + props.playProgress) / mainWF->ratio;
          float xPos = (playPos - mainWFW.viewRange.in) / float(mainWFW.viewRange.size()) * mainWFsize.w;
          ctx.beginPath();
          ctx.strokeStyle(Colours::White);
          ctx.lineWidth(2);
          ctx.moveTo(xPos, 0);
          ctx.lineTo(xPos, mainWFsize.h);
          ctx.stroke();
        }

        // Draw recording progress indicator
        if (module->isRecording && module->recordPosition > 0) {
          float recPos = module->recordPosition / mainWF->ratio;
          float xPos = recPos / float(mainWF->size()) * mainWFsize.w;
          ctx.beginPath();
          ctx.strokeStyle(Colours::Red);
          ctx.lineWidth(2);
          ctx.moveTo(xPos, 0);
          ctx.lineTo(xPos, mainWFsize.h);
          ctx.stroke();
        }

        // In point marker (Blue)
        ctx.beginPath();
        ctx.circle(mainWFW.point(mainWFW.viewRange.in), 2);
        ctx.fill(Colours::Blue);

        ctx.beginPath();
        ctx.circle(mainWFW.point(mainWFW.viewRange.in), 5);
        ctx.stroke(Colours::Blue);

        // Out point marker (Green)
        ctx.beginPath();
        ctx.circle(mainWFW.point(mainWFW.viewRange.out), 2);
        ctx.fill(Colours::Green);

        ctx.beginPath();
        ctx.circle(mainWFW.point(mainWFW.viewRange.out), 5);
        ctx.stroke(Colours::Green);
      });

  }
} // top1::module
