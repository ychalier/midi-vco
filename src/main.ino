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
#include "include/channel.h"
#include "include/tuner.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Config *config;
Allocator *allocator;
Router *router;
Channel *mod_channel;
Tuner *tuner;
bool old_led_state;

void setup()
{
    config = new Config();
    config->setup();
    config->read();
    tuner = new Tuner();
    tuner->setup();
    router = new Router(config, tuner);
    router->setup();
    mod_channel = router->get_spare_channel();
    mod_channel->set(2048);
    allocator = new Allocator(config, router);
    allocator->set_lane_masks();
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handle_note_on);
    MIDI.setHandleNoteOff(handle_note_off);
    MIDI.setHandlePitchBend(handle_pitch_bend);
    MIDI.setHandleControlChange(handle_control_change);
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
    if ((changed & CONFIG_CHANGE_TUNING_FAST) && config->get_tuning_fast()) {
        tuner->tune_fast(allocator);
    }
    if (changed & CONFIG_CHANGE_TUNING_FULL)
    {
        //TODO
    }
    if ((changed & CONFIG_CHANGE_TUNING_RESET) && config->get_tuning_reset())
    {
        tuner->reset_tunings();
    }
}

void update_led()
{
    bool led_state = allocator->is_active();
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

void handle_note_on([[maybe_unused]] byte channel, byte pitch, byte velocity)
{
    if (velocity == 0)
    {
        allocator->note_off({pitch, velocity});
    }
    else
    {
        if (velocity < config->get_minimum_velocity())
        {
            velocity = config->get_minimum_velocity();
        }
        allocator->note_on({pitch, velocity});
    }
}

void handle_note_off([[maybe_unused]] byte channel, byte pitch, byte velocity)
{
    allocator->note_off({pitch, velocity});
}

void handle_pitch_bend([[maybe_unused]] byte channel, int bend)
{
    allocator->pitch_bend(bend);
}

void handle_control_change([[maybe_unused]] byte channel, byte number, byte value)
{
    if (number == MIDI_CONTROL_MOD)
    {
        mod_channel->set(map(value, 0, 127, 0, 4096));
    }
    int changed = config->handle_midi_control(number, value);
    if (changed & CONFIG_CHANGE_HOLD)
    {
        if (config->get_hold())
        {
            allocator->hold_on();
        }
        else
        {
            allocator->hold_off();
        }
    }
}
