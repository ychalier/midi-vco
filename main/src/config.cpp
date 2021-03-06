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
    _active_source = SOURCE_DIRECT;
    _sequencer_record = false;
    _arpeggiator_mode = ARPEGGIATOR_MODE_UP;
    _hold = false;
    _voltage_offset = 0;
    _time_period = 60000 / TIME_MIN_BPM;
    _arpeggiator_sustain = 1;
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

int Config::handle_midi_control(byte channel, byte number, byte value)
{
    int changed = 0;
    if (number == MIDI_CONTROL_GLIDE_INTENSITY)
    {
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
    }
    else if (number == MIDI_CONTROL_GLIDE_CHROMATIC)
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
    else if (number == MIDI_CONTROL_PITCH_BEND_RANGE)
    {
        _pitch_bend_range = round((float)value / 127.0 * 12.0 * 4.0) / 4.0;
    }
    else if (number == MIDI_CONTROL_SOURCE)
    {
        byte old_source = _active_source;
        if (value < 42)
        {
            _active_source = SOURCE_DIRECT;
        }
        else if (value < 84)
        {
            _active_source = SOURCE_ARPEGGIATOR;
        }
        else
        {
            _active_source = SOURCE_SEQUENCER;
        }
        if (old_source != _active_source)
        {
            changed = changed + CONFIG_CHANGE_SOURCE;
        }
    }
    else if (number == MIDI_CONTROL_SEQUENCER_RECORD)
    {
        bool old_sequencer_record = _sequencer_record;
        _sequencer_record = value >= 64;
        if (old_sequencer_record != _sequencer_record)
        {
            changed = changed + CONFIG_CHANGE_SEQUENCER_RECORD;
        }
    }
    else if (number == MIDI_CONTROL_TIME)
    {
        _time_period = 60000 / (TIME_MIN_BPM + (float)(TIME_MAX_BPM - TIME_MIN_BPM) * (float)value / 127.0);
    }
    else if (number == MIDI_CONTROL_ARPEGGIATOR_MODE)
    {
        if (value < 32)
        {
            _arpeggiator_mode = ARPEGGIATOR_MODE_UP;
        }
        else if (value < 64)
        {
            _arpeggiator_mode = ARPEGGIATOR_MODE_DOWN;
        }
        else if (value < 96)
        {
            _arpeggiator_mode = ARPEGGIATOR_MODE_UP_DOWN;
        }
        else
        {
            _arpeggiator_mode = ARPEGGIATOR_MODE_RANDOM;
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
    else if (number == MIDI_CONTROL_ARPEGGIATOR_SUSTAIN)
    {
        _arpeggiator_sustain = (float)value / 127.0;
    }
    return changed;
}

bool Config::is_glide_proportional()
{
    return _glide_proportional;
}

byte Config::get_active_source()
{
    return _active_source;
}

bool Config::should_sequencer_record()
{
    return _sequencer_record;
}

byte Config::get_arpeggiator_mode()
{
    return _arpeggiator_mode;
}

bool Config::get_hold()
{
    return _hold;
}

float Config::get_voltage_offset()
{
    return _voltage_offset;
}

unsigned long Config::get_time_period()
{
    return _time_period;
}

float Config::get_arpeggiator_sustain()
{
    return _arpeggiator_sustain;
}