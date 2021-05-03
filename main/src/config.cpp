#include "Arduino.h"
#include "../include/config.h"

Config::Config()
{
    _polyphony_mode = MODE_MONOPHONIC;
    _priority_mode = PRIORITY_REPLACE_OLDEST;
    _channel_filter = CHANNEL_FILTER_OFF;
    _glide_flags = 0;
    _glide_intensity = 0;
    _pitch_bend_range = PITCH_BEND_RANGE;
    _glide_proportional = false;
}

void Config::setup()
{
    pinMode(PIN_SELECT_PRIORITY, INPUT);
    pinMode(PIN_SELECT_CHANNEL, INPUT);
}

byte Config::categorize_polyphony_mode(int input_value)
{
    if (input_value < 256)
    {
        return MODE_OCTOPHONIC;
    }
    else if (input_value < 512)
    {
        return MODE_QUADROPHONIC;
    }
    else if (input_value < 768)
    {
        return MODE_DUOPHONIC;
    }
    else
    {
        return MODE_MONOPHONIC;
    }
}

byte Config::categorize_priority_mode(int input_value)
{
    if (input_value == HIGH)
    {
        return PRIORITY_REPLACE_NEWEST;
    }
    else
    {
        return PRIORITY_REPLACE_OLDEST;
    }
}

byte Config::categorize_channel_filter(int input_value)
{
    if (input_value == HIGH)
    {
        return CHANNEL_FILTER_ON;
    }
    else
    {
        return CHANNEL_FILTER_OFF;
    }
}

bool Config::read()
{
    byte priority_mode = categorize_priority_mode(digitalRead(PIN_SELECT_PRIORITY));
    byte channel_filter = CHANNEL_FILTER_OFF;
    // byte channel_filter = categorize_channel_filter(digitalRead(PIN_SELECT_CHANNEL));
    byte polyphony_mode = categorize_polyphony_mode(analogRead(PIN_SELECT_MODE));
    bool changed = _priority_mode != priority_mode || _polyphony_mode != polyphony_mode || _channel_filter != channel_filter;
    _priority_mode = priority_mode;
    _polyphony_mode = polyphony_mode;
    _channel_filter = channel_filter;
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

byte Config::get_channel_filter()
{
    return _channel_filter;
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

void Config::handle_midi_control(byte channel, byte number, byte value)
{
    switch (number)
    {
    case MIDI_CONTROL_GLIDE_INTENSITY:
        if (value < 64)
        {
            _glide_intensity = 1.0 - ((float)value / 63.0);
            _glide_proportional = false;
        }
        else
        {
            _glide_intensity = (float)(value - 64) / 63.0;
            _glide_proportional = true;
        }
        break;    
    case MIDI_CONTROL_GLIDE_CHROMATIC:
        if (value < 64)
        {
            _glide_flags = _glide_flags - (_glide_flags & GLIDE_FLAG_CHROMATIC);
        }
        else
        {
            _glide_flags = _glide_flags | GLIDE_FLAG_CHROMATIC;
        }
        break;
    case MIDI_CONTROL_GLIDE_LEGATO:
        if (value < 64)
        {
            _glide_flags = _glide_flags - (_glide_flags & GLIDE_FLAG_LEGATO);
        }
        else
        {
            _glide_flags = _glide_flags | GLIDE_FLAG_LEGATO;
        }
        break;
    case MIDI_CONTROL_PITCH_BEND_RANGE:
        _pitch_bend_range = (float)value / 127.0 * 12.0;
        break;
    default:
        break;
    }
}

bool Config::is_glide_proportional()
{
    return _glide_proportional;
}