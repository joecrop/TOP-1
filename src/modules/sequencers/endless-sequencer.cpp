#include "modules/sequencers/endless-sequencer.hpp"
#include "core/globals.hpp"
#include "core/ui/drawing.hpp"
#include <algorithm>

namespace top1::modules {

  EndlessSequencer::EndlessSequencer() 
    : screen(std::make_unique<EndlessSeqScreen>(this)),
      rng(std::random_device{}()) {
    generatePattern();
  }

  int EndlessSequencer::quantizeToScale(int note) {
    int root = props.rootNote;
    int octave = (note - root) / 12;
    int degree = (note - root) % 12;
    if (degree < 0) {
      degree += 12;
      octave--;
    }

    int scale = props.scale;
    int quantized = degree;

    switch (scale) {
      case 0: // Major
        for (int i = 6; i >= 0; i--) {
          if (degree >= scaleMajor[i]) {
            quantized = scaleMajor[i];
            break;
          }
        }
        break;
      case 1: // Minor
        for (int i = 6; i >= 0; i--) {
          if (degree >= scaleMinor[i]) {
            quantized = scaleMinor[i];
            break;
          }
        }
        break;
      case 2: // Pentatonic
        for (int i = 4; i >= 0; i--) {
          if (degree >= scalePentatonic[i]) {
            quantized = scalePentatonic[i];
            break;
          }
        }
        break;
      case 3: // Blues
        for (int i = 5; i >= 0; i--) {
          if (degree >= scaleBlues[i]) {
            quantized = scaleBlues[i];
            break;
          }
        }
        break;
      case 4: // Dorian
        for (int i = 6; i >= 0; i--) {
          if (degree >= scaleDorian[i]) {
            quantized = scaleDorian[i];
            break;
          }
        }
        break;
      case 5: // Chromatic
        quantized = degree;
        break;
    }

    return root + octave * 12 + quantized;
  }

  void EndlessSequencer::generatePattern() {
    std::uniform_real_distribution<float> probDist(0.0f, 1.0f);
    std::uniform_int_distribution<int> velDist(60, 127);
    
    float densityFactor = props.density / 100.0f;
    int range = props.noteRange;
    int root = props.rootNote;

    for (int step = 0; step < NUM_STEPS; step++) {
      for (int voice = 0; voice < NUM_VOICES; voice++) {
        Voice& v = pattern[step][voice];
        
        // Higher voices have lower probability
        float voiceProb = densityFactor * (1.0f - voice * 0.2f);
        v.probability = voiceProb;
        v.active = probDist(rng) < voiceProb;
        
        if (v.active) {
          // Generate note within range
          std::uniform_int_distribution<int> noteDist(root, root + range);
          int rawNote = noteDist(rng);
          v.note = quantizeToScale(rawNote);
          v.velocity = velDist(rng);
        }
      }
    }
  }

  void EndlessSequencer::evolvePattern() {
    if (!props.evolve) return;
    
    float variance = props.variance / 100.0f;
    std::uniform_real_distribution<float> probDist(0.0f, 1.0f);
    
    // Mutate a random step
    std::uniform_int_distribution<int> stepDist(0, props.length - 1);
    int stepToMutate = stepDist(rng);
    
    if (probDist(rng) < variance) {
      mutateStep(stepToMutate);
    }
  }

  void EndlessSequencer::mutateStep(int step) {
    std::uniform_real_distribution<float> probDist(0.0f, 1.0f);
    std::uniform_int_distribution<int> voiceDist(0, NUM_VOICES - 1);
    std::uniform_int_distribution<int> velDist(60, 127);
    
    int voice = voiceDist(rng);
    Voice& v = pattern[step][voice];
    
    float mutationType = probDist(rng);
    
    if (mutationType < 0.3f) {
      // Toggle active state
      v.active = !v.active;
    } else if (mutationType < 0.6f) {
      // Change note
      int range = props.noteRange;
      int root = props.rootNote;
      std::uniform_int_distribution<int> noteDist(root, root + range);
      v.note = quantizeToScale(noteDist(rng));
    } else if (mutationType < 0.8f) {
      // Change velocity
      v.velocity = velDist(rng);
    } else {
      // Shift note by scale degree
      int shift = (probDist(rng) < 0.5f) ? -1 : 1;
      v.note = quantizeToScale(v.note + shift);
    }
  }

  void EndlessSequencer::process(const audio::ProcessData& data) {
    if (!props.running) return;

    float bpm = Globals::metronome.props.bpm.get();
    float samplesPerBeat = (60.0f / bpm) * Globals::samplerate;
    float samplesPerStep = samplesPerBeat / 4.0f;  // 16th notes

    accumulator += data.nframes;

    if (accumulator >= samplesPerStep) {
      accumulator -= samplesPerStep;
      
      // Move to next step
      currentStep = (currentStep + 1) % props.length.get();
      
      // Trigger notes for current step
      for (int voice = 0; voice < NUM_VOICES; voice++) {
        Voice& v = pattern[currentStep][voice];
        if (v.active && v.note >= 0 && v.note <= 127) {
          data.midi.emplace_back(new midi::NoteOnEvent(v.note, v.velocity));
        }
      }
      
      // Periodically evolve the pattern
      evolutionCounter++;
      if (evolutionCounter >= props.length.get()) {
        evolutionCounter = 0;
        evolvePattern();
      }
    }
  }

  void EndlessSequencer::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* EndlessSeqScreen Implementation      */
  /****************************************/

  using namespace ui::drawing;

  void EndlessSeqScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    int len = module->props.length.get();
    int step = module->currentStep;
    bool running = module->props.running;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("ENDLESS", 160, 18);

    // Running/Evolve indicators
    ctx.font(Fonts::Norm);
    ctx.font(12);
    if (running) {
      ctx.fillStyle(Colours::Green);
      ctx.textAlign(TextAlign::Left, TextAlign::Top);
      ctx.fillText("PLAY", 10, 10);
    }
    if (module->props.evolve) {
      ctx.fillStyle(Colours::Blue);
      ctx.textAlign(TextAlign::Right, TextAlign::Top);
      ctx.fillText("EVOLVE", 310, 10);
    }

    // Scale name
    const char* scaleNames[] = {"MAJOR", "MINOR", "PENTA", "BLUES", "DORIAN", "CHROM"};
    ctx.font(12);
    ctx.fillStyle(Colours::Gray60);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText(scaleNames[module->props.scale.get()], 160, 35);

    // Pattern grid visualization
    float gridX = 20;
    float gridY = 50;
    float cellW = (280.0f) / len;
    float cellH = 18;
    float voiceGap = 2;

    for (int s = 0; s < len; s++) {
      float x = gridX + s * cellW;
      
      for (int v = 0; v < EndlessSequencer::NUM_VOICES; v++) {
        float y = gridY + v * (cellH + voiceGap);
        auto& voice = module->pattern[s][v];
        
        // Background
        Colour bgCol = Colours::Gray70.dim(0.3f);
        if (s == step && running) {
          bgCol = Colours::Gray70;
        }
        
        ctx.beginPath();
        ctx.roundedRect(x + 1, y, cellW - 2, cellH, 2);
        ctx.fillStyle(bgCol);
        ctx.fill();
        
        // Active note indicator
        if (voice.active) {
          // Color based on voice
          Colour noteCol;
          switch (v) {
            case 0: noteCol = Colours::Blue; break;
            case 1: noteCol = Colours::Green; break;
            case 2: noteCol = Colours::White; break;
            default: noteCol = Colours::Red; break;
          }
          
          // Height based on velocity
          float velHeight = (voice.velocity / 127.0f) * (cellH - 4);
          
          ctx.beginPath();
          ctx.roundedRect(x + 2, y + cellH - velHeight - 2, cellW - 4, velHeight, 1);
          ctx.fillStyle(noteCol);
          ctx.fill();
        }
      }
      
      // Step marker
      if (s == step && running) {
        ctx.beginPath();
        ctx.strokeStyle(Colours::White);
        ctx.lineWidth(2);
        ctx.roundedRect(x, gridY - 2, cellW, (cellH + voiceGap) * 4 + 4, 3);
        ctx.stroke();
      }
    }

    // Parameters
    float paramY = 145;
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);

    // Density (Blue)
    ctx.fillStyle(Colours::Blue);
    ctx.fillText("DENSITY", 50, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{}%", module->props.density.get()), 50, paramY + 14);

    // Length (Green)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Green);
    ctx.fillText("LENGTH", 120, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(std::to_string(module->props.length.get()), 120, paramY + 14);

    // Variance (White)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::White);
    ctx.fillText("EVOLVE", 200, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{}%", module->props.variance.get()), 200, paramY + 14);

    // Range (Red)
    ctx.font(Fonts::Norm);
    ctx.font(12);
    ctx.fillStyle(Colours::Red);
    ctx.fillText("RANGE", 270, paramY);
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillText(fmt::format("{}st", module->props.noteRange.get()), 270, paramY + 14);
  }

  bool EndlessSeqScreen::keypress(ui::Key key) {
    switch (key) {
      case ui::Key::K_PLAY:
        module->props.running = !module->props.running;
        if (module->props.running) {
          module->currentStep = 0;
        }
        return true;
      case ui::Key::K_SHIFT:
        // Regenerate pattern
        module->generatePattern();
        return true;
      case ui::Key::K_REC:
        // Toggle evolve mode
        module->props.evolve = !module->props.evolve;
        return true;
      default:
        return false;
    }
  }

  void EndlessSeqScreen::rotary(ui::RotaryEvent e) {
    switch (e.rotary) {
      case ui::Rotary::Blue:
        module->props.density.step(e.clicks);
        break;
      case ui::Rotary::Green:
        module->props.length.step(e.clicks);
        break;
      case ui::Rotary::White:
        module->props.variance.step(e.clicks);
        break;
      case ui::Rotary::Red:
        module->props.noteRange.step(e.clicks);
        break;
    }
  }

} // namespace top1::modules
