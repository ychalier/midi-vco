#include "Arduino.h"
#include "../include/allocator.h"

Allocator::Allocator(Config *config, Router *router)
{
    _config = config;
    _router = router;
    for (int i = 0; i < POOL_COUNT; i++)
    {
        _pools[i] = new Pool(_router);
    }
}

void Allocator::setup()
{
    set_lane_masks();
}

void Allocator::reset()
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        _pools[i]->stop();
    }
}

void Allocator::reset_masked(byte mask)
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        if (check_mask(mask, i))
        {
            _pools[i]->stop();
        }
    }
}

void Allocator::set_lane_masks()
{
    reset();
    for (int i = 0; i < POOL_COUNT; i++)
    {
        _pools[i]->set_lane_mask(0b00000000);
    }
    switch (_config->get_polyphony_mode())
    {
    case MODE_MONOPHONIC:
        _pools[0]->set_lane_mask(0b11111111);
        break;
    case MODE_DUOPHONIC:
        _pools[0]->set_lane_mask(0b00001111);
        _pools[1]->set_lane_mask(0b11110000);
        break;
    case MODE_QUADROPHONIC:
        _pools[0]->set_lane_mask(0b00000011);
        _pools[1]->set_lane_mask(0b00001100);
        _pools[2]->set_lane_mask(0b00110000);
        _pools[3]->set_lane_mask(0b11000000);
        break;
    case MODE_OCTOPHONIC:
        _pools[0]->set_lane_mask(0b00000001);
        _pools[1]->set_lane_mask(0b00000010);
        _pools[2]->set_lane_mask(0b00000100);
        _pools[3]->set_lane_mask(0b00001000);
        _pools[4]->set_lane_mask(0b00010000);
        _pools[5]->set_lane_mask(0b00100000);
        _pools[6]->set_lane_mask(0b01000000);
        _pools[7]->set_lane_mask(0b10000000);
        break;
    }
}

void Allocator::note_on(byte pitch)
{
    note_on_masked(pitch, 0b11111111);
}

void Allocator::note_off(byte pitch)
{
    note_off_masked(pitch, 0b11111111);
}

void Allocator::note_on_masked(byte pitch, byte mask)
{
    bool accepted = false;
    for (int i = 0; i < POOL_COUNT; i++)
    {
        if (check_mask(mask, i) && _pools[i]->is_free() && _pools[i]->is_enabled())
        {
            _pools[i]->load(pitch);
            accepted = true;
            break;
        }
    }
    if (!accepted)
    {
        int optimal_index = -1;
        unsigned long optimal_era, era;
        switch (_config->get_priority_mode())
        {
        case PRIORITY_KEEP_FIRST:
            break;
        case PRIORITY_REPLACE_OLDEST:
            optimal_era = 0;
            for (int i = 0; i < POOL_COUNT; i++)
            {
                era = _pools[i]->get_era();
                if (check_mask(mask, i) &&
                    _pools[i]->is_enabled() &&
                    (optimal_index == -1 || optimal_era > era))
                {
                    optimal_index = i;
                    optimal_era = era;
                }
            }
            break;
        case PRIORITY_REPLACE_NEWEST:
            optimal_era = 0;
            for (int i = 0; i < POOL_COUNT; i++)
            {
                era = _pools[i]->get_era();
                if (check_mask(mask, i) &&
                    _pools[i]->is_enabled() &&
                    (optimal_index == -1 || optimal_era < era))
                {
                    optimal_index = i;
                    optimal_era = era;
                }
            }
            break;
        }
        if (optimal_index >= 0)
        {
            _pools[optimal_index]->load(pitch);
        }
    }
}

void Allocator::note_off_masked(byte pitch, byte mask)
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        if (check_mask(mask, i) && _pools[i]->unload(pitch))
        {
            break;
        }
    }
}

void Allocator::pitch_bend(int bend_value)
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        _pools[i]->bend(bend_value);
    }
}

void Allocator::hold_on()
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        if (_pools[i]->is_active())
        {
            _pools[i]->lock();
        }
    }
}

void Allocator::hold_off()
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        _pools[i]->unlock();
    }
}

void Allocator::after_touch_poly(byte pitch, int bend)
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        if (_pools[i]->buffer_contains(pitch))
        {
            _pools[i]->bend(bend);
            break;
        }
    }
}

void Allocator::after_touch_channel(int bend)
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        _pools[i]->bend(bend);
    }
}

void Allocator::broadcast(byte pitch, int gate)
{
    _router->broadcast(pitch, gate);
}

bool Allocator::check_mask(byte mask, int value)
{
    return (mask >> value) & 1;
}

bool Allocator::is_active()
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        if (_pools[i]->is_active())
        {
            return true;
        }
    }
    return false;
}