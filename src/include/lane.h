#ifndef cv_lane_h
#define cv_lane_h

#include "Arduino.h"
#include "config.h"
#include "constants.h"
#include "coupler.h"
#include <MCP48xx.h>
#include "structs.h"

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
     * @param tuning A pointer to the `Tuning` parameters of this lane
     */
    Lane(Config *config, Coupler *coupler, bool channel, Tuning *tuning);

    /**
     * Set the CV and velocity signals to setpoints.
     *
     * @param cv_setpoint DAC input value for the CV signal.
     * @param vel_setpoint DAC input value for the velocity signal.
     */
    void set(int cv_setpoint, int vel_setpoint);

    /**
     * Set the CV signal to a setpoint and set the GATE to HIGH. Use this
     * method to start playing a note without blocking the main loop.
     *
     * @param cv_setpoint DAC input value for the CV signal.
     * @param vel_setpoint DAC input value for the velocity signal.
     */
    void start(int cv_setpoint, int vel_setpoint);

    /**
     * Set a note to be played according to its pitch.
     *
     * @see `Lane.set`
     *
     * @param pitch 7-bit pitch encoding.
     * @param bend Signed 14-bit encoding of the pitch-bend on that value.
     * @param ignore_detune Ignore detune if true.
     */
    void set_note(Note note, int bend, bool ignore_detune);

    /**
     * Start a note to be played according to its pitch.
     *
     * @see `Lane.start`
     *
     * @param pitch 7-bit pitch encoding.
     * @param bend Signed 14-bit encoding of the pitch-bend on that value.
     * @param ignore_detune Ignore detune if true.
     */
    void start_note(Note note, int bend, bool ignore_detune);

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
     * Basic computation for the DAC input value from a pitch. Independent of
     * runtime configuration.
     *
     * @see https://github.com/ychalier/midi-vco/wiki/Conversion-du-MIDI-en-tension
     *
     * @param pitch Note pitch in semitons.
     * @return DAC input value corresponding to that pitch (0 - 4096).
     */
    static float base_pitch_to_voltage(float pitch);

    /**
     * Compute the DAC input value from a pitch. Takes voltage offset (see
     * `Config`) and tuning into account.
     *
     * @see https://github.com/ychalier/midi-vco/wiki/Conversion-du-MIDI-en-tension
     *
     * @param pitch Note pitch in semitons (0 - 127).
     * @param bend Signed 14-bit encoding of the pitch bend (-8192 - 8191).
     * @return DAC input value corresponding to that pitch (0 - 4096).
     */
    int pitch_to_voltage(byte pitch, int bend);

    /**
     * Map a velocity to a voltage.
     * 
     * @param velocity Velocity value between 0 and 127.
     * @return DAC input value corresponding to that velocity (0 - 4096). 
     */
    int velocity_to_voltage(byte velocity);

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
    Tuning *_tuning;

    /// True if the lane is currently active.
    bool _active;

    /// Last CV setpoint (including pitch bends).
    int _current_cv_setpoint;

    /// Last velocity setpoint.
    int _current_vel_setpoint;

    /// Wrapper for the glide information.
    Glide _glide;
};

#endif