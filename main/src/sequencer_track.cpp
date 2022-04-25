#include "Arduino.h"
#include "../include/sequencer_track.h"

SequencerTrack::SequencerTrack(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    reset();
}

void SequencerTrack::record(MidiEvent event)
{
    if (_memory_index[event.division] < SEQUENCER_MAX_EVENTS_PER_DIVISION)
    {
        int event_index = event.division * SEQUENCER_MAX_EVENTS_PER_DIVISION + _memory_index[event.division];
        _memory[event_index] = event;
        _memory_index[event.division]++;
    }
}

void SequencerTrack::execute_event(int event_index)
{
    if (event_index < SEQUENCER_MEMORY_SIZE)
    {
        if (_memory[event_index].type)
        {
            _allocator->note_on_masked(_memory[event_index].note, _memory[event_index].mask);
        }
        else
        {
            _allocator->note_off_masked(_memory[event_index].note, _memory[event_index].mask);
        }
    }
}

void SequencerTrack::play(int division)
{
    for (int offset = 0; offset < _memory_index[division]; offset++)
    {
        int event_index = division * SEQUENCER_MAX_EVENTS_PER_DIVISION + offset;
        execute_event(event_index);
    }
}

void SequencerTrack::reset()
{
    for (int division = 0; division < SEQUENCER_DIVISIONS_PER_LOOP; division++)
    {
        _memory_index[division] = 0;
    }
}
