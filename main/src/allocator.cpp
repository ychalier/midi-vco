#include "Arduino.h"
#include "../include/allocator.h"

Allocator::Allocator(Config *config, Router *router)
{
    _config = config;
    _router = router;
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _pools[i] = new Pool(_router);
    }
}

void Allocator::setup()
{
    set_masks();
}

void Allocator::reset()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _pools[i]->stop();
    }
}

void Allocator::set_masks()
{
    reset();
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _pools[i]->set_masks(0, 0);
    }
    switch (_config->get_channel_filter())
    {
    case CHANNEL_FILTER_OFF:
        switch (_config->get_polyphony_mode())
        {
        case MODE_MONOPHONIC:
            _pools[0]->set_masks(0b11111111, 0xFFFF);
            break;
        case MODE_DUOPHONIC:
            _pools[0]->set_masks(0b00001111, 0xFFFF);
            _pools[1]->set_masks(0b11110000, 0xFFFF);
            break;
        case MODE_QUADROPHONIC:
            _pools[0]->set_masks(0b00000011, 0xFFFF);
            _pools[1]->set_masks(0b00001100, 0xFFFF);
            _pools[2]->set_masks(0b00110000, 0xFFFF);
            _pools[3]->set_masks(0b11000000, 0xFFFF);
            break;
        case MODE_OCTOPHONIC:
            _pools[0]->set_masks(0b00000001, 0xFFFF);
            _pools[1]->set_masks(0b00000010, 0xFFFF);
            _pools[2]->set_masks(0b00000100, 0xFFFF);
            _pools[3]->set_masks(0b00001000, 0xFFFF);
            _pools[4]->set_masks(0b00010000, 0xFFFF);
            _pools[5]->set_masks(0b00100000, 0xFFFF);
            _pools[6]->set_masks(0b01000000, 0xFFFF);
            _pools[7]->set_masks(0b10000000, 0xFFFF);
            break;
        }
        break;
    case CHANNEL_FILTER_ON:
        switch (_config->get_polyphony_mode())
        {
        case MODE_MONOPHONIC:
            _pools[0]->set_masks(0b00000011, 0b0001);
            _pools[1]->set_masks(0b00001100, 0b0010);
            _pools[2]->set_masks(0b00110000, 0b0100);
            _pools[3]->set_masks(0b11000000, 0b1000);
            break;
        case MODE_DUOPHONIC:
            _pools[0]->set_masks(0b00001111, 0b0001);
            _pools[1]->set_masks(0b11110000, 0b0010);
            break;
        case MODE_QUADROPHONIC:
            _pools[0]->set_masks(0b00000011, 0b0001);
            _pools[1]->set_masks(0b00001100, 0b0001);
            _pools[2]->set_masks(0b00110000, 0b0010);
            _pools[3]->set_masks(0b11000000, 0b0010);
            break;
        case MODE_OCTOPHONIC:
            _pools[0]->set_masks(0b00000001, 0b0001);
            _pools[1]->set_masks(0b00000010, 0b0001);
            _pools[2]->set_masks(0b00000100, 0b0010);
            _pools[3]->set_masks(0b00001000, 0b0010);
            _pools[4]->set_masks(0b00010000, 0b0100);
            _pools[5]->set_masks(0b00100000, 0b0100);
            _pools[6]->set_masks(0b01000000, 0b1000);
            _pools[7]->set_masks(0b10000000, 0b1000);
            break;
        }
        break;
    }
}

void Allocator::note_on(Note note)
{
    note_on_masked(note, 0b11111111);
}

void Allocator::note_off(Note note)
{
    note_off_masked(note, 0b11111111);
}

void Allocator::note_on_masked(Note note, byte mask)
{
    bool accepted = false;
    for (int i = 0; i < LANE_COUNT; i++)
    {
        if (check_mask(mask, i) && _pools[i]->is_free() && _pools[i]->accepts_note(note))
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
            for (int i = 0; i < LANE_COUNT; i++)
            {
                era = _pools[i]->get_era();
                if (check_mask(mask, i) &&
                    _pools[i]->accepts_note(note) &&
                    (optimal_index == -1 || optimal_era > era))
                {
                    optimal_index = i;
                    optimal_era = era;
                }
            }
            break;
        case PRIORITY_REPLACE_NEWEST:
            optimal_era = 0;
            for (int i = 0; i < LANE_COUNT; i++)
            {
                era = _pools[i]->get_era();
                if (check_mask(mask, i) &&
                    _pools[i]->accepts_note(note) &&
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

void Allocator::note_off_masked(Note note, byte mask)
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        if (check_mask(mask, i) && _pools[i]->unload(note))
        {
            break;
        }
    }
}

void Allocator::pitch_bend(byte channel, int bend_value)
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        if (_pools[i]->accepts_channel(channel))
        {
            _pools[i]->bend(bend_value);
        }
    }
}

void Allocator::hold_on()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        if (_pools[i]->is_active())
        {
            _pools[i]->lock();
        }
    }
}

void Allocator::hold_off()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _pools[i]->unlock();
    }
}

void Allocator::after_touch_poly(Note note, int bend)
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        if (_pools[i]->buffer_contains(note))
        {
            _pools[i]->bend(bend);
            break;
        }
    }
}

void Allocator::after_touch_channel(byte channel, int bend)
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        if (_pools[i]->accepts_channel(channel))
        {
            _pools[i]->bend(bend);
        }
    }
}

void Allocator::broadcast_pitch(byte pitch)
{
    Note note = {0, pitch};
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _pools[i]->unlock();
        _pools[i]->load(note);
    }
}

static bool Allocator::check_mask(byte mask, int value)
{
    return (mask >> value) & 1;
}