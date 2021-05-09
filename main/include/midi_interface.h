#ifndef midi_interface_h
#define midi_interface_h

#include "Arduino.h"
#include <MIDI.h>
#include "config.h"
#include "sequencer.h"
#include "allocator.h"
#include "arpeggiator.h"

class MidiInterface
{
public:
    MidiInterface(Config *config, Allocator *allocator, Sequencer *sequencer, Arpeggiator *arpeggiator);
    void update();
    void handle_note_on(byte channel, byte pitch, byte velocity);
    void handle_note_off(byte channel, byte pitch, byte velocity);
    void handle_pitch_bend(byte channel, int bend);
    void handle_control_change(byte channel, byte number, byte value);

private:
    Config *_config;
    Allocator *_allocator;
    Sequencer *_sequencer;
    Arpeggiator *_arpeggiator;
};

#endif