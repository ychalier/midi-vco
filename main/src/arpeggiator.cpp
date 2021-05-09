#include "Arduino.h"
#include "../include/arpeggiator.h"

Arpeggiator::Arpeggiator(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    _buffer = new Buffer();
    _direction = true;
    _current = {0, 0};
    _timestamp = 0;
}

void Arpeggiator::note_on(Note note)
{
    _buffer->push(note);
}

void Arpeggiator::note_off(Note note)
{
    _buffer->pop(note);
}

void Arpeggiator::reset()
{
    _buffer->reset();
    _direction = true;
    _current = {0, 0};
    _timestamp = 0;
}

void Arpeggiator::play(Note note)
{
    _allocator->note_off(_current);
    _current = note;
    _allocator->note_on(_current);
}

bool Arpeggiator::find_next_note(Note &note_next, Note &note_min, Note &note_max)
{
    bool found = false;
    if (_direction)
    {
        note_next = {_current.channel, 128};
    }
    else
    {
        note_next = {_current.channel, 0};
    }
    note_min = {_current.channel, _current.pitch};
    note_max = {_current.channel, _current.pitch};
    for (int i = 0; i < _buffer->get_size(); i++)
    {
        Note buffer_note = _buffer->get_at_index(i);
        if (buffer_note.pitch < note_min.pitch)
        {
            note_min = buffer_note;
        }
        if (buffer_note.pitch > note_max.pitch)
        {
            note_max = buffer_note;
        }
        if ((_direction && buffer_note.pitch > _current.pitch && buffer_note.pitch < note_next.pitch) ||
            (!_direction && buffer_note.pitch < _current.pitch && buffer_note.pitch > note_next.pitch))
        {
            found = true;
            note_next = buffer_note;
        }
    }
    return found;
}

void Arpeggiator::update()
{
    Note note_next, note_min, note_max;
    if (_buffer->get_size() > 0)
    {
        unsigned long now = millis();
        if (now - _timestamp >= _config->get_arpeggiator_period())
        {
            _timestamp = now;
            switch (_config->get_arpeggiator_mode())
            {
            case ARPEGGIATOR_MODE_UP:
                if (find_next_note(note_next, note_min, note_max))
                {
                    play(note_next);
                }
                else
                {
                    play(note_min);
                }
                break;
            case ARPEGGIATOR_MODE_DOWN:
                if (find_next_note(note_next, note_min, note_max))
                {
                    play(note_next);
                }
                else
                {
                    play(note_max);
                }
                break;
            case ARPEGGIATOR_MODE_UP_DOWN:
                if (find_next_note(note_next, note_min, note_max))
                {
                    play(note_next);
                }
                else
                {
                    _direction = !_direction;
                    if (_direction)
                    {
                        play(note_min);
                    }
                    else
                    {
                        play(note_max);
                    }
                }
                break;
            case ARPEGGIATOR_MODE_RANDOM:
                play(_buffer->get_at_index(random(0, _buffer->get_size())));
                break;
            }
        }
    }
}