# TOP-1 Technical Guide

This guide provides in-depth technical information about each module, synthesis algorithms, and advanced usage techniques.

---

## Table of Contents
1. [Synthesizer Engines](#synthesizer-engines)
2. [Effects Processors](#effects-processors)
3. [Sequencer Algorithms](#sequencer-algorithms)
4. [Signal Chain Architecture](#signal-chain-architecture)
5. [Advanced Techniques](#advanced-techniques)

---

## Synthesizer Engines

### 1. Nuke Synth
**Type:** Basic Synthesizer  
**Engine:** Original TOP-1 synthesis

**Technical Details:**
- Polyphony: 8 voices
- Waveform generation with ADSR envelope
- Simple but effective for basic sounds

**Parameters:**
- Attack: Envelope attack time (0-1s)
- Decay/Release: Combined decay and release time
- Brightness: Filter-like timbre control
- Level: Output amplitude

**Best For:** Quick sounds, learning synthesis basics

---

### 2. Super Saw Synth
**Type:** Unison Synthesizer  
**Engine:** Detuned sawtooth stack

**Technical Details:**
- Polyphony: 8 voices
- Up to 7 sawtooth oscillators per voice
- Detuning algorithm: Linear spread around center frequency
- CPU: Moderate (scales with voice count)

**Algorithm:**
```
For each voice:
  For each oscillator (1 to voiceCount):
    frequency = baseFrequency * (1 + detuneAmount * (oscillator - center))
    output += sawtooth(frequency)
  output /= voiceCount  // Normalize amplitude
```

**Parameters:**
- Detune (0-100%): Spread between oscillators
  - 0% = All oscillators at same pitch (mono saw)
  - 50% = Moderate chorus effect
  - 100% = Wide, shimmering sound
- Voices (1-7): Number of simultaneous oscillators
  - More voices = thicker sound, higher CPU usage
- Attack (1-1000ms): Fade-in time
- Release (10-5000ms): Fade-out time

**Best For:** Pads, leads, trance sounds, big chord stabs

**Tips:**
- Use 3-5 voices for optimal thickness/CPU balance
- Moderate detune (30-50%) for classic supersaw
- High detune (70-100%) for extreme chorus/ensemble effects
- Longer attack/release for pads

---

### 3. Chromatic Sampler
**Type:** Sample Playback Synth  
**Engine:** Pitch-shifted sample playback

**Technical Details:**
- Polyphony: 8 voices
- Playback rate = 2^(semitones/12)
- Supports standard audio formats
- ADSR envelope applied to sample playback

**Algorithm:**
```
playbackRate = pow(2.0, (midiNote - sampleRootNote) / 12.0)
phase += playbackRate / sampleRate
output = sample[phase] * envelope
```

**Parameters:**
- Sample: Select from available samples
- Attack: Fade-in time
- Release: Fade-out time  
- Level: Output volume

**Best For:** Melodic instruments, vocal chops, one-shots

**Tips:**
- Use short attack for percussive samples
- Use longer attack/release for pads and atmospheres
- Works great with single-cycle waveforms

---

### 4. FM Synth
**Type:** Frequency Modulation  
**Engine:** 2-operator FM synthesis

**Technical Details:**
- Polyphony: 8 voices
- Classic FM algorithm: Carrier + Modulator
- Both operators are sine waves
- Modulation index controls harmonic complexity

**Algorithm:**
```
carrier_freq = fundamental
modulator_freq = carrier_freq * ratio
modulator_output = sin(2π * modulator_freq * time)
carrier_input = 2π * carrier_freq * time + (index * modulator_output)
output = sin(carrier_input) * envelope
```

**Parameters:**
- Modulation Index (0-10): Harmonic richness
  - 0 = Pure sine wave (no modulation)
  - 1-3 = Subtle harmonic content
  - 4-7 = Rich, bell-like tones
  - 8-10 = Complex, noisy timbres
- Modulation Ratio (0.5-8.0): Frequency relationship
  - Integer ratios (1, 2, 3, 4) = Harmonic sounds
  - Non-integer ratios (1.5, 2.7) = Inharmonic, metallic sounds
  - 0.5 = Sub-octave modulation
- Attack (1-1000ms): Envelope attack
- Release (10-5000ms): Envelope release

**Best For:** Bells, electric pianos, basses, metallic sounds

**Tips:**
- Ratio = 1.0, Index = 2-4 for electric piano sounds
- Ratio = 3.5, Index = 5-8 for bell sounds
- Ratio = 0.5, Index = 1-2 for sub bass
- Modulate index over time for evolving timbres

**Famous FM Ratios:**
- 1:1 = Hollow, octave-rich sounds
- 1:2 = Clarinet-like sounds
- 1:3 = Square wave-ish sounds
- 2:3 = Metallic bells
- 4:5 = Bright, brassy sounds

---

### 5. Pulse Synth
**Type:** Pulse Width Modulation  
**Engine:** Variable pulse width square wave

**Technical Details:**
- Polyphony: 8 voices
- PWM oscillator with LFO modulation
- ADSR envelope for amplitude

**Algorithm:**
```
instantWidth = baseWidth + sin(2π * pwmRate * time) * pwmDepth
if (phase < instantWidth):
  output = +1.0
else:
  output = -1.0
output *= envelope
```

**Parameters:**
- Pulse Width (5-95%): Duty cycle
  - 50% = Perfect square wave (hollow sound)
  - 25% or 75% = Brighter, nasal sound
  - 10% or 90% = Thin, reedy sound
- PWM Rate (0.1-10 Hz): Modulation speed
  - 0.1-0.5 Hz = Slow, evolving sound
  - 1-3 Hz = Classic chorus effect
  - 5-10 Hz = Vibrato-like effect
- Attack (1-1000ms): Fade-in time
- Release (10-5000ms): Fade-out time

**Best For:** Bass, leads, classic analog-style sounds

**Tips:**
- Width = 50%, Rate = 0 for classic square wave bass
- Width = 25%, Rate = 2Hz for sweeping lead
- Combine with filter for classic analog sound
- Lower rates (0.5-2 Hz) for musical modulation

---

### 6. Wavetable Synth
**Type:** Wavetable Synthesis  
**Engine:** Wavetable scanning and interpolation

**Technical Details:**
- Polyphony: 8 voices
- Multiple pre-computed wavetables
- Position scanning and auto-scan LFO
- 256 samples per wavetable

**Algorithm:**
```
effectivePosition = tablePosition + sin(2π * scanRate * time) * scanDepth
wavetableIndex = floor(effectivePosition * numWavetables)
sample = wavetables[wavetableIndex][phase]
output = sample * envelope
```

**Parameters:**
- Table Position (0-100%): Manual wavetable selection
  - Scans through multiple waveforms
  - 0% = Sine wave
  - 100% = Complex waveform
- Scan Rate (0-5 Hz): Automatic scanning speed
  - 0 = Static position
  - 0.1-1 Hz = Slow evolution
  - 2-5 Hz = Vibrato/tremolo effect
- Attack (1-1000ms): Envelope attack
- Release (10-5000ms): Envelope release

**Included Wavetables:**
- Sine wave
- Sawtooth wave
- Square wave  
- Triangle wave
- Various complex waveforms

**Best For:** Evolving pads, modern synth sounds, timbral movement

**Tips:**
- Static position for consistent timbre
- Slow scan (0.1-0.5 Hz) for pads
- Fast scan (2-5 Hz) for vibrato effect
- Modulate position manually for filter-like sweeps

---

### 7. String Synth (Karplus-Strong)
**Type:** Physical Modeling  
**Engine:** Karplus-Strong plucked string algorithm

**Technical Details:**
- Polyphony: 8 voices
- Delay line with filtered feedback
- Noise burst excitation
- Physical modeling simulation

**Algorithm:**
```
// Initialization (note on):
delayLine.fill(whiteNoise)

// Per sample:
output = delayLine.read()
filtered = lowpass(output, brightness)
damped = filtered * (1 - damping)
delayLine.write(damped)
```

**Parameters:**
- Damping (0-100%): String decay rate
  - 0% = Very long sustain (unrealistic)
  - 30-50% = Guitar/harp-like
  - 70-100% = Short, percussive pluck
- Brightness (0-100%): High frequency content
  - 0% = Mellow, dark tone
  - 50% = Natural string sound
  - 100% = Bright, metallic tone
- Body Resonance (0-100%): Acoustic body simulation
  - 0% = Pure string
  - 50% = Acoustic guitar body
  - 100% = Large resonant body
- Release (10-5000ms): Envelope release time

**Best For:** Guitars, harps, plucked strings, kalimbas

**Tips:**
- Damping 40%, Brightness 60% for acoustic guitar
- Damping 20%, Brightness 80% for harp
- Damping 70%, Brightness 40% for bass
- High resonance for folk/acoustic instruments

**Physical Parameters:**
- String tension = Damping (inverse)
- String thickness = Brightness (inverse)
- Body size = Resonance

---

### 8. Vocoder Synth
**Type:** Multi-Band Vocoder  
**Engine:** 16-band spectral vocoding

**Technical Details:**
- Polyphony: 8 voices
- 16 frequency bands from 100 Hz to 8000 Hz
- Logarithmic band spacing
- Bandpass filters + envelope followers

**Algorithm:**
```
For each of 16 bands:
  carrier_band = bandpass(carrier_wave, band_center, band_width)
  modulator_band = bandpass(modulator_noise, band_center, band_width)
  envelope = envelope_follower(modulator_band)
  band_output = carrier_band * envelope * (1 + emphasis)
output = sum(all_bands) * envelope
```

**Parameters:**
- Carrier Waveform (1-4): Sound source
  - 1 = Sine wave (smooth, vowel-like)
  - 2 = Sawtooth (buzzy, harmonic-rich)
  - 3 = Square (hollow, robotic)
  - 4 = Noise (breathy, whispered)
- Band Emphasis (0-100%): Spectral enhancement
  - 0% = Flat frequency response
  - 50% = Moderate mid-range boost
  - 100% = Strong formant emphasis
- Attack (1-1000ms): Envelope attack
- Release (10-5000ms): Envelope release

**Frequency Bands (Hz):**
```
100, 150, 225, 337, 506, 759, 1139, 1708, 2562, 3843, 5765, 8647
```

**Best For:** Robotic voices, synth voices, sci-fi effects

**Tips:**
- Saw carrier with 50% emphasis for classic vocoder
- Noise carrier for whispered/breathy effects
- Square carrier for retro robot voices
- Sine carrier for smooth, vowel-like sounds

---

### 9. Phase Synth
**Type:** Phase Distortion Synthesis  
**Engine:** Casio CZ-style phase distortion

**Technical Details:**
- Polyphony: 8 voices
- Phase warping of sine wave
- Creates harmonics without FM complexity
- ADSR envelope

**Algorithm:**
```
phase = oscillator_phase + sin(oscillator_phase * 2π) * distortion * 0.3
waveform = sin(phase * 2π)
shaped = waveform + (waveform^3) * shape * 0.5
output = shaped * envelope
```

**Parameters:**
- Distortion Amount (0-100%): Phase warping intensity
  - 0% = Pure sine wave
  - 25% = Subtle harmonics
  - 50% = Resonant, formant-like sound
  - 100% = Complex, metallic timbre
- Shape (0-100%): Waveform character
  - 0% = Smooth distortion
  - 50% = Balanced
  - 100% = Aggressive waveshaping
- Attack (1-1000ms): Envelope attack
- Release (10-5000ms): Envelope release

**Best For:** Electric pianos, bells, evolving pads, basses

**Tips:**
- Distortion 30%, Shape 40% for electric piano
- Distortion 70%, Shape 60% for metallic bells
- Distortion 20%, Shape 80% for bass
- Evolving distortion creates filter-like sweeps

---

### 10. Digital Synth (Additive)
**Type:** Additive Synthesis  
**Engine:** Harmonic additive synthesis

**Technical Details:**
- Polyphony: 8 voices
- Up to 16 harmonics per voice
- Individual harmonic detuning
- Brightness filtering of upper harmonics
- 16 phase accumulators per voice

**Algorithm:**
```
output = 0
for h in 1 to numHarmonics:
  harmonic_freq = fundamental * h * (1 + h * 0.1 * detune)
  harmonic_amp = 1.0 / h  // Natural harmonic falloff
  harmonic_amp *= (1 - h/numHarmonics * (1 - brightness))
  output += sin(phase[h] * 2π) * harmonic_amp
output *= envelope / sqrt(numHarmonics)  // Normalize amplitude
```

**Parameters:**
- Harmonics (1-16): Number of partials
  - 1 = Pure sine wave
  - 2-4 = Simple waveforms
  - 5-8 = Complex tones
  - 9-16 = Very rich, organ-like sounds
- Detune (0-100%): Harmonic frequency spread
  - 0% = Perfect harmonic series
  - 25% = Slight chorusing
  - 50% = Detuned, unstable sound
  - 100% = Inharmonic, bell-like
- Brightness (0-100%): High harmonic filtering
  - 0% = Only fundamental (like low-pass filter)
  - 50% = Natural harmonic rolloff
  - 100% = All harmonics equal (bright)
- Release (10-5000ms): Envelope release

**Visual Feedback:**
- Displays harmonic spectrum as vertical bars
- Bar height = harmonic amplitude
- Updates in real-time with parameters

**Best For:** Organs, drawbar sounds, bells, synthetic tones

**Tips:**
- 8 harmonics, 0% detune, 50% brightness for organ
- 12 harmonics, 40% detune, 70% brightness for bells
- 4 harmonics, 0% detune, 30% brightness for clarinet-like
- 16 harmonics, 20% detune, 80% brightness for ensemble

**Harmonic Theory:**
- Odd harmonics (1,3,5,7) = Hollow/square-like
- Even harmonics (2,4,6,8) = Bright/sawtooth-like
- All harmonics = Full spectrum
- Detune creates chorus/ensemble effect

---

### 11. Cluster Synth (Granular)
**Type:** Granular Synthesis  
**Engine:** Grain cloud generator

**Technical Details:**
- Polyphony: 8 voices
- Up to 32 simultaneous grains per voice
- Hann window envelope per grain
- Pitch shifting and randomization
- Density-based grain spawning

**Algorithm:**
```
// Grain spawning (at density rate):
if (randomFloat() < density):
  grain = new Grain()
  grain.pitch = basePitch * pow(2, randomSpread)
  grain.duration = grainSize
  grain.amplitude = randomFloat(0.7, 1.0)

// Per grain, per sample:
grainPhase = grain.elapsed / grain.duration
window = 0.5 * (1 - cos(2π * grainPhase))  // Hann window
frequency = baseFrequency * pow(2, grain.pitch / 12)
sample = sin(2π * frequency * time) * window * amplitude
output = sum(all_active_grains) / numGrains
```

**Parameters:**
- Grain Size (10-500ms): Individual grain duration
  - 10-50ms = Granular, textural
  - 50-150ms = Rhythmic pulsing
  - 150-300ms = Melodic fragments
  - 300-500ms = Smooth, pad-like
- Density (10-100%): Grain spawn rate
  - 10% = Sparse, scattered grains
  - 50% = Moderate texture
  - 100% = Dense grain cloud (50 grains/sec max)
- Pitch (±12 semitones): Base grain pitch shift
  - -12 = One octave down
  - 0 = Original pitch
  - +12 = One octave up
- Spread (0-100%): Random pitch variation
  - 0% = All grains at same pitch
  - 25% = Slight detuning
  - 50% = Moderate cloud
  - 100% = Wide pitch spread (±2 octaves)

**Visual Feedback:**
- Shows active grain cloud
- Grain positions based on pitch and time
- Size indicates envelope progress
- Color/opacity indicates amplitude

**Best For:** Ambient textures, pads, atmospheres, soundscapes

**Tips:**
- Size 200ms, Density 50%, Spread 30% for pads
- Size 50ms, Density 80%, Spread 60% for granular texture
- Size 300ms, Density 30%, Spread 10% for sparse atmosphere
- Modulate density for rhythmic effects

**Grain Windows:**
- Hann window prevents clicks
- Smooth attack and release per grain
- Overlapping grains create continuous sound

**CPU Optimization:**
- Maximum 32 grains per voice
- Inactive grains are recycled
- Efficient overlap calculation

---

### 12. Voltage Synth (Analog Modeling)
**Type:** Subtractive Synthesis  
**Engine:** State Variable Filter with dual envelopes

**Technical Details:**
- Polyphony: 8 voices
- Three oscillator waveforms
- State Variable Filter (SVF) topology
- Dual ADSR envelopes (amplitude + filter)
- Filter envelope modulation

**Oscillator Algorithm:**
```
Sawtooth: phase * 2 - 1
Square: phase < 0.5 ? 1.0 : -1.0
Triangle: phase < 0.5 ? (phase * 4 - 1) : (3 - phase * 4)
```

**Filter Algorithm (SVF):**
```
g = tan(π * cutoff / sampleRate)  // Topology-preserving transform
G = g / (1 + g)

// Per sample:
v1 = (input - ic2eq) * G
v2 = ic1eq + v1
ic1eq = 2*v2 - ic1eq
v3 = v2 * G
ic2eq = ic2eq + 2*v3
lowpass = ic2eq
bandpass = v2
highpass = input - resonance*v2 - ic2eq
```

**Parameters:**
- Waveform (1-3): Oscillator shape
  - 1 = Sawtooth (bright, full harmonics)
  - 2 = Square (hollow, odd harmonics)
  - 3 = Triangle (soft, fewer harmonics)
- Cutoff (0-100%): Filter frequency
  - 0% = Very dark, sub-bass only
  - 30% = Warm, filtered
  - 60% = Open, natural
  - 100% = Bright, unfiltered
- Resonance (0-100%): Filter emphasis
  - 0% = Smooth slope
  - 40% = Moderate resonance
  - 70% = Strong peak
  - 100% = Self-oscillation
- Envelope Amount (0-100%): Filter modulation depth
  - 0% = Static filter
  - 50% = Moderate sweep
  - 100% = Full envelope modulation

**Envelopes:**
- **Amplitude ADSR**: Controls volume
  - Attack: 1-1000ms
  - Decay: 10-2000ms
  - Sustain: 0-100%
  - Release: 10-5000ms
- **Filter ADSR**: Controls cutoff (attack 0.5x amplitude)
  - Modulated by Envelope Amount parameter
  - Same ADSR values as amplitude

**Visual Feedback:**
- Shows selected waveform
- Displays cutoff position as horizontal line
- Updates in real-time

**Best For:** Classic analog sounds, bass, leads, pads

**Tips:**
- Saw + low cutoff + high resonance = classic bass
- Square + medium cutoff + low resonance = hollow lead
- Triangle + high cutoff + no resonance = soft pad
- High envelope amount for plucky, filtered sounds

**Classic Analog Sounds:**
- **TB-303 Bass**: Saw, Cutoff 20%, Resonance 70%, Env 80%
- **Moog Lead**: Saw, Cutoff 50%, Resonance 40%, Env 60%
- **Analog Pad**: Triangle, Cutoff 40%, Resonance 20%, Env 30%

---

### 13. DNA Synth (Wavetable Morphing)
**Type:** Advanced Wavetable Synthesis  
**Engine:** Dual wavetable morphing with mutation

**Technical Details:**
- Polyphony: 8 voices
- 8 pre-computed wavetables (256 samples each)
- Linear interpolation morphing
- Phase distortion mutation
- Real-time waveform display

**Wavetables:**
1. **Sine** - Pure fundamental
2. **Saw** - Bright, all harmonics
3. **Square** - Hollow, odd harmonics
4. **Triangle** - Soft, filtered harmonics
5. **Pulse** - Variable width pulse
6. **Stepped** - Staircase waveform
7. **Noisy** - Random component (generated)
8. **Harmonic** - Rich overtone series

**Morphing Algorithm:**
```
sampleA = wavetableA[phase * 256]
sampleB = wavetableB[phase * 256]
morphed = sampleA * (1 - morph) + sampleB * morph
```

**Mutation Algorithm (Phase Distortion):**
```
mutated_phase = phase + sin(phase * 2π * 3) * mutate * 0.3
output = interpolate(morphed_wavetable, mutated_phase)
```

**Parameters:**
- Table A (1-8): First wavetable source
- Table B (1-8): Second wavetable source
- Morph (0-100%): Crossfade between tables
  - 0% = Pure Table A
  - 50% = 50/50 blend
  - 100% = Pure Table B
- Mutate (0-100%): Phase distortion amount
  - 0% = Clean morphed waveform
  - 25% = Subtle harmonic addition
  - 50% = Moderate distortion
  - 100% = Extreme phase warping

**Visual Feedback:**
- Displays morphed waveform in real-time
- Shows result of morph + mutation
- Updates per parameter change

**Best For:** Evolving pads, complex leads, morphing textures

**Tips:**
- Sine→Saw morph for evolving brightness
- Square→Triangle for hollow→soft transition
- Use mutation to add movement to static morphs
- Morph 50%, Mutate 30% for organic evolution

**Morphing Examples:**
- Sine→Harmonic: Gentle to rich
- Saw→Square: Bright to hollow
- Triangle→Noisy: Clean to textured
- Pulse→Stepped: Smooth to digital

**Phase Distortion:**
- Adds harmonics without changing base waveform
- Creates formant-like resonances
- Triple frequency sine modulation
- Similar to Casio CZ synthesis

---

### 14. Drum Sampler
**Type:** Percussion Sample Playback  
**Engine:** Specialized drum sample engine

**Technical Details:**
- Optimized for one-shot percussion
- Pitch adjustment for tuning
- Envelope control for decay shaping
- Low-latency triggering

**Parameters:**
- Sample: Drum sample selection
- Pitch: Tuning adjustment
- Decay: Sample length control
- Level: Output volume

**Best For:** Drum kits, percussion, one-shots

---

## Effects Processors

### 1. Delay Effect
**Type:** Digital Delay Line  
**Algorithm:** Circular buffer with feedback

**Technical Details:**
```
delayBuffer[writePos] = input + feedback * delayBuffer[readPos]
readPos = (writePos - delaySamples) % bufferSize
output = dry * input + wet * delayBuffer[readPos]
```

**Parameters:**
- Time (0-2000ms): Delay length
  - Can be synced to BPM divisions
- Feedback (0-95%): Number of repeats
  - >95% can cause runaway feedback
- Mix (0-100%): Dry/wet balance
- Bypassed: Effect on/off

**Stereo Processing:**
- Separate delay lines for L/R
- Can create ping-pong effects

**Tips:**
- Short delays (10-50ms) for doubling/thickening
- Medium delays (100-300ms) for slapback
- Long delays (400-2000ms) for echoes
- Sync to BPM for rhythmic delays

---

### 2. Reverb Effect
**Type:** Algorithmic Reverb  
**Algorithm:** FDN (Feedback Delay Network)

**Technical Details:**
- Multiple delay lines with feedback matrix
- Allpass filters for diffusion
- Damping filters for frequency absorption
- Schroeder reverberator topology

**Parameters:**
- Size (0-100%): Room/hall dimensions
  - Controls delay line lengths
  - Larger = longer reverb tail
- Damping (0-100%): High frequency absorption
  - 0% = Bright, metallic reverb
  - 50% = Natural room
  - 100% = Dark, muffled reverb
- Mix (0-100%): Dry/wet balance

**Reverb Types by Size:**
- 0-25%: Small room
- 25-50%: Medium room/studio
- 50-75%: Large hall
- 75-100%: Cathedral/plate

**Tips:**
- Small size + low damping for bright spaces
- Large size + high damping for natural halls
- 100% wet mix for pure reverb (aux send)

---

### 3. Chorus Effect
**Type:** Modulated Delay  
**Algorithm:** Pitch-shifted delay with LFO

**Technical Details:**
```
lfo = sin(2π * rate * time)
delaySamples = baseDelay + lfo * depth * maxDepth
output = dry * input + wet * delayBuffer[delaySamples]
```

**Parameters:**
- Rate (0.1-5 Hz): LFO modulation speed
  - 0.1-0.5 Hz: Slow, subtle movement
  - 1-2 Hz: Classic chorus
  - 3-5 Hz: Vibrato-like effect
- Depth (0-100%): Modulation amount
  - Low depth = subtle thickening
  - High depth = pronounced detuning
- Mix (0-100%): Dry/wet balance

**Tips:**
- Rate 0.5Hz, Depth 40%, Mix 30% for classic chorus
- Rate 2Hz, Depth 60%, Mix 50% for ensemble
- Multiple chorus instances for richer effect

---

### 4. Filter Effect
**Type:** State Variable Filter  
**Algorithm:** SVF topology (LP/BP/HP)

**Technical Details:**
- Same SVF algorithm as Voltage synth
- Switchable filter modes
- Resonance up to self-oscillation

**Filter Modes:**
- **LP (Low Pass)**: Passes lows, cuts highs
  - Warmth, bass emphasis
- **BP (Band Pass)**: Passes middle, cuts lows/highs
  - Telephone/radio effect
- **HP (High Pass)**: Passes highs, cuts lows
  - Thin, bright sound

**Parameters:**
- Cutoff (20-20000 Hz): Filter frequency
- Resonance (0-100%): Filter emphasis
- Type: LP/BP/HP mode selection
- Bypassed: Effect on/off

**Tips:**
- LP mode with low cutoff for bass
- BP mode with resonance for vocal/formant effect
- HP mode to remove rumble
- High resonance for screaming filter sweeps

---

### 5. Compressor Effect
**Type:** Dynamic Range Compressor  
**Algorithm:** RMS detection with soft-knee

**Technical Details:**
```
// RMS Level Detection
rms = sqrt(average(input^2))
db = 20 * log10(rms)

// Gain Reduction
if (db > threshold):
  gainReduction = (db - threshold) * (1 - 1/ratio)
else:
  gainReduction = 0

// Envelope Smoothing
if (gainReduction > envelope):
  envelope += (gainReduction - envelope) / attack
else:
  envelope += (gainReduction - envelope) / release

gain = pow(10, -envelope / 20)
output = input * gain
```

**Parameters:**
- Threshold (-60 to 0 dB): Compression trigger
  - Higher = less compression
  - Lower = more compression
- Ratio (1:1 to 20:1): Compression amount
  - 2:1 = Gentle compression
  - 4:1 = Moderate compression
  - 10:1+ = Heavy limiting
- Attack (0.1-100ms): Response speed
  - Fast attack = catches transients
  - Slow attack = preserves punch
- Release (10-1000ms): Recovery time
  - Fast release = pumping effect
  - Slow release = smooth compression

**Compression Types:**
- **Gentle** (Threshold -20dB, Ratio 2:1): Transparent glue
- **Standard** (Threshold -15dB, Ratio 4:1): Moderate control
- **Heavy** (Threshold -10dB, Ratio 10:1): Aggressive limiting
- **Pumping** (Fast attack, Fast release): Rhythmic breathing

**Tips:**
- Use on master for loudness
- Use on drums for punch and sustain
- Slow attack, fast release for transient preservation
- Fast attack, slow release for sustain

---

### 6. Bitcrusher Effect
**Type:** Digital Degradation  
**Algorithm:** Bit depth and sample rate reduction

**Technical Details:**
```
// Bit Reduction
quantizationStep = 2.0 / pow(2, bitDepth)
crushedSample = floor(input / quantizationStep) * quantizationStep

// Sample Rate Reduction
if (sampleCounter % decimation == 0):
  holdSample = crushedSample
output = holdSample
```

**Parameters:**
- Bit Depth (1-16 bits): Resolution reduction
  - 16 bits = Clean (no effect)
  - 8 bits = Vintage digital
  - 4 bits = Very lo-fi
  - 1 bit = Extreme distortion
- Sample Rate (100-48000 Hz): Frequency reduction
  - 48000 Hz = Clean (no effect)
  - 8000-16000 Hz = Telephone quality
  - 1000-4000 Hz = Extreme aliasing
  - 100-500 Hz = Metallic distortion
- Mix (0-100%): Dry/wet balance

**Effects:**
- Lower bit depth = Quantization noise, distortion
- Lower sample rate = Aliasing, foldover
- Combined = Complex lo-fi character

**Tips:**
- 8-bit, 8kHz for retro game sounds
- 4-bit, 4kHz for aggressive lo-fi
- High mix for effect, low mix for subtle grit
- Automate for dramatic transitions

---

### 7. Distortion Effect
**Type:** Waveshaping Distortion  
**Algorithm:** Soft clipping with tone control

**Technical Details:**
```
// Soft Clipping
gained = input * (1 + drive * 10)
if (abs(gained) > 1.0):
  clipped = sign(gained) * (1 - exp(-abs(gained)))
else:
  clipped = gained

// Tone Control
filtered = lowpass(clipped, 1000 + tone * 19000)
output = dry * input + wet * filtered
```

**Parameters:**
- Drive (0-100%): Distortion amount
  - 0% = Clean
  - 25% = Mild overdrive
  - 50% = Distortion
  - 100% = Heavy fuzz
- Tone (0-100%): Frequency emphasis
  - 0% = Dark, bassy distortion
  - 50% = Balanced
  - 100% = Bright, cutting distortion
- Mix (0-100%): Dry/wet balance

**Distortion Types:**
- Low drive, high tone = Overdrive
- Medium drive, medium tone = Distortion
- High drive, low tone = Fuzz
- Any drive, 100% mix = Full saturation

**Tips:**
- Low drive for warmth and saturation
- High drive for aggressive sounds
- Low tone for bass/rhythm
- High tone for leads
- Parallel processing (50% mix) for natural punch

---

### 8. Phaser Effect
**Type:** Phase Shifting  
**Algorithm:** Cascaded allpass filters with LFO

**Technical Details:**
```
// LFO Modulation
lfo = sin(2π * rate * time)
modulatedFreq = centerFreq + lfo * depth * range

// Allpass Filter Chain (4 stages)
for each stage:
  a = (tan(π * modulatedFreq / sampleRate) - 1) / 
      (tan(π * modulatedFreq / sampleRate) + 1)
  output = a * input + state - a * output
  state = input

// Mix with Feedback
phased = allpassChain(input)
output = input + phased * feedback
```

**Parameters:**
- Rate (0.1-10 Hz): Sweep speed
  - 0.1-0.5 Hz: Slow, spacious sweep
  - 1-3 Hz: Classic phaser
  - 5-10 Hz: Fast, vibrato-like
- Depth (0-100%): Sweep range
  - Low depth = Subtle notches
  - High depth = Dramatic sweep
- Feedback (0-95%): Resonance amount
  - 0% = Mild phasing
  - 50% = Pronounced effect
  - 90%+ = Extreme resonance

**Phaser Types:**
- Slow rate, low depth, low feedback = Subtle movement
- Medium rate, medium depth, medium feedback = Classic phaser
- Fast rate, high depth, high feedback = Intense sweep

**Tips:**
- Rate 0.5Hz, Depth 50%, Feedback 40% for classic phaser
- Works great on guitars, keyboards, vocals
- Stack with other modulation effects
- Automate depth for dynamic sweeps

---

## Sequencer Algorithms

### 1. Euclidean Sequencer
**Algorithm:** Bjorklund's Euclidean rhythm algorithm

**Technical Details:**
```
// Euclidean Distribution
function euclidean(steps, pulses):
  pattern = []
  if (pulses == 0): return [0] * steps
  
  counts = []
  remainders = []
  divisor = steps - pulses
  remainders.append(pulses)
  level = 0
  
  while True:
    counts.append(divisor / remainders[level])
    remainders.append(divisor % remainders[level])
    divisor = remainders[level]
    level += 1
    if remainders[level] <= 1:
      break
  
  counts.append(divisor)
  build_pattern(pattern, level, counts, remainders)
  return rotate(pattern, rotation)
```

**Parameters:**
- Steps (1-32): Pattern length
- Pulses (1-32): Number of hits
- Rotation (0-31): Pattern offset
- Division (1-16): Note timing

**Famous Euclidean Patterns:**
- E(3,8) = Cuban tresillo
- E(5,8) = Cinquillo rhythm
- E(7,12) = Persian rhythm
- E(5,12) = South Indian rhythm

**Tips:**
- Equal steps/pulses = straight time
- Pulses < steps/2 = Sparse patterns
- Experiment with rotation for variations

---

### 2. Pattern Sequencer
**Type:** Step Sequencer  
**Algorithm:** Linear step progression

**Technical Details:**
- Array of note values per step
- Step-by-step playback
- Note-off between steps optional

**Parameters:**
- Length (1-32): Pattern steps
- Current Step: Edit position
- Note Value: MIDI note for step
- Division (1-16): Step timing

**Tips:**
- Use for bass lines and melodies
- Program chord progressions
- Combine with other sequencers on different tracks

---

### 3. Arpeggiator
**Algorithm:** Note order sorting and playback

**Modes:**
- **Up**: Lowest to highest
- **Down**: Highest to lowest
- **Up-Down**: Ping-pong pattern
- **Random**: Random note selection

**Parameters:**
- Mode: Play order
- Octaves (1-4): Octave range
- Division (1-16): Note speed
- Gate Length (1-100%): Note duration

**Algorithm:**
```
heldNotes = getCurrentlyHeldNotes()
if mode == UP:
  sort(heldNotes, ascending)
elif mode == DOWN:
  sort(heldNotes, descending)
elif mode == RANDOM:
  shuffle(heldNotes)

for octave in 1 to numOctaves:
  for note in heldNotes:
    playNote(note + octave * 12)
```

**Tips:**
- Hold chord to arpeggiate
- Use with pads and long releases
- Multiple octaves for range

---

### 4. Endless Sequencer
**Type:** Generative/Algorithmic  
**Algorithm:** Markov chain with mutation

**Technical Details:**
```
// Scale Quantization
notes = [0, 2, 4, 5, 7, 9, 11]  // Major scale
quantized = root + notes[random(0, 7)] + octave * 12

// Mutation
if (random() < mutationRate):
  currentNote = generateNewNote()
else:
  currentNote = varyCurrentNote()

// Play with probability
if (random() < density):
  playNote(currentNote)
```

**Parameters:**
- Mutation Rate (0-100%): Change frequency
- Scale Root (0-11): Key center
- Scale Type: Musical scale
- Division (1-16): Note timing

**Scales:**
- Major: 0,2,4,5,7,9,11
- Minor: 0,2,3,5,7,8,10
- Pentatonic: 0,2,4,7,9
- Blues: 0,3,5,6,7,10

**Tips:**
- Low mutation for repetitive patterns
- High mutation for chaotic sequences
- Use for background melodies
- Great with reverb/delay

---

### 5. Tombola Sequencer
**Type:** Probability Step Sequencer  
**Algorithm:** Per-step probability gates

**Technical Details:**
```
for step in pattern:
  if (random(0, 100) < step.probability):
    playNote(step.note)
    
advanceStep()
```

**Parameters:**
- Steps (4-32): Pattern length
- Current Step: Edit position
- Probability (0-100%): Trigger chance
- Division (1-16): Step timing

**Tips:**
- 100% probability = always triggers
- 50% probability = 50/50 chance
- Use for variation in drum patterns
- Combine certain and uncertain steps

---

### 6. Finger Sequencer
**Type:** Performance Recorder  
**Algorithm:** Real-time MIDI recording

**Technical Details:**
```
// Recording
if recording:
  for event in midiInput:
    if event.time < loopLength:
      recordedNotes.append(event)
    else:
      loopPosition = 0

// Playback
loopPosition += deltaTime
for note in recordedNotes:
  if note.time == loopPosition:
    playNote(note)
```

**Parameters:**
- Loop Length (1-64 beats): Recording length
- Overdub (On/Off): Layer recordings
- Quantize: Timing correction
- Clear/Record: Reset/capture

**Quantize Options:**
- Off: Free timing
- 1/16: Snap to 16th notes
- 1/8: Snap to 8th notes
- 1/4: Snap to quarter notes

**Tips:**
- Record short melodic phrases
- Use overdub for layering
- Quantize after recording for tight timing
- Clear between different ideas

---

### 7. Grid Sequencer
**Type:** Matrix Step Sequencer  
**Algorithm:** 2D grid-based sequencing

**Technical Details:**
```
// Grid Storage
grid[rows][columns] = boolean

// Playback with Swing
for column in 0 to columns:
  swingOffset = (column % 2) * swing
  timeToNextColumn = baseTime * (1 + swingOffset)
  
  for row in 0 to rows:
    if grid[row][column]:
      note = baseNote + (rows - 1 - row) * 2
      velocity = cellVelocity[row][column]
      playNote(note, velocity)
```

**Parameters:**
- Rows (4-16): Note range (pitch)
- Columns (8-32): Pattern length
- Division (1-16): Step timing
- Swing (0-75%): Timing offset

**Grid Mapping:**
- Each row = Different pitch
- Each column = Time step
- Cell on = Note triggers
- Cell off = Silent

**Swing Algorithm:**
```
Even steps: Play on time
Odd steps: Delayed by swing amount
Swing 0% = Straight time
Swing 50% = Triplet feel
Swing 75% = Maximum shuffle
```

**Tips:**
- More rows for melodic range
- More columns for longer patterns
- Use swing for groove
- Per-cell velocity for dynamics

---

### 8. Sketch Sequencer
**Type:** Timeline Drawer  
**Algorithm:** Position-based note triggering

**Technical Details:**
```
// Note Storage
struct DrawnNote {
  float position;  // 0-1 in loop
  int note;        // MIDI note
  float duration;  // Note length
  float velocity;  // 0-1
}

// Playback
loopPosition += deltaTime / loopLength
for note in drawnNotes:
  if (lastPosition <= note.position && 
      loopPosition > note.position):
    playNote(note.note, note.velocity)
    scheduleNoteOff(note.note, note.duration)
```

**Parameters:**
- Loop Length (4-64 beats): Timeline length
- Quantize: Grid snap options
- Note Range (12-48 st): Vertical zoom
- Base Note (36-84): Starting pitch

**Drawing:**
- Click to start note
- Drag to set duration
- Vertical position = Pitch
- Horizontal position = Time

**Quantization:**
- Snaps note start to grid
- Helps align to rhythm
- Off for free timing

**Tips:**
- Draw melodies like piano roll
- Quantize for tight timing
- Long notes for pads
- Short notes for rhythmic parts

---

## Signal Chain Architecture

### Complete Signal Path
```
MIDI Input
    ↓
Sequencer Module
    ↓ (MIDI Events)
Synth Module
    ↓ (Audio)
Track Effect Module
    ↓ (Processed Audio)
Tapedeck (Recording/Playback)
    ↓ (4 Track Audio)
Master Effect Module
    ↓ (Processed Mix)
Mixer Module
    ↓ (Final Mix: Level, Pan, Mute per track)
Audio Output
```

### Per-Track Processing
```
Track 1: Sequencer1 → Synth1 → TrackFX1 → Mixer
Track 2: Sequencer2 → Synth2 → TrackFX2 → Mixer
Track 3: Sequencer3 → Synth3 → TrackFX3 → Mixer
Track 4: Sequencer4 → Synth4 → TrackFX4 → Mixer
```

### Effect Routing
- **Master Effects**: Process final stereo mix
- **Track Effects**: Independent per track (4× effects)
- **Effect Order**: User-defined chain

### Timing and Synchronization
- **Master Clock**: BPM from metronome
- **Sample-Accurate Timing**: Event scheduling to sample precision
- **Sequencer Sync**: All sequencers locked to master clock
- **Division Values**: 1-16 (whole note to 1/16th)

---

## Advanced Techniques

### 1. Layering Techniques

**Synthesizer Layering:**
```
Track 1: Voltage (Saw, Bass) + Compressor
Track 2: FM (Bells) + Reverb  
Track 3: Cluster (Pad) + Chorus + Delay
Track 4: Digital (Organ) + Phaser
```

**Frequency Splitting:**
- Use Filter effect set to LP on bass tracks
- Use Filter effect set to HP on lead tracks
- Prevents frequency masking

### 2. Modulation Techniques

**Parameter Automation:**
- Morph DNA synth through performance
- Sweep Voltage cutoff for filter effects
- Vary Cluster density for evolution

**LFO Effects via Sequencers:**
- Use Endless sequencer with fast division for LFO-like modulation
- Use Sketch sequencer to draw modulation curves

### 3. Rhythmic Techniques

**Polyrhythms:**
```
Track 1: Euclidean (16, 7) - Division 4
Track 2: Euclidean (12, 5) - Division 3
Track 3: Pattern (8 steps) - Division 8
```

**Groove and Swing:**
- Use Grid sequencer swing (25-50%)
- Use Tombola with varying probabilities
- Layer Euclidean patterns with different rotations

### 4. Textural Techniques

**Ambient Soundscapes:**
```
Cluster synth (Large grains, Medium density)
+ Reverb (Large size, High damping)
+ Delay (Long time, Medium feedback)
+ Endless sequencer (Low mutation, Slow division)
```

**Granular Textures:**
```
Cluster synth (Small grains, High density)
+ Bitcrusher (8-bit, 8kHz)
+ Phaser (Slow rate, High depth)
```

### 5. Performance Techniques

**Live Recording Workflow:**
1. Set loop in/out points
2. Enable looping
3. Record first pass (bass)
4. Overdub second pass (chords)
5. Overdub third pass (melody)
6. Add effects per track
7. Mix and balance

**Generative Composition:**
1. Set up Endless sequencer on Track 1 (bass)
2. Set up Tombola sequencer on Track 2 (drums)
3. Use Grid sequencer on Track 3 (melody)
4. Let it run and record interesting sections
5. Use Finger sequencer to capture variations

### 6. Sound Design Techniques

**FM Programming:**
```
Electric Piano: Ratio 1.0, Index 2.5
Bell: Ratio 3.5, Index 6.0
Bass: Ratio 0.5, Index 1.5
Brass: Ratio 4.0, Index 7.0
```

**Additive Synthesis:**
```
Organ: 8 harmonics, 0% detune, 70% brightness
Bell: 12 harmonics, 40% detune, 80% brightness
Pad: 16 harmonics, 20% detune, 50% brightness
```

**Wavetable Morphing:**
```
Evolving Pad: Sine→Harmonic, Morph 50%, Mutate 30%
Aggressive Lead: Saw→Square, Morph 75%, Mutate 60%
Texture: Stepped→Noisy, Morph 40%, Mutate 80%
```

### 7. Mixing Techniques

**EQ with Filters:**
- HP filter on non-bass instruments (removes mud)
- LP filter on bass instruments (focuses low end)
- BP filter for special effects (telephone voice)

**Compression Strategies:**
- Light compression on master (Threshold -20dB, Ratio 2:1)
- Heavy compression on drums (Threshold -10dB, Ratio 8:1)
- Sidechain-style pumping (Fast attack, Fast release)

**Spatial Effects:**
- Reverb on melody tracks (20-40% mix)
- Delay on leads (30-50% mix, sync to BPM)
- Chorus on pads (30-60% mix, slow rate)

### 8. CPU Optimization

**Voice Allocation:**
- Each synth: 8 voices max
- Per-voice processing only when active
- Automatic voice stealing on new notes

**Effect Bypass:**
- Bypass unused effects to save CPU
- Effects process only when not bypassed
- Zero-latency bypass

**Efficient Sequencing:**
- Sample-accurate MIDI event timing
- Minimal overhead per note
- Optimized pattern storage

---

## Preset Examples

### Bass Sounds
```
1. Sub Bass
   - Voltage: Saw, Cutoff 15%, Resonance 0%, Env 0%
   - Filter: LP 80Hz
   
2. FM Bass
   - FM: Ratio 0.5, Index 1.5
   - Compressor: Threshold -15dB, Ratio 4:1

3. Pluck Bass
   - String: Damping 60%, Brightness 40%, Resonance 20%
   - Filter: LP 800Hz, Resonance 30%
```

### Lead Sounds
```
1. Analog Lead
   - Voltage: Saw, Cutoff 60%, Resonance 50%, Env 70%
   - Distortion: Drive 30%, Tone 70%

2. Digital Lead  
   - Digital: 8 Harmonics, Detune 20%, Brightness 80%
   - Phaser: Rate 0.5Hz, Depth 40%

3. FM Lead
   - FM: Ratio 2.0, Index 5.0
   - Delay: 375ms (dotted 8th at 120 BPM), Feedback 40%
```

### Pad Sounds
```
1. Ambient Pad
   - Cluster: Grain 200ms, Density 40%, Pitch 0, Spread 25%
   - Reverb: Size 80%, Damping 40%, Mix 50%
   - Delay: 500ms, Feedback 30%, Mix 20%

2. Analog Pad
   - Super Saw: 5 voices, Detune 35%
   - Chorus: Rate 0.3Hz, Depth 50%, Mix 40%
   - Reverb: Size 60%, Damping 50%, Mix 30%

3. Morphing Pad
   - DNA: Sine→Harmonic, Morph 50%, Mutate 30%
   - Phaser: Rate 0.2Hz, Depth 60%, Feedback 30%
```

### Pluck Sounds
```
1. Guitar
   - String: Damping 40%, Brightness 60%, Resonance 50%
   
2. Harp
   - String: Damping 20%, Brightness 80%, Resonance 30%
   - Reverb: Size 40%, Damping 30%, Mix 20%

3. Kalimba
   - String: Damping 70%, Brightness 40%, Resonance 60%
   - Delay: 150ms, Feedback 20%, Mix 30%
```

### Percussion Patterns
```
1. Basic Beat (Euclidean)
   - Track 1: E(4,16) - Kick
   - Track 2: E(2,16) Rotation 4 - Snare
   - Track 3: E(8,16) - Hi-hat

2. Polyrhythm (Euclidean)
   - Track 1: E(5,12) - Tom
   - Track 2: E(7,16) - Perc
   - Track 3: E(3,8) - Accent

3. Generative (Tombola)
   - Various probabilities: 100%, 60%, 30%
   - Creates variation and groove
```

---

## Troubleshooting

### Common Issues

**1. Distortion/Clipping:**
- Check mixer levels (reduce to 50-70%)
- Lower synth output levels
- Reduce feedback on delays/reverbs
- Use compressor to control peaks

**2. Thin Sound:**
- Layer multiple synths
- Add chorus/phaser for width
- Use reverb for depth
- Check filter cutoff (not too low)

**3. Muddy Mix:**
- Use HP filter on non-bass tracks
- Reduce reverb on bass instruments
- Check frequency overlap between tracks
- Use BP filter to carve space

**4. No Modulation:**
- Manually automate parameters
- Use sequencers creatively for modulation
- Layer multiple sequencers
- Use LFO-rate effects (chorus, phaser)

**5. Timing Issues:**
- Check metronome is running
- Verify BPM setting
- Check sequencer division settings
- Use quantization in Finger/Sketch sequencers

---

## Appendices

### A. MIDI Note Reference
```
C0 = 12, C1 = 24, C2 = 36, C3 = 48
C4 = 60 (Middle C), C5 = 72, C6 = 84
```

### B. Frequency Reference
```
A440 = 440 Hz (Standard tuning)
Each octave doubles frequency
Each semitone = 12th root of 2 (≈1.059)
```

### C. Division Reference (at 120 BPM)
```
Division 1 = 2000ms (whole note)
Division 2 = 1000ms (half note)
Division 4 = 500ms (quarter note)
Division 8 = 250ms (eighth note)
Division 16 = 125ms (sixteenth note)
```

### D. Musical Scales
```
Major: T T S T T T S (0,2,4,5,7,9,11)
Minor: T S T T S T T (0,2,3,5,7,8,10)
Dorian: T S T T T S T (0,2,3,5,7,9,10)
Phrygian: S T T T S T T (0,1,3,5,7,8,10)
Mixolydian: T T S T T S T (0,2,4,5,7,9,10)
Pentatonic: T T m3 T m3 (0,2,4,7,9)
Blues: m3 T S S m3 T (0,3,5,6,7,10)
```
Where T = Tone (2 semitones), S = Semitone, m3 = Minor 3rd (3 semitones)

---

For additional support and community discussion, visit the [TOP-1 Discord](https://discord.gg/4cV9Ucz).
