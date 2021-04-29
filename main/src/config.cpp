#include "Arduino.h"
#include "../include/config.h"

Config::Config()
{
    _polyphony_mode = MODE_MONOPHONIC;
    _priority_mode = PRIORITY_REPLACE_FIRST;
    _channel_filter = CHANNEL_FILTER_OFF;
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
        return PRIORITY_REPLACE_LAST;
    }
    else
    {
        return PRIORITY_REPLACE_FIRST;
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
