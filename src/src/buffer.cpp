#include "Arduino.h"
#include "../include/buffer.h"

Buffer::Buffer()
{
    _cursor = -1;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        _notes[i] = {0, 0};
    }
}

bool Buffer::empty()
{
    return _cursor == -1;
}

Note Buffer::get(bool &found)
{
    if (_cursor >= 0)
    {
        found = true;
        return _notes[_cursor];
    }
    else
    {
        found = false;
        return _notes[0];
    }
}

void Buffer::push(Note note)
{
    _cursor++;
    if (_cursor >= BUFFER_SIZE)
    {
        shift(0);
        _cursor = BUFFER_SIZE - 1;
    }
    _notes[_cursor] = note;
}

bool Buffer::pop(Note note)
{
    bool found = false;
    for (int i = 0; i <= _cursor; i++)
    {
        if (_notes[i].pitch == note.pitch)
        {
            shift(i);
            _cursor--;
            found = true;
            break;
        }
    }
    return found;
}

void Buffer::shift(int start)
{
    for (int i = start; i < BUFFER_SIZE - 1; i++)
    {
        _notes[i] = _notes[i + 1];
    }
}

void Buffer::reset()
{
    _cursor = -1;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        _notes[i] = {0, 0};
    }
}

int Buffer::get_size()
{
    return _cursor + 1;
}

Note Buffer::get_at_index(int index)
{
    if (index >= 0 && index <= _cursor)
    {
        return _notes[index];
    }
    return {0, 0};
}

bool Buffer::contains(Note note)
{
    bool found = false;
    for (int i = 0; i <= _cursor; i++)
    {
        if (_notes[i].pitch == note.pitch)
        {
            found = true;
            break;
        }
    }
    return found;
}