// Drive effect for TOP-1
// Tube-style overdrive/distortion

import("stdfaust.lib");

// Parameters (4 knobs: Blue, Green, White, Red)
drive = hslider("drive", 0.5, 0.0, 1.0, 0.01);    // Blue: Drive amount
tone = hslider("tone", 0.5, 0.0, 1.0, 0.01);      // Green: Tone (bright/dark)
output = hslider("output", 0.5, 0.0, 1.0, 0.01);  // White: Output level
mix = hslider("mix", 1.0, 0.0, 1.0, 0.01);        // Red: Dry/wet mix

// Soft clipping distortion (tube-like)
// Using tanh for smooth saturation
softClip(x) = ma.tanh(x);

// Pre-gain based on drive
preGain = 1.0 + drive * 20.0;

// Tone control - simple lowpass/highpass blend
toneFilter = fi.lowpass(1, 2000 + tone * 8000);

// Distortion chain
distortion = *(preGain) : softClip : toneFilter : *(output);

// Dry/wet mix
process = _ <: (*(1-mix), (distortion : *(mix))) :> _;
