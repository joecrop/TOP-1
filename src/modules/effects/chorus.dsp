// Chorus effect for TOP-1
// Stereo chorus with modulated delay

import("stdfaust.lib");

// Parameters (4 knobs: Blue, Green, White, Red)
rate = hslider("rate", 1.5, 0.1, 5.0, 0.01);      // Blue: LFO rate in Hz
depthMs = hslider("depth", 3.0, 0.5, 10.0, 0.1);  // Green: Depth in ms
mix = hslider("mix", 0.5, 0.0, 1.0, 0.01);        // White: Dry/wet mix
voices = hslider("voices", 2, 1, 4, 1);           // Red: Number of voices

// Base delay time
baseDelay = 0.015;  // 15ms base delay
maxDelay = 48000 * 0.05;  // 50ms max delay

// LFO for modulation
lfo1 = os.osc(rate);
lfo2 = os.osc(rate * 1.1);  // Slightly detuned for stereo
lfo3 = os.osc(rate * 0.9);

// Convert depth to samples
depthSamples = depthMs * 0.001 * ma.SR;
baseSamples = baseDelay * ma.SR;

// Chorus voice with modulated delay
chorusVoice(lfo) = de.fdelay(maxDelay, baseSamples + lfo * depthSamples);

// Multi-voice chorus
chorus = _ <: (
  chorusVoice(lfo1),
  chorusVoice(lfo2) * (voices > 1),
  chorusVoice(lfo3) * (voices > 2),
  chorusVoice(-lfo1) * (voices > 3)
) :> /(voices);

// Dry/wet mix
process = _ <: (*(1-mix), (chorus : *(mix))) :> _;
