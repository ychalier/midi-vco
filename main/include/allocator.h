#ifndef allocator_h
#define allocator_h

#include "Arduino.h"
#include "constants.h"
#include "config.h"
#include "pool.h"
#include "router.h"
#include "structs.h"

/**
 * Main wrapper for the pools that handles callbacks from the MIDI library.
 */
class Allocator
{
public:
    /**
     * Construct an allocator given a config.
     *
     * @param config The config to pass to the allocator.
     * @param router A pointer to the router handling the output lanes.
     */
    Allocator(Config *config, Router *router);

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Set the pool's masks according to the current configuration. All pools
     * will be stopped and reset, even if they're active.
     */
    void set_lane_masks();

    /**
     * Handler for the *note-on* MIDI message. The note will be allocated to a
     * pool if there's one available right now or if a priority rule says to
     * make room for it.
     *
     * @see https://github.com/ychalier/midi-vco/wiki/Politique-d'allocation-des-voies
     *
     */
    void note_on(Note note);

    /**
     * Handler for the *note-off* MIDI message. A pool currently playing that
     * note will stop playing it.
     *
     * @param note The note associated with the *note-off* message.
     */
    void note_off(Note note);

    /**
     * Handler for the *note-on* MIDI message, with an arbitrary lane mask.
     * The note will be allocated to a pool if there's one available right
     * now or if a priority rule says to make room for it. Only pools which
     * indices match the pool mask are considered as valid candiates.
     *
     * @see `Allocator.check_mask`
     *
     * @param mask An 16-bit pool mask
     */
    void note_on_masked(Note note, uint16_t mask);

    /**
     * Handler for the *note-off* MIDI message, with an arbitrary lane mask.
     * A pool, matching the mask, and currently playing that note will stop
     * playing it.
     *
     * @see `Allocator.check_mask`
     *
     * @param mask A 16-bit pool mask
     */
    void note_off_masked(Note note, uint16_t mask);

    /**
     * Handler for the *pitch-bend* MIDI message.
     *
     * @param bend Signed 14-bit integer (between -8192 and 8191).
     */
    void pitch_bend(int bend);

    /**
     * Force reset for all the pools, their buffers, and the lanes.
     */
    void reset();

    /**
     * Force reset pools corresponding to the mask.
     * 
     * @param mask Selector for pools to reset.
     */
    void reset_masked(uint16_t mask);

    /**
     * Artificially send an output to all lanes. This is used during tuning.
     * 
     * @param setpoint CV setpoint to broadcast to all lanes
     * @param gate Gate state to broadcast to all couplers
     */
    void broadcast(int setpoint, int gate);

    /**
     * Checks whether a bit mask includes a value.
     *
     * @param mask A 16-bit mask of the form 0bXXXXXXXXXXXXXXXX
     * @param value A value between 0 and 7, indicating the bit index to check
     *   the value of. Leftmost (most significant) bit is 15. Rightmost (least
     *   significant) is 0.
     * @return Whether mask's bit at position value is 1.
     */
    static bool check_mask(uint16_t mask, int value);

    /**
     * Check if at least one pool is active.
     * 
     * @return true At least one pool is active.
     * @return false All pools are inactive.
     */
    bool is_active();

private:
    /// A reference to the global user config.
    Config *_config;

    /// One router for controlling the output lanes.
    Router *_router;

    /// Array of lane pools; the size of this array should be the same as the
    /// number of output lanes.
    Pool *_pools[POOL_COUNT];
};

#endif