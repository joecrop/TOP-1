// Digital Synth - Lo-fi bitcrushed synthesis
// Creates retro digital sounds with bit reduction and sample rate reduction

import("stdfaust.lib");

// Controls
key = hslider("/KEY", 69, 0, 127, 1);
velocity = hslider("/VELOCITY", 1, 0, 1, 0.01);
trigger = button("/TRIGGER");

// Parameters
bits = hslider("/BITS", 8, 2, 16, 1);              // Blue: Bit depth
sampleReduce = hslider("/SRATE", 1, 1, 32, 1);     // Green: Sample rate reduction
waveSel = hslider("/WAVE", 0, 0, 3, 1);            // White: Wave type (0=saw, 1=square, 2=tri, 3=noise)
glitch = hslider("/GLITCH", 0, 0, 1, 0.01);        // Red: Random glitch amount

// ADSR
attack = 0.01;
decay = 0.1;
sustain = 0.7;
release = 0.2;

// Convert MIDI note to frequency
freq = ba.midikey2hz(key);

// Envelope
env = en.adsr(attack, decay, sustain, release, trigger);

// Oscillators
sawOsc = os.sawtooth(freq);
squareOsc = os.square(freq);
triOsc = os.triangle(freq);
noiseOsc = no.noise;

// Waveform selection using ba.selectn
wave = ba.selectn(4, int(waveSel), sawOsc, squareOsc, triOsc, noiseOsc);

// Bit crushing
bitCrush(b, x) = floor(x * levels) / levels
with {
  levels = pow(2, b - 1);
};

// Sample rate reduction (sample and hold)
sampleHold(rate, x) = ba.sAndH(ba.pulse(ma.SR / rate), x);

// Apply bit crushing and sample reduction
crushed = bitCrush(bits, sampleHold(ma.SR / sampleReduce, wave));

// Glitch - random sample skipping
glitchNoise = no.noise : abs;
glitchGate = glitchNoise > (1 - glitch * 0.5);
glitched = crushed * (1 - glitch) + crushed * ba.sAndH(glitchGate, crushed) * glitch;

// Final output
output = glitched * env * velocity * 0.5;

process = output;
