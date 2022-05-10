#include "Arduino.h"
#include "../include/sequencer_track.h"

SequencerTrack::SequencerTrack(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    _memory_index = 0;
}

void SequencerTrack::record(MidiEvent event)
{
    if (_memory_index < SEQUENCER_MEMORY_SIZE)
    {
        _memory[_memory_index] = event;
        _memory_index++;
    }
}

void SequencerTrack::execute_event(int event_index)
{
    if (event_index < SEQUENCER_MEMORY_SIZE)
    {
        if (_memory[event_index].type)
        {
            _allocator->note_on_masked(_memory[event_index].pitch, _memory[event_index].mask);
        }
        else
        {
            _allocator->note_off_masked(_memory[event_index].pitch, _memory[event_index].mask);
        }
    }
}

void SequencerTrack::play(int division)
{
    for (int event_index = 0; event_index < _memory_index; event_index++)
    {
        if (_memory[_memory_index].division == division)
        {
            execute_event(event_index);
        }
    }
}

void SequencerTrack::reset()
{
    _memory_index = 0;
}
