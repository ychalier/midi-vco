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
 * @see MidiInterface.handle_note_on
 */
void handle_note_on(byte channel, byte pitch, byte velocity)
{
    midif->handle_note_on(channel, pitch, velocity);
}

/**
 * @see MidiInterface.handle_note_off
 */
void handle_note_off(byte channel, byte pitch, byte velocity)
{
    midif->handle_note_off(channel, pitch, velocity);
}

/**
 * @see MidiInterface.handle_pitch_bend
 */
void handle_pitch_bend(byte channel, int bend)
{
    midif->handle_pitch_bend(channel, bend);
}

/**
 * @see MidiInterface.handle_control_change
 */
void handle_control_change(byte channel, byte number, byte value)
{
    midif->handle_control_change(channel, number, value);
}

/**
 * @see MidiInterface.handle_after_touch_poly
 */
void handle_after_touch_poly(byte channel, byte pitch, byte pressure)
{
    midif->handle_after_touch_poly(channel, pitch, pressure);
}

/**
 * @see MidiInterface.handle_after_touch_channel
 */
void handle_after_touch_channel(byte channel, byte pressure)
{
    midif->handle_after_touch_channel(channel, pressure);
}