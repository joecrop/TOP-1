// String Synth - Karplus-Strong plucked string synthesis
// Damp: string damping, Decay: sustain time, Excite: excitation type, Detune

import("stdfaust.lib");

// Parameters matching property names
DAMP = hslider("/DAMP", 0.5, 0, 1, 0.01);        // Blue: String damping (brightness)
DECAY = hslider("/DECAY", 2, 0.1, 10, 0.1);      // Green: Decay time
EXCITE = hslider("/EXCITE", 0, 0, 2, 0.1);       // White: Excitation type (noise, pluck, bow)
SYMP = hslider("/SYMP", 0, 0, 0.3, 0.01);        // Red: Sympathetic string detune

// Gate and key from MIDI
TRIGGER = button("/TRIGGER");
KEY = hslider("/KEY", 69, 0, 127, 1);
VELOCITY = hslider("/VELOCITY", 1, 0, 1, 0.01);

freq = ba.midikey2hz(KEY);

// Excitation sources
noise_excite = no.noise * en.ar(0.001, 0.01, TRIGGER);
pluck_excite = os.impulse : fi.lowpass(1, 2000) * TRIGGER;
bow_excite = os.osc(freq * 2) * en.ar(0.01, 0.05, TRIGGER) * 0.3;

exciter = noise_excite, pluck_excite, bow_excite : ba.selectn(3, int(EXCITE));

// Main string with Karplus-Strong
mainString = pm.ks(freq, 1 - DAMP * 0.9, TRIGGER * 10 * exciter) * en.ar(0.001, DECAY, TRIGGER);

// Sympathetic strings (detuned)
sympString1 = pm.ks(freq * (1 + SYMP * 0.01), 1 - DAMP * 0.95, TRIGGER * 5 * exciter) 
              * en.ar(0.001, DECAY * 0.8, TRIGGER) * SYMP;
sympString2 = pm.ks(freq * (1 - SYMP * 0.01), 1 - DAMP * 0.95, TRIGGER * 5 * exciter)
              * en.ar(0.001, DECAY * 0.8, TRIGGER) * SYMP;

// Mix all strings
output = mainString + (sympString1 + sympString2) * 0.3;

process = output * VELOCITY * 0.5;
