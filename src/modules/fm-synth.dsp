// FM Synth - 2-operator FM synthesis for TOP-1
// Inspired by classic DX-style FM synthesis

import("stdfaust.lib");

// Parameters matching property names
RATIO = hslider("/RATIO", 2.0, 0.5, 8.0, 0.01);        // Blue: Modulator ratio
DEPTH = hslider("/DEPTH", 0.5, 0.0, 1.0, 0.01);        // Green: FM depth/index
ATTACK = hslider("/ATTACK", 0.01, 0.001, 1.0, 0.001);  // White: Attack time
RELEASE = hslider("/RELEASE", 0.3, 0.01, 2.0, 0.01);   // Red: Release time

// Gate and key from MIDI
TRIGGER = button("/TRIGGER");
KEY = hslider("/KEY", 69, 0, 127, 1);
VELOCITY = hslider("/VELOCITY", 1, 0, 1, 0.01);

freq = ba.midikey2hz(KEY);

// ADSR envelope
env = en.adsr(ATTACK, 0.1, 0.7, RELEASE, TRIGGER);

// FM synthesis
// Modulator oscillator
modFreq = freq * RATIO;
modIndex = DEPTH * 10.0 * env;  // Modulation index scales with envelope
modulator = modIndex * os.osc(modFreq);

// Carrier oscillator (frequency modulated by modulator)
carrier = os.osc(freq + modulator * freq);

// Output with envelope
process = carrier * env * VELOCITY * 0.5;
