#ifndef sequencer_h
#define sequencer_h

#include "Arduino.h"
#include "constants.h"
#include "structs.h"
#include "allocator.h"
#include "config.h"
#include "sequencer_channel.h"

/**
 * Implements the features of a sequencer.
 */
class Sequencer
{
public:
    /**
     * Constructor.
     * 
     * @param config A pointer to the global config.
     * @param allocator A pointer to the global allocator.
     */
    Sequencer(Config *config, Allocator *allocator);

    /**
     * Callback for the *note-on* MIDI message. Stores the event in a buffer.
     * If the buffer is full, the event is ignored.
     * 
     * @param note The note that has been played.
     */
    void note_on(Note note);

    /**
     * Callback for the *note-off* MIDI message. Stores the event in a buffer.
     * If the buffer is full, the event is ignored.
     * 
     * @param note The note that has been released.
     */
    void note_off(Note note);

    void update_record_state(bool recording);

    void update_source_activation(bool activated);

    /**
     * Update the sequencer playback.
     */
    void update();

private:
    Config *_config;
    Allocator *_allocator;

    SequencerChannel *_channels[SEQUENCER_CHANNEL_COUNT];

    /// Whether the sequencer is currently recording events.
    bool _recording;

    unsigned long _loopback_timestamp;

    bool update_loopback(unsigned long now);

};

#endif