/**
 * MIDI-VCO
 * Arduino firmware interfacing between a MIDI keyboard and a set of VCOs.
 *
 * @author Yohan Chalier
 * @version 3.0.0-dev.1 2024-08-28
 */

#include <MIDI.h>
#include "include/config.h"
#include "include/allocator.h"
#include "include/structs.h"
#include "include/router.h"
#include "include/midi_interface.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Config *config;
Allocator *allocator;
Router *router;
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
    allocator->set_lane_masks();
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handle_note_on);
    MIDI.setHandleNoteOff(handle_note_off);
    MIDI.setHandlePitchBend(handle_pitch_bend);
    MIDI.setHandleControlChange(handle_control_change);
    midif = new MidiInterface(config, allocator, PIN_SS_BEND);
    midif->setup();
    pinMode(PIN_LED, OUTPUT);
    blink();
    old_led_state = false;
}

void loop()
{
    MIDI.read();
    update_config();
    router->update();
    update_led();
}

/**
 * Initial LED animation, triggered when the synthetizer starts.
 * It lasts for 500 milliseconds.
 */
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

void update_config()
{
    int changed = config->read();
    if (changed & CONFIG_CHANGE_POLYPHONY_MODE)
    {
        allocator->set_lane_masks();
    }
    if (changed & CONFIG_CHANGE_TUNING)
    {
        allocator->reset();
        if (config->is_tuning())
        {
            allocator->broadcast(PITCH_A5, GATE_STATE_DURING_TUNING);
        }
    }
}

void update_led()
{
    bool led_state = config->is_tuning() || allocator->is_active();
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
    old_led_state = led_state;
}

/**
 * @see MidiInterface.handle_note_on
 */
void handle_note_on([[maybe_unused]] byte channel, byte pitch, byte velocity)
{
    if (velocity == 0)
    {
        midif->handle_note_off(pitch);
    }
    else
    {
        midif->handle_note_on(pitch);
    }
}

/**
 * @see MidiInterface.handle_note_off
 */
void handle_note_off([[maybe_unused]] byte channel, byte pitch, [[maybe_unused]] byte velocity)
{
    midif->handle_note_off(pitch);
}

/**
 * @see MidiInterface.handle_pitch_bend
 */
void handle_pitch_bend([[maybe_unused]] byte channel, int bend)
{
    midif->handle_pitch_bend(bend);
}

/**
 * @see MidiInterface.handle_control_change
 */
void handle_control_change([[maybe_unused]] byte channel, byte number, byte value)
{
    midif->handle_control_change(number, value);
}
