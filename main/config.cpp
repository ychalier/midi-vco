#include "Arduino.h"
#include "config.h"
#include "constants.h"


Config::Config() {
    _polyphony_mode = MODE_MONOPHONIC;
    _priority_mode = PRIORITY_LAST_OLDEST;
    _midi_channel_mode = 0;
}


byte Config::get_priority_mode() {
    return _priority_mode;
}


byte Config::get_polyphony_mode() {
    return _polyphony_mode;
}


void Config::setup() {
    pinMode(PIN_SELECT_PRIORITY, INPUT);
    pinMode(PIN_SELECT_CHANNEL, INPUT);
}


byte categorize_polyphony_mode(int input_value) {
    if (input_value < 256) {
        return MODE_MONOPHONIC;
    } else if (input_value < 512) {
        return MODE_DUOPHONIC;
    } else if (input_value < 768) {
        return MODE_QUADROPHONIC;
    } else {
        return MODE_OCTOPHONIC;
    }
}


byte categorize_priority_mode(int input_value) {
    if (input_value == HIGH) {
        return PRIORITY_LAST_OLDEST;
    } else {
        return PRIORITY_LAST_YOUNGEST;
    }
}


byte categorize_midi_channel_mode(int input_value) {
    return 0;
}


bool Config::read() {
    byte priority_mode = categorize_priority_mode(digitalRead(PIN_SELECT_PRIORITY));
    byte midi_channel_mode = categorize_midi_channel_mode(digitalRead(PIN_SELECT_CHANNEL));
    byte polyphony_mode = categorize_polyphony_mode(analogRead(PIN_SELECT_MODE));
    bool changed = _priority_mode != priority_mode || _polyphony_mode != polyphony_mode || _midi_channel_mode != midi_channel_mode;
    _priority_mode = priority_mode;
    _polyphony_mode = polyphony_mode;
    _midi_channel_mode = midi_channel_mode;
    return changed;
}