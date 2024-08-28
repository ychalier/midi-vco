#include "Arduino.h"
#include "../include/pool.h"

Pool::Pool(Router *router)
{
    _router = router;
    _lane_mask = 0;
    _buffer = new Buffer();
    _era = 0;
    _active = false;
    _locked = false;
}

void Pool::set_lane_mask(uint16_t lane_mask)
{
    _lane_mask = lane_mask;
}

bool Pool::is_enabled()
{
    return _lane_mask > 0 && !_locked;
}

bool Pool::is_free()
{
    return !_active;
}

bool Pool::is_active()
{
    return _active;
}

void Pool::load_buffer(int bend, bool set_only)
{
    bool found = false;
    byte pitch = _buffer->get(found);
    if (found)
    {
        for (byte lane_id = 0; lane_id < LANE_COUNT; lane_id++)
        {
            if ((_lane_mask & (1 << lane_id)) > 0)
            {
                if (set_only)
                {
                    _router->select(lane_id)->set_pitch(pitch, bend, false);
                }
                else
                {
                    _router->select(lane_id)->start_pitch(pitch, 0, false);
                }
            }
        }
    }
}

void Pool::load(byte pitch)
{
    if (!_locked)
    {
        _active = true;
        _era = millis();
        _buffer->push(pitch);
        load_buffer(0, false);
    }
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

bool Pool::unload(byte pitch)
{
    if (_active && _buffer->pop(pitch))
    {
        if (!_locked)
        {
            if (_buffer->empty())
            {
                stop();
            }
            else
            {
                load_buffer(0, false);
            }
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

void Pool::lock()
{
    _locked = true;
}

void Pool::unlock()
{
    if (_locked)
    {
        _locked = false;
        if (_buffer->empty())
        {
            stop();
        }
        else
        {
            load_buffer(0, false);
        }
    }
}

bool Pool::buffer_contains(byte pitch)
{
    return _active && _buffer->contains(pitch);
}