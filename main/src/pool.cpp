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
    _buffer = new Buffer();
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

void Pool::load_buffer(int bend, bool set_only)
{
    Note note = _buffer->get();
    for (byte lane_id = 0; lane_id < 8; lane_id++)
    {
        if ((_lane_mask & (1 << lane_id)) > 0)
        {
            if (set_only)
            {
                _router->select(lane_id)->set(Lane::pitch_to_voltage(note.pitch, bend));
            }
            else
            {
                _router->select(lane_id)->start(Lane::pitch_to_voltage(note.pitch, 0), GLIDE_DEFAULT_DURATION);
            }
        }
    }
}

void Pool::load(Note note)
{
    _active = true;
    _era = millis();
    _buffer->push(note);
    load_buffer(0, false);
}

void Pool::stop()
{
    _active = false;
    _buffer->reset();
    for (byte lane_id = 0; lane_id < 8; lane_id++)
    {
        if ((_lane_mask & (1 << lane_id)) > 0)
        {
            _router->select(lane_id)->stop();
        }
    }
}

bool Pool::unload(Note note)
{
    if (_active && _buffer->pop(note))
    {
        if (_buffer->empty())
        {
            stop();
        }
        else
        {
            load_buffer(0, true);
        }
        return true;
    }
    else
    {
        return false;
    }
}

void Pool::bend(int bend_value)
{
    load_buffer(bend_value, true);
}

unsigned long Pool::get_era()
{
    return _era;
}