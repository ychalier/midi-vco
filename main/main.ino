#include <MIDI.h>
#include "config.h"
#include "allocator.h"
#include "note.h"
#include "display.h"


MIDI_CREATE_DEFAULT_INSTANCE();
Config* config;
Allocator* allocator;
Display* display;


void setup() {
    display = new Display();
    display->setup();
    config = new Config();
    config->setup();
    config->read();
    allocator = new Allocator(config);
    allocator->setup();
    allocator->set_masks();
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handle_note_on);
    MIDI.setHandleNoteOff(handle_note_off);
    MIDI.setHandlePitchBend(handle_pitch_bend);
    display->demo();
}


void loop() {
    MIDI.read();
    bool changed = config->read();
    if (changed) {
        allocator->set_masks();
        allocator->start_display(display);
    }
    display->update();
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