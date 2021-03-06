#ifndef allocator_h
#define allocator_h

#include "Arduino.h"
#include "constants.h"
#include "config.h"
#include "structs.h"
#include "pool.h"
#include "router.h"
#include "display.h"

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
     * @param display A pointer to the LED display wrapper.
     * @param router A pointer to the router handling the output lanes.
     */
    Allocator(Config *config, Display *display, Router *router);

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Set the pool's masks according to the current configuration. All pools
     * will be stopped and reset, even if they're active.
     */
    void set_masks();

    /**
     * Display the current pools settings using the LED display. LED *i* will
     * lit up iff. pool *i* is accepting notes.
     */
    void display_state();

    /**
     * Handler for the *note-on* MIDI message. The note will be allocated a
     * pool if there's one available right now or if a priority rule says to
     * make room for it.
     * 
     * @see https://github.com/ychalier/midi-vco/wiki/Politique-d'allocation-des-voies
     * 
     * @param note The note associated with the *note-on* message.
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
     * Handler for the *pitch-bend* MIDI message.
     * 
     * @param channel MIDI channel concerned with the bending; only concerned
     *     pools (regarding their channel mask) will be affected.
     * @param bend Signed 14-bit encoding of the current position of the
     *     pitch-bend control.
     */
    void pitch_bend(byte channel, int bend);

    /**
     * Handler for the polyphonic *after-touch* MIDI message. Behaves similarly
     * to the pitch bend.
     * 
     * @param note The note concerned with the message.
     * @param pressure 7-bit encoding of the amount of pressure on that note.
     */
    void after_touch_poly(Note note, byte pressure);

    /**
     * Handler for the channel *after-touch* MIDI message. Same as
     * `after_touch_poly` but for a whole MIDI channel.
     * 
     * @param channel MIDI channel ID
     * @param pressure 7-bit encoding of the amount of pressure.
     */
    void after_touch_channel(byte channel, byte pressure);

    /**
     * Force reset for all the pools, their buffers, and the lanes.
     */
    void reset();

    /**
     * Lock all active pools in their current state.
     */
    void hold_on();

    /**
     * Unlock all locked pools.
     */
    void hold_off();

private:
    /// A reference to the global user config.
    Config *_config;

    /// A reference to the global LED display.
    Display *_display;

    /// One router for controlling the output lanes.
    Router *_router;

    /// Array of lane pools; the size of this array should be the same as the
    /// number of output lanes.
    Pool *_pools[LANE_COUNT];
};

#endif