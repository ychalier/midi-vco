/**
 * MIDI-VCO
 * Arduino firmware interfacing between a MIDI keyboard and a set of VCOs.
 * 
 * @author Yohan Chalier
 * @version 0.4.0 2021-05-14
 */

#include <MIDI.h>
#include "include/config.h"
#include "include/allocator.h"
#include "include/structs.h"
#include "include/router.h"
#include "include/sequencer.h"
#include "include/arpeggiator.h"
#include "include/midi_interface.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Config *config;
Allocator *allocator;
Router *router;
Sequencer *sequencer;
Arpeggiator *arpeggiator;
MidiInterface *midif;
bool old_led_state;

void setup()
{
    config = new Config();
    config->setup();
    config->read();
    router = new Router(config);
    router->setup();
    allocator = new Allocator(config, router);
    allocator->set_masks();
    sequencer = new Sequencer(config, allocator);
    arpeggiator = new Arpeggiator(config, allocator);
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handle_note_on);
    MIDI.setHandleNoteOff(handle_note_off);
    MIDI.setHandlePitchBend(handle_pitch_bend);
    MIDI.setHandleControlChange(handle_control_change);
    MIDI.setHandleAfterTouchChannel(handle_after_touch_channel);
    MIDI.setHandleAfterTouchPoly(handle_after_touch_poly);
    midif = new MidiInterface(config, allocator, sequencer, arpeggiator, PIN_SS_BEND);
    midif->setup();
    pinMode(PIN_LED, OUTPUT);
    blink();
    old_led_state = false;
}

void loop()
{
    MIDI.read();
    midif->update();
    router->update();
    byte active_source = config->get_active_source();
    bool led_state = config->is_tuning()
        || (active_source == SOURCE_SEQUENCER && config->should_sequencer_record())
        || (active_source == SOURCE_ARPEGGIATOR && config->should_sequencer_record())
        || (active_source == SOURCE_DIRECT && allocator->is_active());
    if (active_source != SOURCE_SEQUENCER)
    {
        if (led_state != old_led_state)
        {
            if (led_state)
            {
                digitalWrite(PIN_LED, HIGH);
            }
            else
            {
                digitalWrite(PIN_LED, LOW);
            }
        }
    }
    old_led_state = led_state;
}

void blink()
{
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
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