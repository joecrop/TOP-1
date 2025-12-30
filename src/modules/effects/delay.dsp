// Simple delay effect for TOP-1
// Tempo-synced delay with feedback and mix controls

import("stdfaust.lib");

// Parameters controlled by the UI (4 knobs: Blue, Green, White, Red)
time = hslider("time", 0.5, 0.01, 2.0, 0.01);      // Delay time in seconds (Blue)
feedback = hslider("feedback", 0.3, 0.0, 0.95, 0.01); // Feedback amount (Green)
mix = hslider("mix", 0.5, 0.0, 1.0, 0.01);         // Dry/Wet mix (White)
tone = hslider("tone", 0.5, 0.0, 1.0, 0.01);       // Lowpass filter on feedback (Red)

// Maximum delay time in samples (2 seconds at 48kHz)
maxDelay = 96000;

// Simple lowpass filter for the delay feedback
lowpass(cutoff) = fi.lowpass(1, cutoff * 20000);

// Single delay line with feedback and filtering
delayLine = +~(de.delay(maxDelay, int(time * ma.SR)) : lowpass(tone) : *(feedback));

// Main process: mono in, mono out with dry/wet mix
process = _ <: (_, delayLine) : (*(1-mix), *(mix)) :> _;
