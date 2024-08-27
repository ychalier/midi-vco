#include "Arduino.h"
#include "../include/buffer.h"

Buffer::Buffer()
{
    _cursor = -1;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        _notes[i] = 0;
    }
}

bool Buffer::empty()
{
    return _cursor == -1;
}

byte Buffer::get(bool &found)
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

void Buffer::push(byte pitch)
{
    _cursor++;
    if (_cursor >= BUFFER_SIZE)
    {
        shift(0);
        _cursor = BUFFER_SIZE - 1;
    }
    _notes[_cursor] = pitch;
}

bool Buffer::pop(byte pitch)
{
    bool found = false;
    for (int i = 0; i <= _cursor; i++)
    {
        if (_notes[i] == pitch)
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
        _notes[i] = 0;
    }
}

int Buffer::get_size()
{
    return _cursor + 1;
}

byte Buffer::get_at_index(int index)
{
    if (index >= 0 && index <= _cursor)
    {
        return _notes[index];
    }
    return 0;
}

bool Buffer::contains(byte pitch)
{
    bool found = false;
    for (int i = 0; i <= _cursor; i++)
    {
        if (_notes[i] == pitch)
        {
            found = true;
            break;
        }
    }
    return found;
}