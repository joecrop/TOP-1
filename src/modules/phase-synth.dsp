// Phase Synth - Phase Distortion synthesis
// Inspired by Casio CZ series synthesizers

import("stdfaust.lib");

// Controls
key = hslider("/KEY", 69, 0, 127, 1);
velocity = hslider("/VELOCITY", 1, 0, 1, 0.01);
trigger = button("/TRIGGER");

// Parameters
distortion = hslider("/DISTORT", 0.5, 0, 1, 0.01);    // Blue: Phase distortion amount
resonance = hslider("/RESO", 0.5, 0, 1, 0.01);        // Green: Resonance (window shape)
modDepth = hslider("/MODDEPTH", 0.3, 0, 1, 0.01);     // White: Mod envelope depth
modRate = hslider("/MODRATE", 2, 0.1, 10, 0.1);       // Red: Mod rate

// ADSR for amplitude
attack = 0.01;
decay = 0.15;
sustain = 0.7;
release = 0.3;

// Convert MIDI note to frequency
freq = ba.midikey2hz(key);

// Envelope
env = en.adsr(attack, decay, sustain, release, trigger);

// Modulation envelope for phase distortion
modEnv = en.adsr(0.001, modRate * 0.1, 0.3, modRate * 0.05, trigger);

// Phase accumulator (0 to 1 sawtooth)
phase = os.phasor(1, freq);

// Phase distortion function
// Creates different waveshapes by remapping the phase
phaseDistort(p, d) = select2(p < d, 
                              (p - d) / (2 * (1 - d)),  // Second half: slow rise
                              p / (2 * d) + 0.5)        // First half: fast rise
with {
  d = max(0.01, min(0.99, 0.5 + distortion * 0.4 * (1 + modEnv * modDepth)));
};

// Apply phase distortion
distortedPhase = phaseDistort(phase, 0.5 + distortion * 0.4);

// Convert to waveform (cosine for smooth sound)
wave1 = cos(distortedPhase * 2 * ma.PI);

// Resonance - adds a resonant peak using a windowed sine
resFreq = freq * (1 + resonance * 3);
resWave = os.osc(resFreq) * en.ar(0.001, 0.1 * (1 - resonance), trigger) * resonance;

// Combine waves
combined = wave1 * (1 - resonance * 0.5) + resWave * 0.5;

// Final output
output = combined * env * velocity * 0.5;

process = output;
