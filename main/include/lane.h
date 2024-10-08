#ifndef lane_h
#define lane_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "constants.h"
#include "config.h"
#include "structs.h"
#include "coupler.h"

/**
 * Manage one output VCO (CV and GATE signals).
 */
class Lane
{
public:
    /**
     * Constructor.
     *
     * @param config A pointer to the global configuration.
     * @param coupler A pointer to `Coupler` controlling the CV and GATE
     *   signals.
     * @param channel If true, use `Coupler` channel A; otherwise use channel
     *   B.
     * @param led_id Id of the LED to control in the display, mapped to GATE.
     */
    Lane(Config *config, Coupler *coupler, bool channel, int led_id);

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Set the CV signal to a setpoint.
     *
     * @param setpoint DAC input value.
     */
    void set(int setpoint);

    /**
     * Set the CV signal to a setpoint and set the GATE to HIGH. Use this
     * method to start playing a note without blocking the main loop.
     *
     * @param setpoint DAC input value.
     */
    void start(int setpoint);

    /**
     * Set a note to be played according to its pitch.
     *
     * @see `Lane.set`
     *
     * @param pitch 7-bit pitch encoding.
     * @param bend Signed 14-bit encoding of the pitch-bend on that value.
     */
    void set_pitch(byte pitch, int bend, bool ignore_detune);

    /**
     * Start a note to be played according to its pitch.
     *
     * @see `Lane.start`
     *
     * @param pitch 7-bit pitch encoding.
     * @param bend Signed 14-bit encoding of the pitch-bend on that value.
     */
    void start_pitch(byte pitch, int bend, bool ignore_detune);

    /**
     * Set the GATE to LOW.
     */
    void stop();

    /**
     * Check if a glide is active and update it if necessary. This function
     * should be called repeateadly from the main loop.
     */
    void update();

    /**
     * Compute the DAC input value from a note.
     *
     * @see https://github.com/ychalier/midi-vco/wiki/Conversion-du-MIDI-en-tension
     *
     * @param pitch Note pitch in semitons.
     * @param bend Signed 14-bit encoding of the pitch bend.
     * @return DAC input value corresponding to that note.
     */
    static int pitch_to_voltage(Config *config, byte pitch, int bend);

    /**
     * Reverse the pitch to voltage computation.
     *
     * @param setpoint The DAC setpoint to convert.
     * @return The corresponding note pitch as a 7-bit integer (rounded to the
     *   closest integer match).
     */
    static byte setpoint_to_pitch(int setpoint);

private:
    Config *_config;
    Coupler *_coupler;
    bool _channel;
    int _led_id;

    /// True if the lane is currently active.
    bool _active;

    /// Last setpoint (including pitch bends).
    int _current_setpoint;

    /// Wrapper for the glide information.
    Glide _glide;
};

#endif