#ifndef arpeggiator_h
#define arpeggiator_h

#include "Arduino.h"
#include "config.h"
#include "structs.h"
#include "allocator.h"
#include "buffer.h"

/**
 * Implements the features of an arpeggiator.
 */
class Arpeggiator
{
public:
    /**
     * Constructor
     * 
     * @param config A pointer to the global config.
     * @param allocator A pointer to the global allocator.
     */
    Arpeggiator(Config *config, Allocator *allocator);

    /**
     * Callback for the *note-on* MIDI message. Stores the note in a buffer.
     * 
     * @param note The note that has been played.
     */
    void note_on(byte pitch);

    /**
     * Callback for the *note-off* MIDI message. Remove the note from the
     * buffer.
     * 
     * @param note The note that has been released.
     */
    void note_off(byte pitch);

    /**
     * Where the note playing happens. Must be called in the main program loop.
     */
    void update();

    /**
     * Reset of the internal buffer.
     */
    void reset();

private:
    Config *_config;
    Allocator *_allocator;

    /// Note buffer
    Buffer *_buffer;

    /// If true, notes are currently playing from the lowest to the highest
    /// pitch; else, it is the other way around.
    bool _direction;

    /// The last note that have been played. Used to dermine which note to play
    /// next according to the direction.
    byte _current;

    /// When was the last note played
    unsigned long _timestamp;

    /**
     * Determine which note to play next, in the buffer.
     * 
     * @param note_next A reference for storing the next note that should be
     *   played, according to the current direction and the last played note.
     *   If none is found (e.g. if the highest note was already playing and
     *   we were looking for an even higher note), then values set are wrong.
     *   The returned value indicates whether this value should be considered
     *   or not.
     * @param note_min A reference for storing the note in the buffer with the
     *   highest pitch.
     * @param note_max A reference for storing the note in the buffer with the
     *   lowest pitch. 
     * @return `true` if a valid next note was found. If not, one of the
     *   extrema should be played instead.
     */
    bool find_next_note(byte &pitch_next, byte &pitch_min, byte &pitch_max);

    /**
     * Make the arpeggiator play a note. Store it as the current note, and
     * update the timestamp.
     * 
     */
    void play(byte pitch);
};

#endif