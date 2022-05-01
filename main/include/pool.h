#ifndef pool_h
#define pool_h

#include "Arduino.h"
#include <MIDI.h>
#include "constants.h"
#include "router.h"
#include "buffer.h"
#include "structs.h"

/**
 * Abstract representation of an lane allocation unit.
 * 
 * @see [Politique d'allocation des voies](https://github.com/ychalier/midi-vco/wiki/Politique-d'allocation-des-voies)
 */
class Pool
{

public:
    /**
     * Constructor.
     * 
     * @param router Pointer to the router holding the lanes.
     */
    Pool(Router *router);

    /**
     * Setter for the masks.
     * 
     * @param lane_mask Lanes included in the pool. Least significant bits map
     *     to the firsts lanes.
     */
    void set_lane_mask(byte lane_mask);

    /**
     * Check if the pool includes at least one lane and is not locked.
     * 
     * @return Whether the pool is enabled.
     */
    bool is_enabled();

    /**
     * Check if the pool does not have an active note.
     * 
     * @return Whether the pool is free.
     */
    bool is_free();

    /**
     * Getter for the _active attribute.
     */
    bool is_active();

    /**
     * Set a new active note, and play that note on the included lanes.
     * 
     * @param note The note to load.
     */
    void load(byte pitch);

    /**
     * Attempt to unload a note, if it is in the buffer. In that case, stop the
     * included lanes and free the pool.
     * 
     * @param note The note to unload.
     * @return True if the note was in the buffer and stopped.
     */
    bool unload(byte pitch);

    /**
     * Load the current note in the buffer on the lanes.
     * 
     * @param bend The pitch bend value to associate with the note.
     * @param set_only If true, the note is set one the lanes. If false, the
     *     note is started on the lanes, which impacts the GATE signal.
     */
    void load_buffer(int bend, bool set_only);

    /**
     * Stop all the lanes and reset the buffer.
     */
    void stop();

    /**
     * Propagate the *pitch-bend* message to the lanes.
     * 
     * @param bend_value The amount of bending (pitch bend & after touch)
     */
    void bend(int bend_value);

    /**
     * Getter for the _era attribute.
     */
    unsigned long get_era();

    /**
     * Lock the pool. Incoming note-on messages will be ignored. Incoming
     * note-off messages will remove notes from the internal buffer, but that
     * will not affect the lanes until the pool gets unlocked.
     */
    void lock();

    /**
     * Unlock the pool. Update the lanes with if necessary.
     */
    void unlock();

    /**
     * Check whether the pool's buffer contains a note.
     * 
     * @param note The note to find.
     * @return `true` if the internal buffer contains that note.
     */
    bool buffer_contains(byte pitch);

private:
    Router *_router;

    /// Mask for the lanes included in the pool.
    byte _lane_mask;

    /// Note buffer.
    Buffer *_buffer;

    /// Timestamp of last note activation.
    unsigned long _era;

    /// Whether a note is currently loaded in the pool.
    bool _active;

    /// Whether the pool is locked.
    bool _locked;
};

#endif