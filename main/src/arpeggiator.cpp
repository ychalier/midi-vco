#include "Arduino.h"
#include "../include/arpeggiator.h"

Arpeggiator::Arpeggiator(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
}

void Arpeggiator::note_on(Note note)
{
}

void Arpeggiator::note_off(Note note)
{
}

void Arpeggiator::update()
{
}