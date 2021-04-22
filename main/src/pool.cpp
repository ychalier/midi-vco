#include "Arduino.h"
#include "../include/pool.h"

unsigned int channel_to_mask(byte channel)
{
    if (channel == MIDI_CHANNEL_OMNI)
    {
        return 0xFFFF;
    }
    else if (channel >= 1 && channel <= 16)
    {
        return 1 << (channel - 1);
    }
    else
    {
        return 0;
    }
}

Pool::Pool(Router *router)
{
    _router = router;
    _lane_mask = 0;
    _channel_mask = 0;
    _current_note = {1, 0};
    _era = 0;
    _active = false;
}

void Pool::set_masks(byte lane_mask, unsigned int channel_mask)
{
    _lane_mask = lane_mask;
    _channel_mask = channel_mask;
}

bool Pool::is_enabled()
{
    return _lane_mask > 0;
}

bool Pool::is_free()
{
    return !_active;
}

bool Pool::accepts_channel(byte channel)
{
    int mask = 0;
    if (channel == MIDI_CHANNEL_OMNI)
    {
        mask = 0xFFFF;
    }
    else if (channel >= 1 && channel <= 16)
    {
        mask = 1 << (channel - 1);
    }
    return (_channel_mask & mask) > 0;
}

bool Pool::accepts_note(Note note)
{
    return is_enabled() && accepts_channel(note.channel);
}

void Pool::load(Note note)
{
    _active = true;
    _era = millis();
    _current_note = note;
    for (byte lane_id = 0; lane_id < 8; lane_id++)
    {
        if ((_lane_mask & (1 << lane_id)) > 0)
        {
            _router->select(lane_id)->start(Lane::pitch_to_voltage(note.pitch, 0));
        }
    }
}

void Pool::unload()
{
    _active = false;
    for (byte lane_id = 0; lane_id < 8; lane_id++)
    {
        if ((_lane_mask & (1 << lane_id)) > 0)
        {
            _router->select(lane_id)->stop();
        }
    }
}

void Pool::bend(int bend_value)
{
    for (byte lane_id = 0; lane_id < 8; lane_id++)
    {
        if ((_lane_mask & (1 << lane_id)) > 0)
        {
            _router->select(lane_id)->set(Lane::pitch_to_voltage(_current_note.pitch, bend_value));
        }
    }
}

unsigned long Pool::get_era()
{
    return _era;
}

Note Pool::get_current_note()
{
    return _current_note;
}
