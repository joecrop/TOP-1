// Spring Reverb effect for TOP-1
// Simple delay-based reverb with spring-like character

import("stdfaust.lib");

// Parameters controlled by the UI (4 knobs: Blue, Green, White, Red)
size = hslider("size", 0.5, 0.1, 1.0, 0.01);       // Room size (Blue)
damp = hslider("damp", 0.5, 0.0, 1.0, 0.01);       // Damping (Green)
mix = hslider("mix", 0.3, 0.0, 1.0, 0.01);          // Dry/Wet mix (White)
decay = hslider("decay", 0.5, 0.1, 0.95, 0.01);     // Decay/feedback (Red)

// Simple feedback delay reverb (Schroeder-style)
maxDelay = 48000;  // 1 second at 48kHz

// Delay times in samples (prime-ish numbers for diffusion)
d1 = int(1557 * size);
d2 = int(1617 * size);
d3 = int(1491 * size);
d4 = int(1422 * size);

// Simple comb filter: input + delayed*feedback
fbcomb(dt, fb) = (+ : de.delay(maxDelay, dt)) ~ (*(fb) : fi.lowpass(1, 8000 * (1-damp)));

// Simple allpass: Schroeder structure
sallpass(dt, g) = (+ : _ <: de.delay(maxDelay, dt), *(g)) ~ (*(-g)) : +;

// 4 parallel combs summed, then 2 allpasses in series
reverb4 = _ <: fbcomb(d1, decay), fbcomb(d2, decay), fbcomb(d3, decay), fbcomb(d4, decay) :> /(4);
reverb = reverb4 : sallpass(int(225 * size), 0.5) : sallpass(int(556 * size), 0.5);

// Main process: mono in, mono out with dry/wet mix
process = _ <: (*(1-mix), (reverb : *(mix))) :> _;
