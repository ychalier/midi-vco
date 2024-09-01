#include "Arduino.h"
#include "../include/config.h"

Config::Config()
{
    _polyphony_mode = MODE_MONOPHONIC;
    _priority_mode = PRIORITY_REPLACE_OLDEST;
    _glide_flags = 0;
    _glide_intensity = 0;
    _pitch_bend_range = PITCH_BEND_RANGE;
    _glide_proportional = false;
    _hold = false;
    _voltage_offset = 0;
    _detune = 0;
    _tuning = false;
    _bend_channel_switch = false;
    _glide_channel_switch = false;
}

void Config::setup()
{
    pinMode(PIN_POLYPHONY_MODE, INPUT);
    pinMode(PIN_PITCH_BEND_RANGE, INPUT);
    pinMode(PIN_GLIDE_INTENSITY, INPUT);
    pinMode(PIN_DETUNE, INPUT);
    pinMode(PIN_PRIORITY_MODE, INPUT);
    pinMode(PIN_TUNE, INPUT);
    pinMode(PIN_BEND_CHANNEL_SWITCH, INPUT);
    pinMode(PIN_GLIDE_CHANNEL_SWITCH, INPUT);
}

bool Config::_read_polyphony_mode()
{
    int value = analogRead(PIN_POLYPHONY_MODE);
    int polyphony_mode = MODE_MONOPHONIC;
    if (value >= 682)
    {
        polyphony_mode = MODE_POLY10;
    }
    else if (value >= 341)
    {
        polyphony_mode = MODE_POLY5;
    }
    bool changed = _polyphony_mode != polyphony_mode;
    _polyphony_mode = polyphony_mode;
    return changed;
}

void Config::_read_pitch_bend_range()
{
    int value = analogRead(PIN_PITCH_BEND_RANGE);
    _pitch_bend_range = round((float)value / 1023.0 * 12.0 * 4.0) / 4.0;
}

void Config::_read_glide_intensity()
{
    int value = analogRead(PIN_GLIDE_INTENSITY);
    if (value < 512)
    {
        _glide_intensity = 1.0 - ((float)value / 511.0);
        _glide_proportional = false;
    }
    else
    {
        _glide_intensity = (float)(value - 512) / 511.0;
        _glide_proportional = true;
    }
}

void Config::_read_detune()
{
    int index = map(analogRead(PIN_DETUNE), 0, 1023, 0, DETUNE_VALUE_COUNT);
    _detune = DETUNE_VALUES[index];
}

bool Config::_read_priority_mode()
{
    int value = digitalRead(PIN_PRIORITY_MODE);
    byte priority_mode = PRIORITY_REPLACE_NEWEST;
    if (value == HIGH)
    {
        priority_mode = PRIORITY_REPLACE_OLDEST;
    }
    bool changed = priority_mode != _priority_mode;
    _priority_mode = priority_mode;
    return changed;
}

bool Config::_read_tuning()
{
    int value = digitalRead(PIN_TUNE);
    bool tuning = value == HIGH;
    bool changed = _tuning != tuning;
    _tuning = tuning;
    return changed;
}

void Config::_read_bend_channel_switch()
{
    int value = digitalRead(PIN_BEND_CHANNEL_SWITCH);
    _bend_channel_switch = value == HIGH;
}

void Config::_read_glide_channel_switch()
{
    int value = digitalRead(PIN_GLIDE_CHANNEL_SWITCH);
    _glide_channel_switch = value == HIGH;
}

void Config::_read_minimum_velocity()
{
    _minimum_velocity = map(analogRead(PIN_MINIMUM_VELOCITY), 0, 1023, 0, 127);
}

int Config::read()
{
    int changed = 0;
    if (_read_polyphony_mode())
    {
        changed = changed + CONFIG_CHANGE_POLYPHONY_MODE;
    }
    if (_read_priority_mode())
    {
        changed = changed + CONFIG_CHANGE_PRIORITY_MODE;
    }
    if (_read_tuning())
    {
        changed = changed + CONFIG_CHANGE_TUNING;
    }
    _read_bend_channel_switch();
    _read_glide_channel_switch();
    _read_pitch_bend_range();
    _read_glide_intensity();
    _read_detune();
    _read_minimum_velocity();
    return changed;
}

byte Config::get_polyphony_mode()
{
    return _polyphony_mode;
}

byte Config::get_priority_mode()
{
    return _priority_mode;
}

byte Config::get_glide_flags()
{
    return _glide_flags;
}

float Config::get_glide_intensity()
{
    return _glide_intensity;
}

float Config::get_pitch_bend_range()
{
    return _pitch_bend_range;
}

int Config::handle_midi_control(byte number, byte value)
{
    int changed = 0;
    if (number == MIDI_CONTROL_GLIDE_CHROMATIC)
    {
        if (value < 64)
        {
            _glide_flags = _glide_flags - (_glide_flags & GLIDE_FLAG_CHROMATIC);
        }
        else
        {
            _glide_flags = _glide_flags | GLIDE_FLAG_CHROMATIC;
        }
    }
    else if (number == MIDI_CONTROL_GLIDE_LEGATO)
    {
        if (value < 64)
        {
            _glide_flags = _glide_flags - (_glide_flags & GLIDE_FLAG_LEGATO);
        }
        else
        {
            _glide_flags = _glide_flags | GLIDE_FLAG_LEGATO;
        }
    }
    else if (number == MIDI_CONTROL_HOLD)
    {
        bool old_hold = _hold;
        _hold = value >= 64;
        if (old_hold != _hold)
        {
            changed = changed + CONFIG_CHANGE_HOLD;
        }
    }
    else if (number == MIDI_CONTROL_VOLTAGE_OFFSET)
    {
        _voltage_offset = (float)value / 127.0;
    }
    return changed;
}

bool Config::is_glide_proportional()
{
    return _glide_proportional;
}

bool Config::get_hold()
{
    return _hold;
}

float Config::get_voltage_offset()
{
    return _voltage_offset;
}

int Config::get_detune()
{
    return _detune;
}

bool Config::is_tuning()
{
    return _tuning;
}

bool Config::get_bend_channel_switch()
{
    return _bend_channel_switch;
}

bool Config::get_glide_channel_switch()
{
    return _glide_channel_switch;
}

byte Config::get_minimum_velocity()
{
    return _minimum_velocity;
}
