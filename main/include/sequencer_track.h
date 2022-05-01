#ifndef sequencer_channel_h
#define sequencer_channel_h

#include "Arduino.h"
#include "constants.h"
#include "structs.h"
#include "config.h"
#include "allocator.h"

/**
 * Implements the features of a sequencer channel.
 */
class SequencerTrack
{
public:
    /**
     * Constructor.
     *
     * @param config A pointer to the global config.
     * @param allocator A pointer to the global allocator.
     */
    SequencerTrack(Config *config, Allocator *allocator);

    void record(MidiEvent event);

    void play(int division);

    void reset();

private:
    Config *_config;
    Allocator *_allocator;
    MidiEvent _memory[SEQUENCER_MEMORY_SIZE];
    int _memory_index[SEQUENCER_DIVISIONS_PER_LOOP];

    void execute_event(int event_index);
};

#endif