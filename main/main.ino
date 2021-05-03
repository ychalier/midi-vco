/**
 * MIDI-VCO
 * Arduino firmware interfacing between a MIDI keyboard and a set of VCOs.
 * 
 * @author Yohan Chalier
 * @version 0.2.0 2021-04-29
 */

#include <MIDI.h>
#include "include/config.h"
#include "include/allocator.h"
#include "include/structs.h"
#include "include/display.h"
#include "include/router.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Config *config;
Allocator *allocator;
Display *display;
Router *router;

void setup()
{
    display = new Display();
    display->setup();
    config = new Config();
    config->setup();
    config->read();
    router = new Router(config, display);
    router->setup();
    allocator = new Allocator(config, display, router);
    allocator->set_masks();
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handle_note_on);
    MIDI.setHandleNoteOff(handle_note_off);
    MIDI.setHandlePitchBend(handle_pitch_bend);
    display->demo();
}

void loop()
{
    MIDI.read();
    bool changed = config->read();
    if (changed)
    {
        allocator->set_masks();
        allocator->display_state();
    }
    display->update();
    router->update();
}

/**
 * Handler for the MIDI note-on message.
 *
 * @param channel MIDI channel associated with the note.
 * @param pitch Note pitch in semitons.
 * @param velocity Velocity associated with the note, ignored here.
 */
void handle_note_on(byte channel, byte pitch, byte velocity)
{
    Note note = {channel, pitch};
    if (velocity == 0)
    {
        allocator->note_off(note);
    }
    else
    {
        allocator->note_on(note);
    }
}

/**
 * Handler for the MIDI note-off message.
 *
 * @param channel MIDI channel associated with the note.
 * @param pitch Note pitch in semitons.
 * @param velocity Velocity associated with the note, ignored here.
 */
void handle_note_off(byte channel, byte pitch, byte velocity)
{
    Note note = {channel, pitch};
    allocator->note_off(note);
}

/**
 * Handler for the MIDI pitch-bend message.
 *
 * @param channel MIDI channel to bend the pitch of.
 * @param bend Signed 14-bit encoding of the current position of the pitch-bend
 *     control.
 */
void handle_pitch_bend(byte channel, int bend)
{
    allocator->pitch_bend(channel, bend);
}

void handle_control_change(byte channel, byte number, byte value)
{
    config->handle_midi_control(channel, number, value);
}