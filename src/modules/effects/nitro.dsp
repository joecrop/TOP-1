// Nitro Effect - Compressor/Boost
// Adds punch and loudness to the signal

import("stdfaust.lib");

// Parameters
drive = hslider("/DRIVE", 0.5, 0, 1, 0.01);        // Blue: Input drive/gain
ratio = hslider("/RATIO", 4, 1, 20, 0.5);          // Green: Compression ratio
attack = hslider("/ATTACK", 0.01, 0.001, 0.1, 0.001);  // White: Attack time
release = hslider("/RELEASE", 0.1, 0.01, 1, 0.01);     // Red: Release time

// Threshold (fixed, but scaled by drive)
threshold = -20 + drive * 10;  // -20dB to -10dB

// Makeup gain (auto-calculated based on ratio)
makeupGain = 1 + (ratio - 1) * 0.1;

// Compressor implementation
compressor = co.compressor_mono(ratio, threshold, attack, release);

// Input drive (saturation-like)
inputDrive(x) = x * (1 + drive * 3) : ma.tanh;

// Output saturation for warmth
saturate(x) = x : ma.tanh;

// Full chain
nitro(x) = x : inputDrive : compressor : *(makeupGain) : saturate;

// Mono processing
process = nitro;
