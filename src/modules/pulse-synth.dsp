// Pulse Synth - Classic pulse wave with PWM for TOP-1
// Inspired by analog pulse/square wave synthesis

import("stdfaust.lib");

// Parameters matching property names
WIDTH = hslider("/WIDTH", 0.5, 0.05, 0.95, 0.01);      // Blue: Pulse width
PWM = hslider("/PWM", 0.0, 0.0, 10.0, 0.1);            // Green: PWM LFO rate
ATTACK = hslider("/ATTACK", 0.01, 0.001, 1.0, 0.001);  // White: Attack time
RELEASE = hslider("/RELEASE", 0.3, 0.01, 2.0, 0.01);   // Red: Release time

// Gate and key from MIDI
TRIGGER = button("/TRIGGER");
KEY = hslider("/KEY", 69, 0, 127, 1);
VELOCITY = hslider("/VELOCITY", 1, 0, 1, 0.01);

freq = ba.midikey2hz(KEY);

// ADSR envelope
env = en.adsr(ATTACK, 0.1, 0.8, RELEASE, TRIGGER);

// PWM LFO modulates pulse width
pwmLfo = os.osc(PWM) * 0.3;  // LFO for pulse width modulation
modulatedWidth = WIDTH + pwmLfo : max(0.05) : min(0.95);

// Pulse wave oscillator using pulsetrain
pulse = os.pulsetrain(freq, modulatedWidth);

// Sub oscillator one octave down (square wave)
sub = os.square(freq * 0.5) * 0.3;

// Mix and apply envelope
process = (pulse + sub) * env * VELOCITY * 0.4;
