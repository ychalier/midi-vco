#ifndef arpeggiator_h
#define arpeggiator_h

#include "Arduino.h"
#include "config.h"
#include "structs.h"
#include "allocator.h"
#include "buffer.h"

class Arpeggiator
{
public:
    Arpeggiator(Config *config, Allocator *allocator);
    void note_on(Note note);
    void note_off(Note note);
    void update();
    void reset();

private:
    Config *_config;
    Allocator *_allocator;
    Buffer *_buffer;
    bool _direction;
    Note _current;
    unsigned long _timestamp;
    bool Arpeggiator::find_next_note(Note &note_next, Note &note_min, Note &note_max);
    void play(Note note);
};

#endif