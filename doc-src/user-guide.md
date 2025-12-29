# TOP-1 User Guide

## Quick Start

The TOP-1 is a complete music workstation with 14 synthesizers, 8 effects, 8 sequencers, a 4-track tape recorder, and mixer. This guide will help you navigate all the features.

### Basic Navigation

**Key Bindings:**
- `Ctrl + U` - Open Synth selector
- `Ctrl + E` - Open Effect selector (master)
- `Ctrl + S` - Open Sequencer selector
- `Ctrl + T` - Open Tapedeck (recorder)
- `Ctrl + Y` - Open Mixer
- `Ctrl + G` - Open Metronome
- `F1-F4` - Select Track 1-4

**Encoder Controls:**
- `Q/A` - Red Encoder (increase/decrease)
- `W/S` - Blue Encoder (increase/decrease)
- `E/D` - White Encoder (increase/decrease)
- `R/F` - Green Encoder (increase/decrease)

**Transport Controls:**
- `Space` - Play/Stop
- `Z` - Record
- `Left/Right Arrow` - Rewind/Forward
- `Shift + Left/Right Arrow` - Skip backward/forward

---

## Synthesizers (14 Total)

Press `Ctrl + U` to access the synth selector, then use encoders to select and configure.

### 1. **Nuke** - Original TOP-1 Synth
Basic synthesizer with classic waveform generation.
- **Red**: Attack
- **Blue**: Decay/Release
- **White**: Brightness/Filter
- **Green**: Level

### 2. **Super Saw** - Detuned Saw Stack
Rich unison synthesizer with up to 7 detuned saw waves.
- **Red**: Detune amount (0-100%)
- **Blue**: Voices (1-7)
- **White**: Attack time
- **Green**: Release time

### 3. **Sampler** - Chromatic Sample Player
Plays samples chromatically across the keyboard.
- **Red**: Sample selection
- **Blue**: Attack time
- **White**: Release time
- **Green**: Level/Volume
- Supports playback at different pitches

### 4. **FM** - Frequency Modulation Synth
Classic FM synthesis with sine wave modulation.
- **Red**: Modulation Index (0-10) - Harmonicity
- **Blue**: Modulation Ratio (0.5-8.0) - Frequency ratio
- **White**: Attack time
- **Green**: Release time
- Creates bell-like, metallic, and complex timbres

### 5. **Pulse** - Pulse Width Modulation Synth
Square wave with variable pulse width.
- **Red**: Pulse Width (5-95%) - Tone color
- **Blue**: PWM Rate (0.1-10 Hz) - Modulation speed
- **White**: Attack time
- **Green**: Release time
- Width of 50% = square wave, other values = varied timbres

### 6. **Wavetable** - Multi-Waveform Synth
Scans through different waveforms smoothly.
- **Red**: Table Position (0-100%) - Selects waveform
- **Blue**: Scan Rate (0-5 Hz) - Auto-scan speed
- **White**: Attack time
- **Green**: Release time
- Contains sine, saw, square, triangle, and variations

### 7. **String** - Karplus-Strong Physical Model
Physical modeling synthesis for plucked/struck strings.
- **Red**: Damping (0-100%) - String decay rate
- **Blue**: Brightness (0-100%) - High frequency content
- **White**: Body Resonance (0-100%) - Acoustic body simulation
- **Green**: Release time
- Great for guitars, harps, and plucked instruments

### 8. **Vocoder** - 16-Band Vocoder
Multi-band vocoder for robotic and synthetic voices.
- **Red**: Carrier Waveform (1-4) - Sine/Saw/Square/Noise
- **Blue**: Band Emphasis (0-100%) - Frequency balance
- **White**: Attack time
- **Green**: Release time
- 16 frequency bands from 100 Hz to 8000 Hz

### 9. **Phase** - Phase Distortion Synthesis
OP-1 inspired phase distortion synth.
- **Red**: Distortion Amount (0-100%) - Phase warping
- **Blue**: Shape (0-100%) - Waveform character
- **White**: Attack time
- **Green**: Release time
- Creates evolving, metallic sounds

### 10. **Digital** - Additive Synthesis
Builds sounds from up to 16 harmonics.
- **Red**: Harmonics (1-16) - Number of partials
- **Blue**: Detune (0-100%) - Harmonic detuning
- **White**: Brightness (0-100%) - High harmonic filtering
- **Green**: Release time
- Visual: Shows harmonic spectrum bars

### 11. **Cluster** - Granular Synthesis
Texture generator with grain clouds.
- **Red**: Grain Size (10-500ms) - Individual grain length
- **Blue**: Density (10-100%) - Grain spawn rate
- **White**: Pitch (±12 semitones) - Grain pitch shift
- **Green**: Spread (0-100%) - Pitch randomization
- Visual: Shows active grain cloud
- Great for pads, textures, and soundscapes

### 12. **Voltage** - Analog Modeling Synth
Classic analog-style subtractive synthesis.
- **Red**: Waveform (1-3) - Saw/Square/Triangle
- **Blue**: Cutoff (0-100%) - Filter frequency
- **White**: Resonance (0-100%) - Filter emphasis
- **Green**: Envelope Amount (0-100%) - Filter modulation
- Dual ADSR envelopes (amp + filter)
- Visual: Shows waveform and cutoff position

### 13. **DNA** - Wavetable Morphing Synth
Advanced wavetable synthesis with mutation.
- **Red**: Table A (1-8) - First wavetable
- **Blue**: Table B (1-8) - Second wavetable
- **White**: Morph (0-100%) - Crossfade between tables
- **Green**: Mutate (0-100%) - Phase distortion amount
- 8 Wavetables: Sine, Saw, Square, Triangle, Pulse, Stepped, Noisy, Harmonic
- Visual: Shows morphed waveform in real-time

### 14. **Drum Sampler** - Percussion Sample Player
Specialized for drum and percussion samples.
- **Red**: Sample selection
- **Blue**: Pitch adjustment
- **White**: Decay time
- **Green**: Level

---

## Effects (8 Total)

Press `Ctrl + E` for master effects or access per-track effects from the mixer.

### 1. **Delay** - Echo Effect
Classic delay line with feedback.
- **Red**: Time (0-2000ms) - Delay length
- **Blue**: Feedback (0-95%) - Number of repeats
- **White**: Mix (0-100%) - Dry/wet balance
- **Green**: Bypassed (toggle)
- Synchronized to BPM when using musical divisions

### 2. **Reverb** - Hall Reverb
Spacious reverb for depth and ambience.
- **Red**: Size (0-100%) - Room/hall size
- **Blue**: Damping (0-100%) - High frequency absorption
- **White**: Mix (0-100%) - Dry/wet balance
- **Green**: Bypassed (toggle)
- Creates sense of space and dimension

### 3. **Chorus** - Pitch Modulation
Thickens sound with pitch-modulated delays.
- **Red**: Rate (0.1-5 Hz) - LFO speed
- **Blue**: Depth (0-100%) - Modulation amount
- **White**: Mix (0-100%) - Dry/wet balance
- **Green**: Bypassed (toggle)
- Adds richness and width to sounds

### 4. **Filter** - State Variable Filter
Multi-mode resonant filter.
- **Red**: Cutoff (20-20000 Hz) - Filter frequency
- **Blue**: Resonance (0-100%) - Filter emphasis
- **White**: Type (LP/BP/HP) - Filter mode
- **Green**: Bypassed (toggle)
- LP = Low Pass, BP = Band Pass, HP = High Pass

### 5. **Compressor** - Dynamic Range Control
Controls signal dynamics and adds punch.
- **Red**: Threshold (-60 to 0 dB) - Compression trigger
- **Blue**: Ratio (1:1 to 20:1) - Compression amount
- **White**: Attack (0.1-100ms) - Response speed
- **Green**: Release (10-1000ms) - Recovery time
- Automatically balances volume and adds sustain

### 6. **Bitcrusher** - Lo-Fi Digital Degradation
Reduces bit depth and sample rate for gritty sounds.
- **Red**: Bit Depth (1-16 bits) - Resolution reduction
- **Blue**: Sample Rate (100-48000 Hz) - Sample decimation
- **White**: Mix (0-100%) - Dry/wet balance
- **Green**: Bypassed (toggle)
- Creates retro digital and aggressive textures

### 7. **Distortion** - Waveshaping Distortion
Adds harmonic saturation and grit.
- **Red**: Drive (0-100%) - Distortion amount
- **Blue**: Tone (0-100%) - Frequency emphasis
- **White**: Mix (0-100%) - Dry/wet balance
- **Green**: Bypassed (toggle)
- From subtle warmth to aggressive fuzz

### 8. **Phaser** - Phase Shifting Effect
Sweeping notch filter for psychedelic sounds.
- **Red**: Rate (0.1-10 Hz) - Sweep speed
- **Blue**: Depth (0-100%) - Sweep range
- **White**: Feedback (0-95%) - Resonance amount
- **Green**: Bypassed (toggle)
- Creates swooshing, moving filter effects

---

## Sequencers (8 Total)

Press `Ctrl + S` to access the sequencer selector.

### 1. **Euclidean** - Euclidean Rhythm Generator
Creates rhythmic patterns using Euclidean algorithm.
- **Red**: Steps (1-32) - Pattern length
- **Blue**: Pulses (1-32) - Number of hits
- **White**: Rotation (0-31) - Pattern offset
- **Green**: Division (1-16) - Note timing
- Automatically distributes pulses evenly across steps
- Great for creating polyrhythms

### 2. **Pattern** - Step Sequencer
Traditional step sequencer with up to 32 steps.
- **Red**: Pattern Length (1-32) - Number of steps
- **Blue**: Current Step selection
- **White**: Note value for current step
- **Green**: Division (1-16) - Step timing
- Edit notes per step, create melodies and basslines

### 3. **Arpeggiator** - Note Arpeggiator
Plays held notes in sequence.
- **Red**: Mode (Up/Down/UpDown/Random) - Play order
- **Blue**: Octaves (1-4) - Octave range
- **White**: Division (1-16) - Note speed
- **Green**: Gate Length (1-100%) - Note duration
- Hold notes to arpeggiate them automatically

### 4. **Endless** - Generative Sequencer
Algorithmic melody generator with probability.
- **Red**: Mutation Rate (0-100%) - Change frequency
- **Blue**: Scale Root (0-11) - Key center
- **White**: Scale Type (Major/Minor/etc) - Musical scale
- **Green**: Division (1-16) - Note timing
- Creates evolving, semi-random melodies
- Never plays the same pattern twice

### 5. **Tombola** - Probability Sequencer
Each step has independent trigger probability.
- **Red**: Steps (4-32) - Pattern length
- **Blue**: Current Step selection
- **White**: Probability (0-100%) - Trigger chance
- **Green**: Division (1-16) - Step timing
- Each step may or may not play based on probability
- Great for variation and humanization

### 6. **Finger** - Performance Recorder
Records and loops your playing.
- **Red**: Loop Length (1-64 beats) - Recording length
- **Blue**: Overdub (On/Off) - Layer recordings
- **White**: Quantize (Off/1/16/1/8/1/4) - Timing correction
- **Green**: Clear/Record toggle
- Record notes in real-time, loops automatically
- Build up complex parts through layering

### 7. **Grid** - Matrix Step Sequencer
2D grid-based pattern programming.
- **Red**: Rows (4-16) - Note range (pitch)
- **Blue**: Columns (8-32) - Pattern length (time)
- **White**: Division (1-16) - Step timing
- **Green**: Swing (0-75%) - Timing offset
- Click grid cells to toggle notes
- Visual matrix shows entire pattern
- Each row represents a different note

### 8. **Sketch** - Drawable Timeline Sequencer
Draw notes on a timeline like a piano roll.
- **Red**: Loop Length (4-64 beats) - Timeline length
- **Blue**: Quantize (Off/1/16/1/8/1/4) - Grid snap
- **White**: Note Range (12-48 semitones) - Vertical zoom
- **Green**: Base Note (36-84) - Starting pitch
- Draw notes by clicking and dragging
- Visual timeline shows all notes
- Free-form melody creation

---

## Tapedeck (4-Track Recorder)

Press `Ctrl + T` to access the tape recorder.

### Transport Controls
- `Space` - Play/Stop
- `Z` - Record (hold to record)
- `Left/Right Arrow` - Rewind/Forward
- `Shift + Left/Right` - Skip backward/forward

### Loop Controls
- `I` - Jump to Loop In point
- `Shift + I` - Set Loop In point at current position
- `O` - Jump to Loop Out point
- `Shift + O` - Set Loop Out point at current position
- `L` - Toggle Looping on/off

### Editing
- `Ctrl + X` - Cut selected tape section
- `Ctrl + C` - Lift (copy) selected tape section
- `Ctrl + V` - Drop (paste) tape section

### Tracks
- `F1-F4` - Select Track 1-4 for recording/playback
- Each track can have independent effects
- Mix all tracks together in the Mixer

### Recording Workflow
1. Select a track with `F1-F4`
2. Set up your synth/sequencer
3. Hold `Z` to start recording
4. Release `Z` to stop recording
5. Use loops for overdubbing

---

## Mixer

Press `Ctrl + Y` to access the mixer.

### Per-Track Controls
Each of the 4 tracks has:
- **Level** - Track volume (0-100%)
- **Pan** - Left/Right positioning (-90% to +90%)
- **Mute** - Silence track (toggle)
- **Effects** - Independent effect chain per track

### Track Effects
Each track can have its own effects:
1. Access mixer (`Ctrl + Y`)
2. Select track (`F1-F4`)
3. Use track-specific effect controls
4. Same 8 effects as master, per-track routing

### Master Effects
Master effects process the final mix:
- Applied after all tracks are mixed
- Access via `Ctrl + E`
- Processes everything going to output

---

## Metronome

Press `Ctrl + G` to access the metronome.

### Controls
- **Red**: BPM (40-240) - Tempo
- **Blue**: Time Signature - Beats per bar
- **White**: Click Volume - Metronome level
- **Green**: Enabled (toggle)

### Features
- Provides timing reference for recording
- Synchronizes sequencers and delays
- Visual beat indicator
- First beat of bar is accented

---

## Tips & Techniques

### Creating a Track
1. Start metronome (`Ctrl + G`)
2. Select a synth (`Ctrl + U`)
3. Choose a sequencer (`Ctrl + S`)
4. Select track (`F1`)
5. Record to tape (`Z`)
6. Add effects (`Ctrl + E` or via mixer)
7. Layer more tracks (`F2-F4`)
8. Mix in mixer (`Ctrl + Y`)

### Layering Sounds
- Use multiple tracks for different parts (drums, bass, melody, pads)
- Each track can have different synth/sequencer combinations
- Use per-track effects for individual sound design
- Master effects for final polish

### Creating Textures
- **Cluster** synth for ambient pads
- **Digital** synth with high harmonics for bells
- **Reverb** + **Delay** for spaciousness
- **Phaser** + **Chorus** for movement

### Rhythmic Patterns
- **Euclidean** for complex polyrhythms
- **Tombola** for variation and groove
- **Grid** for precise step programming
- Combine multiple sequencers on different tracks

### Sound Design
- **FM** synth for metallic/bell sounds
- **Phase** synth for evolving textures
- **Voltage** synth for classic analog sounds
- **DNA** synth for complex morphing timbres
- Layer **Bitcrusher** + **Distortion** for aggressive sounds

### Performance
- Use **Finger** sequencer to record improvisations
- **Arpeggiator** for instant chord patterns
- **Endless** for generative backgrounds
- **Sketch** for drawing melodies

---

## Signal Flow

```
Sequencer → Synth → Track Effects → Tapedeck → Master Effects → Mixer → Output
```

1. **Sequencer** generates MIDI notes
2. **Synth** converts notes to audio
3. **Track Effects** process individual track
4. **Tapedeck** records/plays back audio
5. **Master Effects** process final mix
6. **Mixer** balances and outputs audio

---

## Keyboard Reference

### Module Selection
- `Ctrl + U` - Synth
- `Ctrl + E` - Effects
- `Ctrl + S` - Sequencer  
- `Ctrl + T` - Tapedeck
- `Ctrl + Y` - Mixer
- `Ctrl + G` - Metronome

### Encoders
- `Q/A` - Red Encoder (+/-)
- `W/S` - Blue Encoder (+/-)
- `E/D` - White Encoder (+/-)
- `R/F` - Green Encoder (+/-)

### Transport
- `Space` - Play/Stop
- `Z` - Record
- `←/→` - Rewind/Forward
- `Shift + ←/→` - Skip

### Tracks
- `F1` - Track 1
- `F2` - Track 2
- `F3` - Track 3
- `F4` - Track 4

### Loops
- `I` - Go to Loop In
- `Shift + I` - Set Loop In
- `O` - Go to Loop Out
- `Shift + O` - Set Loop Out
- `L` - Toggle Looping

### Editing
- `Ctrl + X` - Cut
- `Ctrl + C` - Lift (Copy)
- `Ctrl + V` - Drop (Paste)

---

## Getting Started Projects

### Project 1: Simple Beat
1. Select Drum Sampler synth (`Ctrl + U`)
2. Choose Euclidean sequencer (`Ctrl + S`)
3. Set 16 steps, 4 pulses
4. Record to Track 1
5. Layer more drums on other tracks

### Project 2: Ambient Texture
1. Select Cluster synth (granular)
2. Set grain size to 200ms
3. Add Reverb and Delay effects
4. Use Endless sequencer for slow notes
5. Record long, evolving textures

### Project 3: Bassline
1. Select Voltage synth (analog)
2. Use Pattern sequencer
3. Program 16-step bass pattern
4. Add Filter effect with low cutoff
5. Add Compressor for punch

### Project 4: Melodic Sequence
1. Select DNA synth (wavetable)
2. Use Grid sequencer
3. Create melodic pattern on grid
4. Add Chorus for width
5. Layer with pad on another track

---

## Troubleshooting

### No Sound
- Check if Jack audio is running
- Verify mixer levels aren't at 0
- Check if tracks are muted
- Ensure effects aren't bypassed when expected

### Timing Issues
- Set metronome BPM
- Check sequencer division settings
- Verify tape is playing

### Recording Problems
- Select correct track (F1-F4)
- Hold Z key to record
- Check if synth is producing sound
- Verify sequencer is active

---

For more information, visit the [TOP-1 GitHub repository](https://github.com/topisani/TOP-1).
