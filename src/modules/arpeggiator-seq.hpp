#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include "core/audio/midi.hpp"
#include <array>

namespace top1::modules {

  class ArpeggiatorSeq : public SequencerModule {

    ui::ModuleScreen<ArpeggiatorSeq>::ptr screen;

  public:

    struct Props : Properties {
      Property<int> rate     = {this, "RATE",     4,    {1, 16, 1}};  // Steps per beat
      Property<int> range    = {this, "RANGE",    1,    {1, 4, 1}};   // Octave range
      Property<int> pattern  = {this, "PATTERN",  1,    {1, 8, 1}};   // Arp pattern
      Property<float> gate   = {this, "GATE",     0.8,  {0.1, 1.0, 0.05}};
    } props;

    ArpeggiatorSeq();

    void process(const audio::ProcessData&) override;

    void display() override;
    
    void init() override;

  private:
    std::array<int, 16> heldNotes;  // Buffer of held notes
    int numHeldNotes = 0;
    int currentStep = 0;
    int samplesUntilNextStep = 0;
    bool noteIsOn = false;
    int samplesUntilNoteOff = 0;
    int currentNote = 0;
    
    void addNote(int note);
    void removeNote(int note);
    int getArpNote(int step);
    void advanceStep(audio::ProcessData& data);
    
    friend class ArpeggiatorSeqScreen;
  };
}
