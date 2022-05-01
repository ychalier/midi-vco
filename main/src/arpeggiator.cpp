#include "Arduino.h"
#include "../include/arpeggiator.h"

Arpeggiator::Arpeggiator(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    _buffer = new Buffer();
    _direction = true;
    _current = 0;
    _timestamp = 0;
}

void Arpeggiator::note_on(byte pitch)
{
    if (_config->is_recording())
    {
        _buffer->push(pitch);
    }
    else
    {
        _allocator->note_on(pitch);
    }
}

void Arpeggiator::note_off(byte pitch)
{
    if (!_buffer->pop(pitch))
    {
        _allocator->note_off(pitch);
    }
    else if (_buffer->empty())
    {
        _allocator->reset();
    }
}

void Arpeggiator::reset()
{
    _buffer->reset();
    _direction = true;
    _current = 0;
    _timestamp = 0;
}

void Arpeggiator::play(byte pitch)
{
    _allocator->note_off(_current);
    _current = pitch;
    _allocator->note_on(_current);
}

bool Arpeggiator::find_next_note(byte &pitch_next, byte &pitch_min, byte &pitch_max)
{
    bool found = false;
    if (_direction)
    {
        pitch_next = 128;
    }
    else
    {
        pitch_next = 0;
    }
    pitch_min = _current;
    pitch_max = _current;
    for (int i = 0; i < _buffer->get_size(); i++)
    {
        byte buffer_pitch = _buffer->get_at_index(i);
        if (buffer_pitch < pitch_min)
        {
            pitch_min = buffer_pitch;
        }
        if (buffer_pitch > pitch_max)
        {
            pitch_max = buffer_pitch;
        }
        if ((_direction && buffer_pitch > _current && buffer_pitch < pitch_next) ||
            (!_direction && buffer_pitch < _current && buffer_pitch > pitch_next))
        {
            found = true;
            pitch_next = buffer_pitch;
        }
    }
    return found;
}

void Arpeggiator::update()
{
    byte pitch_next, pitch_min, pitch_max;
    if (_buffer->get_size() > 0)
    {
        unsigned long now = millis();
        if ((now - _timestamp) >= _config->get_time_period())
        {
            _timestamp = now;
            switch (_config->get_arpeggiator_mode())
            {
            case ARPEGGIATOR_MODE_UP:
                _direction = true;
                if (find_next_note(pitch_next, pitch_min, pitch_max))
                {
                    play(pitch_next);
                }
                else
                {
                    play(pitch_min);
                }
                break;
            case ARPEGGIATOR_MODE_DOWN:
                _direction = false;
                if (find_next_note(pitch_next, pitch_min, pitch_max))
                {
                    play(pitch_next);
                }
                else
                {
                    play(pitch_max);
                }
                break;
            case ARPEGGIATOR_MODE_UP_DOWN:
                if (find_next_note(pitch_next, pitch_min, pitch_max))
                {
                    play(pitch_next);
                    if (pitch_next == pitch_min)
                    {
                        _direction = true;
                    }
                    else if (pitch_next == pitch_max)
                    {
                        _direction = false;
                    }
                }
                else
                {
                    _direction = !_direction;
                    if (_direction)
                    {
                        play(pitch_min);
                    }
                    else
                    {
                        play(pitch_max);
                    }
                }
                break;
            case ARPEGGIATOR_MODE_RANDOM:
                play(_buffer->get_at_index(random(0, _buffer->get_size())));
                break;
            }
        }
        else if ((now - _timestamp) > _config->get_arpeggiator_sustain() * _config->get_time_period())
        {
            _allocator->note_off(_current);
        }
    }
}