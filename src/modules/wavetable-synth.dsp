// Wavetable Synth - Morphing wavetable oscillator
// Position: table position (morphs between waveforms)
// Detune: adds detuned oscillator, Attack, Release

import("stdfaust.lib");

// Parameters matching property names
POSITION = hslider("/POSITION", 0, 0, 1, 0.01);  // Blue: Wavetable position
DETUNE = hslider("/DETUNE", 0, 0, 0.5, 0.01);    // Green: Detune amount
ATTACK = hslider("/ATTACK", 0.01, 0.001, 2, 0.001);
RELEASE = hslider("/RELEASE", 0.3, 0.01, 4, 0.01);

// Gate and key from MIDI
TRIGGER = button("/TRIGGER");
KEY = hslider("/KEY", 69, 0, 127, 1);
VELOCITY = hslider("/VELOCITY", 1, 0, 1, 0.01);

freq = ba.midikey2hz(KEY);

// Basic wavetable with 4 positions: sine, triangle, saw, square
// Position 0-0.25: sine
// Position 0.25-0.5: sine->triangle morph
// Position 0.5-0.75: triangle->saw morph  
// Position 0.75-1.0: saw->square morph

// Individual waveforms
sine(f) = os.osc(f);
triangle(f) = os.triangle(f);
saw(f) = os.sawtooth(f);
square(f) = os.square(f);

// Crossfade between waveforms based on position
wavetable(f, pos) = w1 * (1-morph) + w2 * morph
with {
    // Determine which waveforms to crossfade
    idx = int(pos * 3);  // 0, 1, 2, or 3
    morph = pos * 3 - idx;  // 0-1 within segment
    
    // Select waveforms based on position
    w1 = sine(f), triangle(f), saw(f), square(f) : ba.selectn(4, idx);
    w2 = sine(f), triangle(f), saw(f), square(f) : ba.selectn(4, min(idx+1, 3));
};

// Main oscillator with detune
osc1 = wavetable(freq, POSITION);
osc2 = wavetable(freq * (1 + DETUNE * 0.02), POSITION) * (DETUNE > 0);
osc3 = wavetable(freq * (1 - DETUNE * 0.02), POSITION) * (DETUNE > 0);

// Mix oscillators
oscillator = osc1 + (osc2 + osc3) * 0.5 * DETUNE : *(0.5);

// Envelope
env = en.adsr(ATTACK, 0.1, 0.8, RELEASE, TRIGGER);

process = oscillator * env * VELOCITY * 0.8;
