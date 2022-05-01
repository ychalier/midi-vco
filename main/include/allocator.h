#ifndef allocator_h
#define allocator_h

#include "Arduino.h"
#include "constants.h"
#include "config.h"
#include "structs.h"
#include "pool.h"
#include "router.h"

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
    void note_on(byte pitch);

    /**
     * Handler for the *note-off* MIDI message. A pool currently playing that
     * note will stop playing it.
     *
     * @param note The note associated with the *note-off* message.
     */
    void note_off(byte pitch);

    /**
     * Handler for the *note-on* MIDI message, with an arbitrary lane mask.
     * The note will be allocated to a pool if there's one available right
     * now or if a priority rule says to make room for it. Only pools which
     * indices match the pool mask are considered as valid candiates.
     *
     * @see `Allocator.check_mask`
     *
     * @param mask An 8-bit pool mask
     */
    void note_on_masked(byte pitch, byte mask);

    /**
     * Handler for the *note-off* MIDI message, with an arbitrary lane mask.
     * A pool, matching the mask, and currently playing that note will stop
     * playing it.
     *
     * @see `Allocator.check_mask`
     *
     * @param mask An 8-bit pool mask
     */
    void note_off_masked(byte pitch, byte mask);

    /**
     * Handler for the *pitch-bend* MIDI message.
     *
     * @param bend Signed 14-bit integer, sum of pitch bend and after touch
     */
    void pitch_bend(int bend);

    /**
     * Handler for the polyphonic *after-touch* MIDI message. Behaves similarly
     * to the pitch bend.
     *
     * @param note The note concerned with the message.
     * @param bend Signed 14-bit integer, sum of pitch bend and after touch
     */
    void after_touch_poly(byte pitch, int bend);

    /**
     * Handler for the channel *after-touch* MIDI message. Same as
     * `after_touch_poly` but for a whole MIDI channel.
     *
     * @param bend Signed 14-bit integer, sum of pitch bend and after touch
     */
    void after_touch_channel(int bend);

    /**
     * Force reset for all the pools, their buffers, and the lanes.
     */
    void reset();

    void reset_masked(byte mask);

    /**
     * Lock all active pools in their current state.
     */
    void hold_on();

    /**
     * Unlock all locked pools.
     */
    void hold_off();

    /**
     * Artificially send an output to all lanes. This is used during tuning.
     */
    void broadcast(byte pitch, int gate);

    /**
     * Checks whether a bit mask includes a value.
     *
     * @param mask An 8-bit mask of the form 0bXXXXXXXX
     * @param value A value between 0 and 7, indicating the bit index to check
     *   the value of. Leftmost (most significant) bit is 7. Rightmost (least
     *   significant) is 0.
     * @return Whether mask's bit at position value is 1.
     */
    static bool check_mask(byte mask, int value);

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