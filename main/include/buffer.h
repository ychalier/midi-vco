#ifndef buffer_h
#define buffer_h

#include "Arduino.h"
#include "constants.h"
#include "structs.h"

/**
 * Note buffer. Allows for remembering previously triggered notes that were
 * replaced in the allocation process but that are still active (no note-off)
 * message received about them. If the latest note ends before the replaced
 * one, the replaced one will be played again.
 */
class Buffer
{
public:
    Buffer();

    /**
     * Check if the buffer is empty.
     * 
     * @return True if the buffer is empty.
     */
    bool empty();

    /**
     * Push a note to the buffer. If the buffer is full, its contents is
     * shifted, discarding the oldest note in it. The note to push is then
     * added on top of the stack.
     * 
     * @param note The note to insert in the buffer.
     */
    void push(Note note);

    /**
     * Get the currently active note (i.e. the last note inserted in the
     * buffer that has not been released yet). If the buffer is empty, returns
     * a dummy note with channel 0 and pitch 0.
     * 
     * @return The currently active note.
     */
    Note get();

    /**
     * Attempt to remove a note from the buffer. If it succeeds, the cursor is
     * decremented by one.
     * 
     * @param note The note to remove from the buffer.
     * @return True if the buffer did contained the note and it was
     *     successfully removed.
     */ 
    bool pop(Note note);

    /**
     * Shift the content of the buffer to remove empty spaces.
     * 
     * @param start The first index that should get assigned the value of its
     *     neighbor (the next index). All indices after this one and until the
     *     penultimate one gets shifted.
     */
    void shift(int start);

    /**
     * Empty the buffer.
     */
    void reset();

private:
    /// Memory for the notes.
    Note _notes[BUFFER_SIZE];

    /// Index of currently active note. If none (buffer is empty), then the
    /// cursor is -1.
    int _cursor;
};

#endif