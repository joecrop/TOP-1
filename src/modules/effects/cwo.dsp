// CWO Effect - Tape Wow and Flutter
// Creates vintage tape machine pitch wobble and warble effects

import("stdfaust.lib");

// Parameters
rate = hslider("/RATE", 2, 0.1, 10, 0.1);         // Blue: Modulation rate
depth = hslider("/DEPTH", 0.3, 0, 1, 0.01);       // Green: Modulation depth  
flutter = hslider("/FLUTTER", 0.2, 0, 1, 0.01);   // White: Flutter (faster wobble)
mix = hslider("/MIX", 0.5, 0, 1, 0.01);           // Red: Wet/dry mix

// Maximum delay for pitch modulation (in samples at 48kHz)
maxDelay = 4800;  // 100ms

// Wow - slow pitch variation
wowLFO = os.osc(rate) * 0.5 + 0.5;  // 0 to 1

// Flutter - faster random-ish variation
flutterLFOraw = (os.osc(rate * 6.3) + os.osc(rate * 7.1) + os.osc(rate * 11.7)) / 3;
flutterLFO = flutterLFOraw * 0.5 + 0.5;

// Combined modulation
modBase = wowLFO * (1 - flutter) + flutterLFO * flutter;
modulation = modBase * depth;

// Base delay and modulation amount
baseDelay = maxDelay / 2;

// Per-channel modulation (slight stereo offset)
modL = modulation;
modR = modulation * 0.97 + depth * 0.02;

// Delay times
delayL = baseDelay + modL * maxDelay * 0.4;
delayR = baseDelay + modR * maxDelay * 0.4;

// Smooth delays
smoothL = si.smoo(delayL);

// Mono processing with dry/wet mix
process = _ <: (_ * (1 - mix)), (de.fdelay(maxDelay, smoothL) * mix) :> _;
