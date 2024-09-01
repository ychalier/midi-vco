/**
 * MIDI-VCO
 * Arduino firmware interfacing between a MIDI keyboard and a set of VCOs.
 *
 * @author Yohan Chalier
 * @version 3.0.0-dev.1 2024-08-28
 */

#include <MIDI.h>
#include "include/allocator.h"
#include "include/channel.h"
#include "include/config.h"
#include "include/router.h"
#include "include/structs.h"
#include "include/tuner.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Allocator *allocator;
Config *config;
Channel *mod_channel;
Router *router;
Tuner *tuner;
bool previous_midi_led_state;
bool current_midi_led_state;
unsigned long midi_led_high_time_ms;

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
    pinMode(PIN_LED_MIDI, OUTPUT);
    blink();
    previous_midi_led_state = false;
    current_midi_led_state = false;
    midi_led_high_time_ms = 0;
}

void loop()
{
    MIDI.read();
    update_config();
    router->update();
    update_midi_led();
}

/**
 * Initial LED animation, triggered when the synthetizer starts.
 * It lasts for 500 milliseconds.
 */
void blink()
{
    digitalWrite(PIN_LED_MIDI, HIGH);
    delay(100);
    digitalWrite(PIN_LED_MIDI, LOW);
    delay(100);
    digitalWrite(PIN_LED_MIDI, HIGH);
    delay(100);
    digitalWrite(PIN_LED_MIDI, LOW);
    delay(100);
    digitalWrite(PIN_LED_MIDI, HIGH);
    delay(100);
    digitalWrite(PIN_LED_MIDI, LOW);
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

void set_midi_led_high()
{
    current_midi_led_state = true;
    if (previous_midi_led_state != current_midi_led_state)
    {
        digitalWrite(PIN_LED_MIDI, HIGH);
        previous_midi_led_state = true;
    }
    midi_led_high_time_ms = millis();
}

void update_midi_led()
{
    if (current_midi_led_state && (millis() - midi_led_high_time_ms > MIDI_LED_TIMEOUT))
    {
        current_midi_led_state = false;
        digitalWrite(PIN_LED_MIDI, LOW);
    }
    previous_midi_led_state = current_midi_led_state;
}

void handle_note_on([[maybe_unused]] byte channel, byte pitch, byte velocity)
{
    set_midi_led_high();
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
    set_midi_led_high();
    allocator->note_off({pitch, velocity});
}

void handle_pitch_bend([[maybe_unused]] byte channel, int bend)
{
    set_midi_led_high();
    allocator->pitch_bend(bend);
}

void handle_control_change([[maybe_unused]] byte channel, byte number, byte value)
{
    set_midi_led_high();
    if (number == MIDI_CONTROL_MOD)
    {
        mod_channel->set(map(value, 0, 127, 0, 4096));
    }
}
