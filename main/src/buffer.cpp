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

byte Buffer::get()
{
    if (_cursor >= 0)
    {
        return _notes[_cursor];
    }
    else
    {
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
        // In a perfect world, this condition would not be needed. But we live
        // in a society where being nice and honest can get you in trouble.
        // Hence, one may have to sometimes step away from the path of harmony
        // to get things done.
        // The actual VCO may maintain the note for longer than this logical
        // system thinks. This mean that even if the GATE signal is off, the
        // user might hear a sound. So we have to make sure the correct note is
        // played, even after being released. This behavior introduces a bug
        // when the user triggers an effect such as pitch-bend, aftertouch or
        // glide. The effect affects the last note in memory, but if it was
        // erased, this note is set to pitch 0, which causes weird sounds to
        // occur. This conditions ensures that notes are not erased from
        // memory until they are replaced by another real one.
        if (_notes[i + 1] > 0)
        {
            _notes[i] = _notes[i + 1];
        }
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