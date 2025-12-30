// Filter effect for TOP-1
// Resonant lowpass filter with envelope follower

import("stdfaust.lib");

// Parameters (4 knobs: Blue, Green, White, Red)
cutoff = hslider("cutoff", 1000, 100, 10000, 1);  // Blue: Cutoff frequency
resonance = hslider("reso", 0.5, 0.0, 0.95, 0.01); // Green: Resonance (limited for stability)
envAmount = hslider("env", 0.0, -1.0, 1.0, 0.01);  // White: Envelope amount
mix = hslider("mix", 1.0, 0.0, 1.0, 0.01);         // Red: Dry/wet mix

// Simple envelope follower
envFollow = abs : si.smooth(ba.tau2pole(0.01));

// Resonant lowpass using simple biquad (more stable)
q = 0.5 + resonance * 10;

// Filter function - wrap everything properly
filterProc(sig) = sig : fi.resonlp(fc, q, 1) with {
  // Calculate envelope from signal
  envVal = sig : envFollow;
  // Dynamic cutoff modulated by envelope
  fc = cutoff + envVal * envAmount * 5000 : max(100) : min(10000);
};

// Process with dry/wet mix
process = _ <: (*(1-mix), (filterProc : *(mix))) :> _;
