#ifndef arpeggiator_h
#define arpeggiator_h

#include "Arduino.h"
#include "config.h"
#include "structs.h"
#include "allocator.h"

class Arpeggiator
{
public:
    Arpeggiator(Config *config, Allocator *allocator);
    void note_on(Note note);
    void note_off(Note note);
    void update();

private:
    Config *_config;
    Allocator *_allocator;
};

#endif