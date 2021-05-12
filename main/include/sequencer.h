#ifndef sequencer_h
#define sequencer_h

#include "Arduino.h"
#include "constants.h"
#include "structs.h"
#include "allocator.h"
#include "config.h"

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

    /**
     * Switch between playing state and recording state. Switching to record
     * mode clears the buffer.
     */
    void update_state(bool recording);

    /**
     * Update the sequencer playback.
     */
    void update();

private:
    Config *_config;
    Allocator *_allocator;

    /// Whether the sequencer is currently recording events.
    bool _recording;

    /// Current number of events recorded.
    int _size;

    /// Events buffers.
    MidiEvent _memory[SEQUENCER_MEMORY_SIZE];

    /// Timestamp of when the record started.
    unsigned long _record_timestamp;

    /// Timestamp of when the playback started.
    unsigned long _playback_timestamp;

    /// Current playback buffer index. Designates the upcoming event.
    int _playback_index;

    /**
     * Add an event to the buffer. If the buffer is full, it is ignored.
     * 
     * @param event Event to append to the buffer.
     */
    void record_event(MidiEvent event);
};

#endif