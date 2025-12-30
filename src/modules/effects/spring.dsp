// Spring Reverb Effect for TOP-1
// Simulates classic spring reverb tank sound

import("stdfaust.lib");

// Parameters (controlled from C++)
tension = hslider("/TENSION", 0.5, 0, 1, 0.01);  // Spring tension
mix = hslider("/MIX", 0.5, 0, 1, 0.01);          // Dry/wet mix  
decay = hslider("/DECAY", 0.6, 0, 1, 0.01);      // Decay time
tone = hslider("/TONE", 0.5, 0, 1, 0.01);        // High frequency damping

// Spring reverb simulation using comb filter network
// Springs have characteristic "boing" from multiple reflections

// Comb filter delays - prime-ish ratios for natural sound
delay1 = int((0.019 + tension * 0.01) * ma.SR);
delay2 = int((0.023 + tension * 0.012) * ma.SR);
delay3 = int((0.031 + tension * 0.015) * ma.SR);
delay4 = int((0.037 + tension * 0.018) * ma.SR);

// Allpass delays for diffusion
ap1d = int((0.0047 + tension * 0.002) * ma.SR);
ap2d = int((0.0071 + tension * 0.003) * ma.SR);

// Feedback amount based on decay
fb = 0.7 + decay * 0.25;

// Tone filter cutoff
toneCutoff = 2000 + tone * 6000;

// Allpass diffusers
allpass1 = fi.allpass_comb(4096, max(1, ap1d), 0.5);
allpass2 = fi.allpass_comb(4096, max(1, ap2d), 0.5);

// Individual comb filters with lowpass in feedback
comb1 = fi.fb_comb(8192, max(1, delay1), 1, -fb * 0.88) : fi.lowpass(1, toneCutoff);
comb2 = fi.fb_comb(8192, max(1, delay2), 1, -fb * 0.86) : fi.lowpass(1, toneCutoff);
comb3 = fi.fb_comb(8192, max(1, delay3), 1, -fb * 0.84) : fi.lowpass(1, toneCutoff);
comb4 = fi.fb_comb(8192, max(1, delay4), 1, -fb * 0.82) : fi.lowpass(1, toneCutoff);

// Comb bank - splits input to 4 parallel combs
combBank = _ <: comb1, comb2, comb3, comb4 :> _ / 4;

// Spring chirp - short decay high freq component
chirpDelay = int(0.003 * ma.SR);
springChirp = fi.fb_comb(1024, max(1, chirpDelay), 0.3, 0.2) : 
              fi.bandpass(2, 1500 + tension * 2000, 4000 + tension * 4000);

// Main spring reverb processor (mono)
springReverb = _ <: (
  // Dry path
  _ * (1 - mix),
  // Wet path: diffusion -> comb bank + chirp
  (_ : allpass1 : allpass2 : combBank) * mix * 0.85,
  (_ * 0.3 : springChirp) * mix * 0.15
) :> _;

// Mono processing
process = springReverb;
