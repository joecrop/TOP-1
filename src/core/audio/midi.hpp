#pragma once

#include <cmath>
#include <string>

#include "util/poly-ptr.hpp"

namespace top1::midi {

  struct MidiEvent {

    typedef unsigned char byte;

    enum EventType {
      NOTE_OFF = 0b1000,
      NOTE_ON = 0b1001,
      CONTROL_CHANGE = 0b1011,
    } type;

    int channel;
    byte *data;
    int time;

  };

  struct NoteOnEvent : public MidiEvent {
    int key;
    int velocity;

    NoteOnEvent(const MidiEvent& event) : MidiEvent(event), key(data[0]), velocity(data[1]) {};
    
    // Constructor for programmatic creation (e.g., from sequencer)
    NoteOnEvent(int note, int vel) : key(note), velocity(vel) {
      type = NOTE_ON;
      channel = 0;
      eventData[0] = note;
      eventData[1] = vel;
      data = eventData;
      time = 0;
    }
  private:
    byte eventData[2];  // Storage for programmatically created events
  };

  struct NoteOffEvent : public MidiEvent {
    int key = data[0];
    int velocity = data[1];

    NoteOffEvent(const MidiEvent& event) : MidiEvent(event) {};
  };

  struct ControlChangeEvent : public MidiEvent {
    int controler = data[0];
    int value = data[1];

    ControlChangeEvent(const MidiEvent& event) : MidiEvent(event) {};
  };

  // TODO: Replace with variant AnyMidiEvent
  using MidiEventPtr = top1::poly_ptr<MidiEvent,
    NoteOnEvent, NoteOffEvent, ControlChangeEvent>;

  inline float freqTable[128];

  constexpr void generateFreqTable(float tuning = 440) {
    for (int i = 0; i < 128; i++) {
      freqTable[i] = tuning * std::pow(2, float(i - 69)/float(12));
    }
  }

  // Get note name string from MIDI note number
  inline std::string noteName(int midiNote) {
    static const char* noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    if (midiNote < 0 || midiNote > 127) return "?";
    int octave = (midiNote / 12) - 1;
    int note = midiNote % 12;
    return std::string(noteNames[note]) + std::to_string(octave);
  }

}
