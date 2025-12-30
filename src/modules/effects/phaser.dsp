// Phaser Effect - Classic analog-style phaser
// Rate: LFO speed, Depth: sweep range, Stages: number of allpass stages, Mix

import("stdfaust.lib");

rate = hslider("rate", 0.5, 0.05, 4, 0.01);    // Blue: LFO rate Hz
depth = hslider("depth", 0.6, 0, 1, 0.01);      // Green: Sweep depth
stages = hslider("stages", 4, 2, 8, 1);         // White: Allpass stages (2-8)
mix = hslider("mix", 0.5, 0, 1, 0.01);          // Red: Dry/Wet

// LFO for sweep
lfo = os.osc(rate) * 0.5 + 0.5;

// Base frequency and sweep range
baseFreq = 200;
maxFreq = 3000;
sweepFreq = baseFreq + lfo * depth * (maxFreq - baseFreq);

// Single allpass filter stage
allpassStage(freq) = fi.allpassnn(1, (a)) with {
    // Coefficient for allpass based on frequency
    w = 2 * ma.PI * freq / ma.SR;
    a = (1 - sin(w)) / cos(w);
};

// Chain of allpass stages (fixed at 6 for compilation)
phaserChain = allpassStage(sweepFreq) : 
              allpassStage(sweepFreq * 1.3) : 
              allpassStage(sweepFreq * 1.6) : 
              allpassStage(sweepFreq * 2.0) :
              allpassStage(sweepFreq * 2.5) :
              allpassStage(sweepFreq * 3.0);

// Apply depth-controlled mixing of stages
applyStages(x) = x : phaserChain;

// Mix wet and dry with phase cancellation effect
process = _ <: (_, applyStages) : (*(1-mix), *(mix)) :> _;
