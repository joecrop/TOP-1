// Cluster Synth - Granular synthesis
// Creates clouds of grains from oscillators

import("stdfaust.lib");

// Controls
key = hslider("/KEY", 69, 0, 127, 1);
velocity = hslider("/VELOCITY", 1, 0, 1, 0.01);
trigger = button("/TRIGGER");

// Parameters
grainSize = hslider("/GRAINSIZE", 50, 5, 200, 1);    // Grain size in ms
density = hslider("/DENSITY", 0.5, 0.1, 1, 0.01);     // Grain density
spread = hslider("/SPREAD", 0.3, 0, 1, 0.01);         // Pitch spread
texture = hslider("/TEXTURE", 0.5, 0, 1, 0.01);       // Grain shape/texture

// Convert MIDI note to frequency
freq = ba.midikey2hz(key);

// Envelope
env = en.adsr(0.01, 0.1, 0.8, 0.3, trigger);

// Grain generator - uses multiple detuned oscillators with random amplitude modulation
grainRate = 1000 / grainSize;

// Create multiple grain streams
grain(detune, phase) = os.osc(freq * (1 + detune * spread)) * 
                        (0.5 + 0.5 * os.osc(grainRate * (1 + phase * 0.3)));

// Multiple grain streams with different detunes and phases
grainCloud = (grain(0, 0) + 
              grain(0.02, 0.25) + 
              grain(-0.02, 0.5) + 
              grain(0.04, 0.75) +
              grain(-0.04, 0.33) +
              grain(0.01, 0.66)) / 6;

// Add texture - crossfade between smooth and noisy grains
noiseGrain = no.noise * env * 0.3;
texturedOutput = grainCloud * (1 - texture) + 
                 (grainCloud * (1 + noiseGrain * texture));

// Final output with envelope and velocity
output = texturedOutput * env * velocity * 0.4;

process = output;
