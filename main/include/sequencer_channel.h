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
class SequencerChannel
{
public:
    /**
     * Constructor.
     * 
     * @param config A pointer to the global config.
     * @param allocator A pointer to the global allocator.
     */
    SequencerChannel(Config *config, Allocator *allocator);

    /**
     * Add a event to the memory. If the memory is full, the event is ignored.
     * 
     * @return True if the event was added to the memory. False if the memory
     *   was full and the event was discarded.
     */
    bool record(bool type, Note note, byte mask, unsigned long loopback_timestamp);

    /**
     * Play next events in memory. Start at `_playback_index`, and increment
     * while events are close in time.
     */
    void play(int play_until_div, bool loopback);

    /**
     * Empty the memory and set the playback index to 0.
     */
    void reset_memory();

    /**
     * Set the playback index to 0.
     */
    void reset_playback();

private:
    Config *_config;
    Allocator *_allocator;

    /// Current number of events recorded.
    int _memory_index;

    /// Events buffers.
    MidiEvent _memory[SEQUENCER_MEMORY_SIZE];

    /// Current playback buffer index. Designates the upcoming event.
    int _playback_index;

    int _first_note_index;

    unsigned long _record_timestamp;

    void execute_event();

    bool update_loopback(unsigned long now);

};

#endif