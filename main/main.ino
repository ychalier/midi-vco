#include <MIDI.h>
#include "config.h"
#include "allocator.h"
#include "note.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Config* config;
Allocator* allocator;

void setup() {
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handler_note_on);
    MIDI.setHandleNoteOff(handler_note_off);
    config = new Config();
    allocator = new Allocator(config);
    allocator->setup();
    allocator->set_masks();
}

void loop() {
    MIDI.read();
}

void handler_note_on(byte channel, byte pitch, byte velocity) { 
    Note note = { channel, pitch };
    allocator->note_on(note);
}

void handler_note_off(byte channel, byte pitch, byte velocity) { 
    Note note = { channel, pitch };
    allocator->note_off(note);
}
