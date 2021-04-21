#include <MIDI.h>
#include "config.h"
#include "allocator.h"
#include "note.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Config* config;
Allocator* allocator;

void setup() {
    config = new Config();
    allocator = new Allocator(config);
    allocator->setup();
    allocator->set_masks();
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handle_note_on);
    MIDI.setHandleNoteOff(handle_note_off);
    MIDI.setHandlePitchBend(handle_pitch_bend);
}

void loop() {
    MIDI.read();
}

void handle_note_on(byte channel, byte pitch, byte velocity) { 
    Note note = { channel, pitch };
    allocator->note_on(note);
}

void handle_note_off(byte channel, byte pitch, byte velocity) { 
    Note note = { channel, pitch };
    allocator->note_off(note);
}

void handle_pitch_bend(byte channel, int bend) {
    allocator->pitch_bend(channel, bend);
}