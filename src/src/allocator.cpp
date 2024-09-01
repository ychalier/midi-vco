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

void Allocator::reset_masked(uint16_t mask)
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
        _pools[i]->set_lane_mask(0b0000000000000000);
    }
    switch (_config->get_polyphony_mode())
    {
    case MODE_MONOPHONIC:
        _pools[0]->set_lane_mask(0b0000001111111111);
        break;
    case MODE_POLY5:
        _pools[0]->set_lane_mask(0b0000000000000011);
        _pools[1]->set_lane_mask(0b0000000000001100);
        _pools[2]->set_lane_mask(0b0000000000110000);
        _pools[3]->set_lane_mask(0b0000000011000000);
        _pools[4]->set_lane_mask(0b0000001100000000);
        break;
    case MODE_POLY10:
        _pools[0]->set_lane_mask(0b0000000000000001);
        _pools[1]->set_lane_mask(0b0000000000000010);
        _pools[2]->set_lane_mask(0b0000000000000100);
        _pools[3]->set_lane_mask(0b0000000000001000);
        _pools[4]->set_lane_mask(0b0000000000010000);
        _pools[5]->set_lane_mask(0b0000000000100000);
        _pools[6]->set_lane_mask(0b0000000001000000);
        _pools[7]->set_lane_mask(0b0000000010000000);
        _pools[8]->set_lane_mask(0b0000000100000000);
        _pools[9]->set_lane_mask(0b0000001000000000);
        break;
    }
}

void Allocator::note_on(Note note)
{
    note_on_masked(note, 0b1111111111111111);
}

void Allocator::note_off(Note note)
{
    note_off_masked(note, 0b1111111111111111);
}

void Allocator::note_on_masked(Note note, uint16_t mask)
{
    bool accepted = false;
    for (int i = 0; i < POOL_COUNT; i++)
    {
        if (check_mask(mask, i) && _pools[i]->is_free() && _pools[i]->is_enabled())
        {
            _pools[i]->load(note);
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
            _pools[optimal_index]->load(note);
        }
    }
}

void Allocator::note_off_masked(Note note, uint16_t mask)
{
    for (int i = 0; i < POOL_COUNT; i++)
    {
        if (check_mask(mask, i) && _pools[i]->unload(note))
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

void Allocator::broadcast(int setpoint, int gate)
{
    _router->broadcast(setpoint, gate);
}

bool Allocator::check_mask(uint16_t mask, int value)
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