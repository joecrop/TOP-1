# Getting Started with TOP-1

This guide will help you get up and running with TOP-1, a virtual synthesizer workstation inspired by the Teenage Engineering OP-1.

## Quick Start

### Launching TOP-1

1. Make sure JACK audio is running on your system
2. Run `./bin/top-1` from the build directory
3. TOP-1 will connect to JACK automatically at 48kHz

### Basic Navigation

TOP-1 has several main views accessible via keyboard shortcuts:

| Key | View |
|-----|------|
| **Ctrl+U** | Synth |
| **Ctrl+I** | Effect |
| **Ctrl+O** | Sequencer |
| **Ctrl+T** | Tape Deck |
| **Ctrl+Y** | Mixer |
| **Ctrl+G** | Metronome |
| **Ctrl+J** | Drums |

### Changing Modules (Synths, Effects, Sequencers)

To switch between different synth engines, effects, or sequencers:

1. **Hold Shift first**
2. **While holding Shift**, press the view key:
   - **Ctrl+U** for synths
   - **Ctrl+I** for effects
   - **Ctrl+O** for sequencers
3. A selector menu appears showing available modules
4. Use the **Blue encoder** (↑/↓ arrow keys) to scroll through options
5. **Press the view key again without Shift** to confirm and view the selected module

**Important:** You must hold Shift *before* pressing the view key to open the selector.

### Playing Notes

Use your computer keyboard as a piano:

```
  W E   T Y U
 A S D F G H J
```

- Bottom row (A-J): White keys (C, D, E, F, G, A, B)
- Top row (W, E, T, Y, U): Black keys (C#, D#, F#, G#, A#)
- Use **,** and **.** to shift octaves down/up

### The Four Colored Encoders

Every module in TOP-1 is controlled by four virtual encoders, each with a distinct color:

| Color | Function |
|-------|----------|
| **Blue** | Primary parameter (pitch, time, frequency) |
| **Green** | Secondary parameter (modulation depth, feedback) |
| **White** | Tertiary parameter (tone, shape, character) |
| **Red** | Level/Mix (volume, wet/dry, output) |

The encoders map to your arrow keys or can be clicked/dragged in the UI.

## Building Your First Sound

### Step 1: Choose a Synth

1. Hold **Shift**, then press **Ctrl+U** to open the synth selector
2. Use **↑/↓ arrows** (Blue encoder) to browse synths:
   - **Nuke** - Nuclear-powered bass/lead
   - **SuperSaw** - Stacked detuned saws
   - **FM** - FM synthesis
   - **Cluster** - Granular textures
   - **And more...**
3. Release **Shift** to confirm selection
4. Press **Ctrl+U** (without Shift) anytime to return to the synth view

### Step 2: Shape the Sound

Use the four encoders to shape your synth:
- **Blue**: Usually pitch or fundamental frequency
- **Green**: Often detune or modulation
- **White**: Often filter or harmonic content
- **Red**: Usually envelope or output level

### Step 3: Add an Effect

1. Hold **Shift**, then press **Ctrl+I** to open the effect selector
2. Use **↑/↓ arrows** to browse effects:
   - **Delay** - Echo/delay
   - **Reverb** - Space and ambience
   - **Chorus** - Thickening/detuning
   - **Drive** - Distortion/saturation
   - **CWO** - Tape wobble
   - **And more...**

### Step 4: Create a Sequence

1. Hold **Shift**, then press **Ctrl+O** to open the sequencer selector
2. Use **↑/↓ arrows** to choose a sequencer type:
   - **Pattern** - Step sequencer (16 steps)
   - **Arpeggiator** - Automatic arpeggiation
   - **Endless** - Generative patterns

#### Using the Pattern Sequencer
The pattern sequencer is a 16-step grid that you program using MIDI input.

**Edit Mode (when stopped):**
1. Use **Blue encoder** to move the cursor column left/right
2. Play notes on your **MIDI keyboard** - they populate the current column
3. Use **Shift + Blue** to change pattern length (1-16 steps)

**Record Mode (when playing):**
1. Press **Space** to start the sequencer
2. The cursor advances automatically with the beat
3. Play notes on your **MIDI keyboard** to record into the grid in real-time
4. Press **Space** again to stop

**Other controls:**
- **Green encoder**: Change root note
- **White encoder**: Adjust swing
- **Red encoder**: Change scale (Chromatic/Major/Minor/Pentatonic/Blues/Drums)

#### Using the Arpeggiator
1. Hold down multiple keys on your MIDI keyboard
2. The arpeggiator plays them in sequence
3. Adjust **Blue** for mode, **Green** for octave range

#### Using the Endless Sequencer
1. Set **Blue** for density (note probability)
2. Set **Green** for range (octave span)
3. Set **White** for complexity
4. Press play and let it generate patterns!

## Working with the Tape Deck

TOP-1 includes a virtual 4-track tape deck for recording and playback.

### Recording
1. Select a track (1-4)
2. Arm recording (check your key bindings)
3. Play notes - they'll be recorded to tape
4. Stop recording when done

### Tape Speed (Varispeed)
The tape deck includes varispeed control:
- Adjust playback speed from 0.5x to 2.0x
- Creates pitch-shifting and time-stretching effects
- Great for creative sound design

## LFO Modulation

TOP-1 includes two global LFOs that can modulate synth parameters:

### Accessing LFO Settings
The LFO section appears in the synth view. Two LFOs are available:

- **LFO 1**: Routed via the Mod Matrix
- **LFO 2**: Routed via the Mod Matrix

### LFO Parameters
- **Rate**: Speed of oscillation (0.1 - 20 Hz)
- **Shape**: Wave shape (sine, triangle, square, saw, random)
- **Depth**: Modulation amount

### Mod Matrix
The Mod Matrix (Shift+M or access via menu) lets you assign LFO modulation to any parameter:
1. Select a slot
2. Choose source (LFO1, LFO2, or Envelope)
3. Choose destination parameter
4. Set modulation depth (positive or negative)

## Tips and Tricks

### Sound Design Tips

1. **Layer sounds**: Use all 4 tracks with different synths
2. **Offset sequencers**: Start sequences at different times for polyrhythms
3. **Automate with LFO**: Route LFOs to filter cutoff for movement
4. **Use tape speed**: Record at half speed, play back at normal for octave-up sounds

### Performance Tips

1. **Endless Sequencer in Markov mode**: Learns patterns as you play
2. **Arpeggiator + Hold**: Set notes and let it run
3. **Effect automation**: Different effects per track for variety

### Creative Ideas

1. **Granular textures**: Cluster synth with small grain size + Spring reverb
2. **Lo-fi beats**: Digital synth (bitcrushed) + CWO (tape wobble)
3. **Ambient pads**: String synth + Delay (high feedback) + Reverb
4. **Acid bass**: FM or Pulse synth + Filter effect + Pattern sequencer

## Keyboard Reference

### Global Keys
| Key | Action |
|-----|--------|
| **Ctrl+U** | Synth view |
| **Ctrl+I** | Effect view |
| **Ctrl+O** | Sequencer view |
| **Ctrl+T** | Tape Deck view |
| **Ctrl+Y** | Mixer view |
| **Ctrl+G** | Metronome view |
| **Ctrl+J** | Drums view |
| **Shift + (view key)** | Open module selector (hold Shift first!) |
| **Space** | Play/Stop |
| **,** / **.** | Octave down/up |

### Piano Keys
```
  W E   T Y U
 A S D F G H J
 (C D E F G A B)
```

### Encoder Controls
| Key | Encoder |
|-----|---------|
| ↑/↓ | Blue encoder |
| ←/→ | Green encoder |
| (varies) | White/Red encoders |

## Troubleshooting

### No Sound?
1. Check JACK is running: `jack_lsp`
2. Check connections in a JACK patchbay (QjackCtl, Carla)
3. Make sure TOP-1 is connected to your audio output
4. Check track isn't muted

### Crackling/Clicks?
1. Increase JACK buffer size (try 2048 samples)
2. Lower CPU load by using fewer effects
3. Check for high system load

### MIDI Not Working?
1. Connect your MIDI device before starting TOP-1
2. Check MIDI connections in JACK
3. TOP-1 receives on all MIDI channels

## Next Steps

- Read the [Module Reference](modules.md) for detailed parameter info
- Check the [Design Document](design.md) for architecture details
- Explore and experiment - happy synthesizing!
