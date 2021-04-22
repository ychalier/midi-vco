#ifndef pool_h
#define pool_h

#include "Arduino.h"
#include "constants.h"
#include "router.h"
#include "note.h"

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
     * @param channel_mask Channels whitelisted in the pool. Least significant
     *     bits map to the first channels.
     */
    void set_masks(byte lane_mask, unsigned int channel_mask);

    /**
     * Check if the pool includes at least one lane.
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
     * Check if a given channel is whitelisted in the pool.
     * 
     * @param channel A MIDI channel (from 1 to 16).
     * @return Whether the channel is whitelisted.
     */
    bool accepts_channel(byte channel);

    /**
     * Check if a given note could play in the pool, whether it is free or not.
     * This checks if the pool is enabled and if the note passes the channel
     * filter.
     * 
     * @param note The note to check for.
     * @return Whether the note is accepted.
     */
    bool accepts_note(Note note);

    /**
     * Set a new active note, and play that note on the included lanes.
     * 
     * @param note The note to load.
     */
    void load(Note note);

    /**
     * Stop the included lanes and free the pool.
     */
    void unload();

    /**
     * Propagate the *pitch-bend* message to the lanes.
     * 
     * @param bend_value The amount of bending in the *pitch-bend* message.
     */
    void bend(int bend_value);

    /**
     * Getter for the _era attribute.
     */
    unsigned long get_era();

    /**
     * Getter for the _current_note attribute.
     */
    Note get_current_note();

private:
    Router *_router;

    /// Mask for the lanes included in the pool.
    byte _lane_mask;

    /// Mask for the MIDI channels whitelisted in the pool.
    unsigned int _channel_mask;

    /// Last active note.
    Note _current_note;

    /// Timestamp of last note activation.
    unsigned long _era;

    /// Wether a note is currently loaded in the pool.
    bool _active;
};

#endif