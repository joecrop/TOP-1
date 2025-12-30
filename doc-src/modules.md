# TOP-1 Module Reference

This document provides detailed instructions for using each module in the TOP-1.

## Table of Contents
- [Synth Engines](#synth-engines)
- [Effects](#effects)
- [Sequencers](#sequencers)
- [Modulation](#modulation)
- [Tape Controls](#tape-controls)

---

## Synth Engines

Access the synth section with `Ctrl + U`. Use the encoder wheel to select different synth engines.

### Nuke
Additive synthesis engine.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Attack | Envelope attack time |
| Green | Decay | Envelope decay time |
| White | Sustain | Envelope sustain level |
| Red | Release | Envelope release time |

### Super Saw
Fat detuned sawtooth ensemble for pads and leads.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Detune | Amount of detuning between oscillators |
| Green | Mix | Blend between oscillators |
| White | Cutoff | Filter cutoff frequency |
| Red | Resonance | Filter resonance |

**Tips:** Lower detune values create subtle chorus-like effects. Higher values create massive, wide sounds perfect for trance leads or thick pads.

### FM Synth
2-operator FM synthesis with carrier and modulator.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Ratio | Modulator frequency ratio (1-16) |
| Green | Index | FM modulation depth |
| White | Attack | Envelope attack time |
| Red | Release | Envelope release time |

**Tips:** 
- Ratio of 1:1 creates bell-like tones
- Ratio of 2:1 or 3:1 creates brass-like tones
- Higher index values create brighter, more complex timbres

### Pulse Synth
Classic pulse wave with pulse width modulation.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Width | Pulse width (0-100%) |
| Green | PWM | Pulse width modulation amount |
| White | Rate | PWM modulation rate |
| Red | Sub | Sub-oscillator mix level |

**Tips:** A width of 50% creates a square wave. Subtle PWM adds movement and warmth to sustained notes.

### Wavetable Synth
Morphable wavetable synthesis.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Position | Wavetable position (morphs between waves) |
| Green | Morph | Modulation amount on position |
| White | Rate | Morph modulation rate |
| Red | Filter | Low-pass filter cutoff |

**Tips:** Slowly modulating the position creates evolving pad textures.

### String Synth
Karplus-Strong physical modeling for plucked string sounds.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Brightness | Excitation brightness |
| Green | Damping | String damping (higher = faster decay) |
| White | Position | Pluck position along string |
| Red | Feedback | Feedback amount for sustain |

**Tips:** Low damping + high feedback creates long sustaining strings. High damping creates short plucky sounds.

### Cluster Synth
Granular synthesis with multiple overlapping grains.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Spray | Random pitch/time variation |
| Green | Size | Grain size |
| White | Density | Number of active grains |
| Red | Texture | Noise/texture amount |

**Tips:** Small grains + high spray creates ethereal, cloud-like textures. Large grains + low spray creates more tonal sounds.

### Digital Synth
Lo-fi bitcrushed synthesis for retro digital sounds.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Bits | Bit depth (2-16 bits) |
| Green | SRate | Sample rate reduction |
| White | Wave | Waveform selection (saw/square/tri/noise) |
| Red | Glitch | Random sample glitching |

**Tips:** 8-bit + moderate sample reduction = classic video game sounds. Extreme settings create harsh noise textures.

### Phase Synth
Phase distortion synthesis inspired by the Casio CZ series.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Distortion | Phase distortion amount |
| Green | Mode | Distortion waveform mode (0-4) |
| White | Resonance | Resonant peak emphasis |
| Red | Release | Envelope release time |

**Modes:**
- 0: Saw-like distortion
- 1: Square-like distortion
- 2: Pulse distortion
- 3: Resonant distortion
- 4: Sync-like distortion

---

## Effects

Access effects with `Ctrl + E` for master effects. Each track (F1-F4) can have its own effect.

### Delay
Tempo-synced delay with feedback.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Time | Delay time (0.01-2 seconds) |
| Green | Feedback | Delay feedback amount |
| White | Mix | Dry/wet mix |
| Red | Tone | Feedback filter (darker to brighter) |

**Bypass:** Press Shift to bypass the effect.

### Reverb
Algorithmic reverb for space and ambience.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Size | Room size |
| Green | Damping | High frequency damping |
| White | Mix | Dry/wet mix |
| Red | Width | Stereo width |

### Chorus
Classic stereo chorus effect.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Rate | Modulation rate |
| Green | Depth | Modulation depth |
| White | Mix | Dry/wet mix |
| Red | Feedback | Chorus feedback |

### Filter
Multi-mode resonant filter.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Cutoff | Filter cutoff frequency |
| Green | Resonance | Filter resonance/Q |
| White | Mode | Filter mode (LP/BP/HP) |
| Red | Drive | Input drive/saturation |

### Drive
Saturation and distortion effect.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Drive | Distortion amount |
| Green | Tone | Post-distortion tone |
| White | Mix | Dry/wet mix |
| Red | Output | Output level |

### Phaser
Classic phaser effect.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Rate | Modulation rate |
| Green | Depth | Modulation depth |
| White | Feedback | Phaser feedback |
| Red | Stages | Number of phaser stages |

### CWO (Tape Wow & Flutter)
Simulates vintage tape machine pitch wobble.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Rate | Wobble rate (0.1-10 Hz) |
| Green | Depth | Wobble depth |
| White | Flutter | Fast flutter amount (vs slow wow) |
| Red | Mix | Dry/wet mix |

**Tips:** Low rate + moderate depth = vintage tape warmth. High flutter creates lo-fi VHS-style degradation.

### Nitro (Compressor/Boost)
Dynamics compressor with visual feedback.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Drive | Input gain/threshold |
| Green | Ratio | Compression ratio (1:1 to 20:1) |
| White | Attack | Attack time |
| Red | Release | Release time |

The screen displays a compression curve and gain reduction meter.

**Tips:** Low ratio (2-4:1) for gentle leveling. High ratio (10:1+) for limiting/pumping effects.

### Spring (Spring Reverb)
Simulates classic spring reverb tank.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Tension | Spring tension (affects character) |
| Green | Mix | Dry/wet mix |
| White | Decay | Reverb decay time |
| Red | Tone | High frequency content |

**Tips:** High tension creates the classic "boing" spring sound. Lower tension is smoother.

---

## Sequencers

Access sequencers with `Ctrl + O`. Hold Shift first, then press Ctrl+O to open selector, use Blue encoder to choose, then press Ctrl+O again to confirm.

### Pattern Sequencer
16-step × 8-note grid sequencer with real-time MIDI recording.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Cursor/Length | Move cursor column (Shift: change pattern length) |
| Green | Root | Root note of scale |
| White | Swing | Swing amount |
| Red | Scale | Scale type (Chromatic/Major/Minor/Pentatonic/Blues/Drums) |

**How to Use:**

**Edit Mode (stopped):**
1. Use the **Blue encoder** to move the cursor column left/right
2. Play notes on your **MIDI keyboard** - they will populate the current column
3. Use **Shift + Blue** to change pattern length

**Record Mode (playing):**
1. Press **Play** to start the sequencer
2. The cursor advances automatically with the beat
3. Play notes on your **MIDI keyboard** - they record into the grid as it plays
4. Press **Play** again to stop

The grid shows 8 note rows based on the selected scale. Notes are color-coded by row position (Blue/Green/White/Red pairs).

### Arpeggiator
Classic arpeggiator with multiple patterns.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Mode | Pattern mode (up/down/up-down/random) |
| Green | Octaves | Octave range (1-4) |
| White | Rate | Arpeggio rate (note division) |
| Red | Gate | Note gate length |

**Modes:**
- 0: Up - Notes play low to high
- 1: Down - Notes play high to low
- 2: Up-Down - Alternates direction
- 3: Random - Random note order

**Tips:** Hold multiple keys to define the arpeggio chord. The arp will cycle through held notes.

### Endless Sequencer
Generative/algorithmic sequencer that creates evolving patterns.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Blue | Mode | Generation mode |
| Green | Density | Note density/probability |
| White | Range | Pitch range |
| Red | Complexity | Pattern complexity |

**Modes:**
- 0: Random - Pure random notes
- 1: Walk - Random walk (stepwise motion)
- 2: Markov - Markov chain (learns patterns)
- 3: Probability - Probability-weighted grid

**Tips:** 
- Walk mode creates more melodic, stepwise lines
- Markov mode becomes more interesting over time as it learns
- Lower density creates sparse, ambient patterns

---

## Modulation

### LFO System
The TOP-1 has 2 global LFOs that can modulate synth parameters.

**LFO Waveforms:**
- Sine - Smooth modulation
- Triangle - Linear modulation
- Square - Stepped modulation
- Saw Up - Rising ramp
- Saw Down - Falling ramp
- Random - Sample & hold

**Accessing LFO settings:** Use the Mod Matrix screen to assign LFO destinations.

---

## Tape Controls

### Tape Speed (Varispeed)
The tapedeck now includes varispeed control for pitch/tempo manipulation.

| Knob | Parameter | Description |
|------|-----------|-------------|
| Green | Speed | Playback speed (0.5x to 2x) |

**Tips:** 
- Speed below 1.0 = slower playback, lower pitch
- Speed above 1.0 = faster playback, higher pitch
- Recording at half speed then playing back normal = octave up effect

---

## Quick Reference

### Key Bindings

| Key | Action |
|-----|--------|
| **Ctrl+U** | Synth view |
| **Ctrl+I** | Effect view |
| **Ctrl+O** | Sequencer view |
| **Ctrl+T** | Tape Deck view |
| **Ctrl+Y** | Mixer view |
| **Ctrl+G** | Metronome view |
| **Ctrl+J** | Drums view |
| **Shift + (view key)** | Open module selector (hold Shift *before* pressing view key) |
| **Space** | Play/Stop |
| **Shift** | Bypass effect (when on effect screen) |

### How to Change Modules (Synths, Effects, Sequencers)

1. **Hold Shift first**
2. While holding Shift, press the view key (e.g., Ctrl+U for synth)
3. Use ↑/↓ arrows (Blue encoder) to scroll through available modules
4. **Press the view key again without Shift** to confirm and view the module

**Important:** You must hold Shift *before* pressing the view key.

### Signal Flow
```
MIDI Input → Synth Engine → Per-Track Effect → Mixer → Master Effect → Tape → Output
                                    ↑
                              LFO Modulation
```

Each track can have its own effect, and all tracks are mixed before passing through the master effect and being recorded to tape.
