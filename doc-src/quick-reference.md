# TOP-1 Quick Reference Card

## Module Access
| Key | Module |
|-----|--------|
| `Ctrl + U` | Synth Selector |
| `Ctrl + E` | Effects (Master) |
| `Ctrl + S` | Sequencer |
| `Ctrl + T` | Tapedeck |
| `Ctrl + Y` | Mixer |
| `Ctrl + G` | Metronome |

## Encoders
| Keys | Encoder | Color |
|------|---------|-------|
| `Q` / `A` | +/- | Red |
| `W` / `S` | +/- | Blue |
| `E` / `D` | +/- | White |
| `R` / `F` | +/- | Green |

## Transport
| Key | Function |
|-----|----------|
| `Space` | Play/Stop |
| `Z` | Record (hold) |
| `←` | Rewind |
| `→` | Forward |
| `Shift + ←` | Skip Backward |
| `Shift + →` | Skip Forward |

## Tracks
| Key | Track |
|-----|-------|
| `F1` | Track 1 |
| `F2` | Track 2 |
| `F3` | Track 3 |
| `F4` | Track 4 |

## Loops
| Key | Function |
|-----|----------|
| `I` | Go to Loop In |
| `Shift + I` | Set Loop In |
| `O` | Go to Loop Out |
| `Shift + O` | Set Loop Out |
| `L` | Toggle Looping |

## Editing
| Key | Function |
|-----|----------|
| `Ctrl + X` | Cut Selection |
| `Ctrl + C` | Lift (Copy) |
| `Ctrl + V` | Drop (Paste) |

---

## 14 Synthesizers

1. **Nuke** - Original TOP-1 synth
2. **Super Saw** - Detuned saw stack (1-7 voices)
3. **Sampler** - Chromatic sample playback
4. **FM** - 2-operator frequency modulation
5. **Pulse** - PWM square wave
6. **Wavetable** - Scanning wavetable synth
7. **String** - Karplus-Strong physical model
8. **Vocoder** - 16-band vocoder
9. **Phase** - Phase distortion synthesis
10. **Digital** - Additive synthesis (1-16 harmonics)
11. **Cluster** - Granular synthesis engine
12. **Voltage** - Analog modeling (subtractive)
13. **DNA** - Wavetable morphing with mutation
14. **Drum Sampler** - Percussion sampler

## 8 Effects

1. **Delay** - Echo (0-2000ms, feedback, mix)
2. **Reverb** - Hall reverb (size, damping, mix)
3. **Chorus** - Pitch modulation (rate, depth, mix)
4. **Filter** - SVF filter (LP/BP/HP, cutoff, resonance)
5. **Compressor** - Dynamics (threshold, ratio, attack, release)
6. **Bitcrusher** - Lo-fi (bit depth, sample rate, mix)
7. **Distortion** - Waveshaping (drive, tone, mix)
8. **Phaser** - Phase shift (rate, depth, feedback)

## 8 Sequencers

1. **Euclidean** - Algorithmic rhythm (steps, pulses, rotation)
2. **Pattern** - Step sequencer (1-32 steps)
3. **Arpeggiator** - Note arpeggiator (up/down/random, 1-4 octaves)
4. **Endless** - Generative melody (mutation, scale)
5. **Tombola** - Probability sequencer (per-step probability)
6. **Finger** - Performance recorder (loop, overdub, quantize)
7. **Grid** - Matrix sequencer (rows × columns, swing)
8. **Sketch** - Timeline drawer (draw notes, quantize)

---

## Common Workflows

### Recording a Track
1. `Ctrl + G` - Start metronome
2. `Ctrl + U` - Select synth
3. `Ctrl + S` - Select sequencer
4. `F1` - Choose track
5. `Z` (hold) - Record
6. `Ctrl + Y` - Mix in mixer

### Layering Sounds
1. Record Track 1 (bass)
2. `F2` - Switch to Track 2
3. Change synth/sequencer
4. Record Track 2 (chords)
5. Repeat for Tracks 3 & 4

### Adding Effects
- **Per Track**: `Ctrl + Y` (Mixer), select track, adjust track effects
- **Master**: `Ctrl + E` - Affects final mix

### Creating Loops
1. `Shift + I` - Set loop start
2. Play/record
3. `Shift + O` - Set loop end
4. `L` - Enable looping
5. Overdub additional layers

---

## Signal Flow
```
Sequencer → Synth → Track FX → Tapedeck → Master FX → Mixer → Output
```

---

## Quick Sound Design Tips

### Bass
- **Voltage**: Saw, low cutoff, high resonance
- **FM**: Ratio 0.5, Index 1.5
- **Pulse**: Width 50%, no PWM

### Lead
- **Digital**: 8 harmonics, 20% detune
- **Voltage**: Saw, high cutoff, envelope mod
- **Phase**: High distortion

### Pad
- **Cluster**: 200ms grains, 40% density
- **Super Saw**: 5 voices, 35% detune
- **DNA**: Sine→Harmonic morph

### Pluck
- **String**: 40% damping, 60% brightness
- **FM**: Ratio 1.0, Index 2.5

### Texture
- **Cluster**: Small grains, high density
- **DNA**: High mutation
- **Endless**: Low mutation rate

---

## Effect Combinations

### Ambient
- Reverb (80% size) + Delay (500ms, 30% feedback)

### Lo-Fi
- Bitcrusher (8-bit, 8kHz) + Distortion (30% drive)

### Classic
- Chorus (0.5Hz) + Reverb (60% size)

### Aggressive
- Distortion (70% drive) + Phaser (fast rate)

---

## Sequencer Tips

### Polyrhythms
- Euclidean on multiple tracks with different steps

### Variation
- Tombola with mixed probabilities (100%, 60%, 30%)

### Live Performance
- Finger sequencer with overdub enabled

### Precise Programming
- Grid for drums and rhythmic patterns
- Sketch for melodies and curves

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| No sound | Check mixer levels, track mute, effects bypass |
| Distortion | Lower levels, use compressor |
| Thin sound | Layer synths, add chorus/reverb |
| Muddy mix | HP filter on non-bass, lower reverb on bass |
| Timing off | Check metronome BPM, sequencer division |

---

## Frequency Ranges

| Instrument | Frequency Range |
|------------|----------------|
| Sub Bass | 20-60 Hz |
| Bass | 60-250 Hz |
| Low Mids | 250-500 Hz |
| Mids | 500-2000 Hz |
| High Mids | 2000-6000 Hz |
| Highs | 6000-20000 Hz |

---

## BPM & Timing (at 120 BPM)

| Division | Time | Musical Note |
|----------|------|--------------|
| 1 | 2000ms | Whole |
| 2 | 1000ms | Half |
| 4 | 500ms | Quarter |
| 8 | 250ms | Eighth |
| 16 | 125ms | Sixteenth |

---

## MIDI Notes

| Octave | C | D | E | F | G | A | B |
|--------|---|---|---|---|---|---|---|
| 0 | 12 | 14 | 16 | 17 | 19 | 21 | 23 |
| 1 | 24 | 26 | 28 | 29 | 31 | 33 | 35 |
| 2 | 36 | 38 | 40 | 41 | 43 | 45 | 47 |
| 3 | 48 | 50 | 52 | 53 | 55 | 57 | 59 |
| 4 | 60 | 62 | 64 | 65 | 67 | 69 | 71 |
| 5 | 72 | 74 | 76 | 77 | 79 | 81 | 83 |
| 6 | 84 | 86 | 88 | 89 | 91 | 93 | 95 |

*Note: C4 (60) = Middle C*

---

## Quick Presets

### Electric Piano
- FM: Ratio 1.0, Index 2.5
- Chorus: Rate 0.5Hz, Depth 40%

### Organ
- Digital: 8 harmonics, 0% detune, 70% brightness

### Synth Brass
- FM: Ratio 4.0, Index 7.0
- Filter: LP 3kHz, Resonance 30%

### Analog Bass
- Voltage: Saw, Cutoff 20%, Resonance 60%
- Compressor: Threshold -15dB, Ratio 6:1

### Ambient Pad
- Cluster: Grain 200ms, Density 40%
- Reverb: Size 80% + Delay: 500ms

---

For detailed information, see:
- **User Guide** (`doc-src/user-guide.md`)
- **Technical Guide** (`doc-src/technical-guide.md`)
- **GitHub**: https://github.com/topisani/TOP-1
- **Discord**: https://discord.gg/4cV9Ucz
