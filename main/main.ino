/**
 * MIDI-VCO
 * Arduino firmware interfacing between a MIDI keyboard and a set of VCOs.
 * 
 * @author Yohan Chalier
 * @version 0.3.0 2021-05-08
 */

#include <MIDI.h>
#include "include/config.h"
#include "include/allocator.h"
#include "include/structs.h"
#include "include/display.h"
#include "include/router.h"
#include "include/sequencer.h"
#include "include/arpeggiator.h"
#include "include/midi_interface.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Config *config;
Allocator *allocator;
Display *display;
Router *router;
Sequencer *sequencer;
Arpeggiator *arpeggiator;
MidiInterface *midif;

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
    sequencer = new Sequencer(config, allocator);
    arpeggiator = new Arpeggiator(config, allocator);
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handle_note_on);
    MIDI.setHandleNoteOff(handle_note_off);
    MIDI.setHandlePitchBend(handle_pitch_bend);
    midif = new MidiInterface(config, allocator, sequencer, arpeggiator);
    display->demo();
}

void loop()
{
    MIDI.read();
    midif->update();
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
    midif->handle_note_on(channel, pitch, velocity);
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
    midif->handle_note_off(channel, pitch, velocity);
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
    midif->handle_pitch_bend(channel, bend);
}

void handle_control_change(byte channel, byte number, byte value)
{
    midif->handle_control_change(channel, number, value);
}