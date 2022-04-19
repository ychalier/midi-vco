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
    _button_sequencer_record = HIGH;
    _arpeggiator_mode = ARPEGGIATOR_MODE_UP;
    _hold = false;
    _voltage_offset = 0;
    _time_period = 60000 / TIME_MIN_BPM;
    _arpeggiator_sustain = 1;
    _detune = 0;
    _tuning = false;
}

void Config::setup()
{
    pinMode(PIN_SOURCE, INPUT);
    pinMode(PIN_POLYPHONY_MODE, INPUT);
    pinMode(PIN_PITCH_BEND_RANGE, INPUT);
    pinMode(PIN_GLIDE_INTENSITY, INPUT);
    pinMode(PIN_ARPEGGIATOR_MODE, INPUT);
    pinMode(PIN_DETUNE, INPUT);
    pinMode(PIN_TIME, INPUT);
    pinMode(PIN_PRIORITY_MODE, INPUT);
    pinMode(PIN_CHANNEL_FILTER, INPUT);
    pinMode(PIN_TUNE, INPUT);
    pinMode(PIN_REC, INPUT);
}

bool Config::_read_source()
{
    int value = analogRead(PIN_SOURCE);
    byte source = SOURCE_DIRECT;
    if (value > 682)
    {
        source = SOURCE_ARPEGGIATOR;
    }
    else if (value > 341)
    {
        source = SOURCE_SEQUENCER;
    }
    bool changed = _active_source != source;
    _active_source = source;
}

bool Config::_read_polyphony_mode()
{
    int value = analogRead(PIN_POLYPHONY_MODE);
    int polyphony_mode = MODE_MONOPHONIC;
    if (value >= 768)
    {
        polyphony_mode = MODE_OCTOPHONIC;
    }
    else if (value >= 512)
    {
        polyphony_mode = MODE_QUADROPHONIC;
    }
    else if (value >= 256)
    {
        polyphony_mode = MODE_DUOPHONIC;
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

void Config::_read_arpeggiator_mode()
{
    int value = analogRead(PIN_ARPEGGIATOR_MODE);
    if (value < 341)
    {
        _arpeggiator_mode = ARPEGGIATOR_MODE_UP;
    }
    else if (value < 682)
    {
        _arpeggiator_mode = ARPEGGIATOR_MODE_DOWN;
    }
    else
    {
        _arpeggiator_mode = ARPEGGIATOR_MODE_UP_DOWN;
    }
}

void Config::_read_detune()
{
    _detune = floor((float)(analogRead(PIN_DETUNE) * (2 * DETUNE_RANGE + 1)) / 1024.0) - DETUNE_RANGE;
}

void Config::_read_time()
{
    int value = analogRead(PIN_TIME);
    _time_period = 60000 / (TIME_MIN_BPM + (float)(TIME_MAX_BPM - TIME_MIN_BPM) * (float)value / 1023.0);
}

bool Config::_read_priority_mode()
{
    _priority_mode = PRIORITY_REPLACE_OLDEST;
    return false;
}

bool Config::_read_channel_filter()
{
    _channel_filter = CHANNEL_FILTER_OFF;
    return false;
}

bool Config::_read_sequencer_record()
{
    int value = digitalRead(PIN_REC);
    bool changed = false;
    if (_button_sequencer_record == HIGH && value == LOW)
    {
        changed = true;
        _sequencer_record = !_sequencer_record;
    }
    _button_sequencer_record = value;
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

int Config::read()
{
    int changed = 0;
    if (_read_source())
    {
        changed = changed + CONFIG_CHANGE_SOURCE;
    }
    if (_read_polyphony_mode())
    {
        changed = changed + CONFIG_CHANGE_POLYPHONY_MODE;
    }
    // if (_read_priority_mode())
    // {
    //     changed = changed + CONFIG_CHANGE_PRIORITY_MODE;
    // }
    // if (_read_channel_filter())
    // {
    //     changed = changed + CONFIG_CHANGE_CHANNEL_FILTER;
    // }
    if (_read_sequencer_record())
    {
        changed = changed + CONFIG_CHANGE_SEQUENCER_RECORD;
    }
    if (_read_tuning())
    {
        changed = changed + CONFIG_CHANGE_TUNING;
    }
    _read_pitch_bend_range();
    _read_glide_intensity();
    _read_arpeggiator_mode();
    _read_detune();
    _read_time();
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
    // if (number == MIDI_CONTROL_GLIDE_INTENSITY)
    // {
    //     if (value < 64)
    //     {
    //         _glide_intensity = 1.0 - ((float)value / 63.0);
    //         _glide_proportional = false;
    //     }
    //     else
    //     {
    //         _glide_intensity = (float)(value - 64) / 63.0;
    //         _glide_proportional = true;
    //     }
    // }
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
    // else if (number == MIDI_CONTROL_PITCH_BEND_RANGE)
    // {
    //     _pitch_bend_range = round((float)value / 127.0 * 12.0 * 4.0) / 4.0;
    // }
    // else if (number == MIDI_CONTROL_SOURCE)
    // {
    //     byte old_source = _active_source;
    //     if (value < 42)
    //     {
    //         _active_source = SOURCE_DIRECT;
    //     }
    //     else if (value < 84)
    //     {
    //         _active_source = SOURCE_ARPEGGIATOR;
    //     }
    //     else
    //     {
    //         _active_source = SOURCE_SEQUENCER;
    //     }
    //     if (old_source != _active_source)
    //     {
    //         changed = changed + CONFIG_CHANGE_SOURCE;
    //     }
    // }
    // else if (number == MIDI_CONTROL_SEQUENCER_RECORD)
    // {
    //     bool old_sequencer_record = _sequencer_record;
    //     _sequencer_record = value >= 64;
    //     if (old_sequencer_record != _sequencer_record)
    //     {
    //         changed = changed + CONFIG_CHANGE_SEQUENCER_RECORD;
    //     }
    // }
    // else if (number == MIDI_CONTROL_TIME)
    // {
    //     _time_period = 60000 / (TIME_MIN_BPM + (float)(TIME_MAX_BPM - TIME_MIN_BPM) * (float)value / 127.0);
    // }
    // else if (number == MIDI_CONTROL_ARPEGGIATOR_MODE)
    // {
    //     if (value < 32)
    //     {
    //         _arpeggiator_mode = ARPEGGIATOR_MODE_UP;
    //     }
    //     else if (value < 64)
    //     {
    //         _arpeggiator_mode = ARPEGGIATOR_MODE_DOWN;
    //     }
    //     else if (value < 96)
    //     {
    //         _arpeggiator_mode = ARPEGGIATOR_MODE_UP_DOWN;
    //     }
    //     else
    //     {
    //         _arpeggiator_mode = ARPEGGIATOR_MODE_RANDOM;
    //     }
    // }
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

int Config::get_detune()
{
    return _detune;
}

bool Config::is_tuning()
{
    return _tuning;
}