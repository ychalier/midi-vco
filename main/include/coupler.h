#ifndef coupler_h
#define coupler_h

#include "Arduino.h"
#include "channel.h"
#include <MCP48xx.h>

/**
 * Implements the paraphonic feature. Simulates two output lanes using two CV
 * signals, one shared GATE signal and one shared VELOCITY signal.
 */
class Coupler
{
public:
    /**
     * Constructor.
     *
     * @param dac_pin SS PIN controller for the DAC corresponding to the
     *   two lanes bound to the coupler.
     * @param gate_pin PIN for the output GATE signal.
     * @param vel_channel Reference to a DAC channel for the velocity output.
     */
    Coupler(int dac_pin, int gate_pin, Channel* vel_channel);

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Set the CV signal for one of the two lanes.
     *
     * @param channel Selector for which lane to update (A is `true`, B is
     *   `false`).
     * @param setpoint_cv Setpoint for CV output (0-4096).
     * @param setpoint_vel Setpoint for velocity output (0-4096).
     */
    void set(bool channel, int setpoint_cv, int setpoint_vel);

    /**
     * Activate one of the virtual lanes.
     *
     * @param channel Which lane to activate (A is `true`, B is `false`).
     */
    void activate(bool channel);

    /**
     * Deactivate one of the virtual lanes.
     *
     * @param channel Which lane to deactivate (A is `true`, B is `false`).
     */
    void deactivate(bool channel);

    /**
     * Update the real output based on the previous and the current logical
     * state.
     */
    void update();

    /**
     * Turn the gate signal on or off. Does not force: if the gate passed as
     * argument is the same as the internal state, nothing happens.
     * 
     * @param gate Gate state, `HIGH` or `LOW`.
     */
    void set_gate(int gate);

private:
    /// A DAC interface to control the CV signals.
    MCP4822 *_dac;

    /// A reference to the DAC Channel for outputting velocity.
    Channel *_vel_channel;

    /// Whether the channel A is active or not.
    bool _active_a;

    /// Whether the channel B is active or not.
    bool _active_b;

    /// Logical CV setpoint for channel A. This ignores the value of channel B.
    int _cv_setpoint_a;

    /// Logical CV setpoint for channel B. This ignores the value of channel A.
    int _cv_setpoint_b;

    /// Actual CV setpoint for channel A. May differ from `_cv_setpoint_a` if the
    /// channel is off while the other is on.
    int _cv_a;

    /// Actual CV setpoint for channel B. May differ from `_cv_setpoint_b` if the
    /// channel is off while the other is on.
    int _cv_b;

    /// Logical velocity setpoint for channel A.
    int _vel_setpoint_a;

    /// Logical velocity setpoint for channel B.
    int _vel_setpoint_b;

    /// Actual velocity setpoint.
    int _vel;

    /// Current state of the GATE signal.
    bool _gate;

    /// Gate pin.
    int _gate_pin;
};

#endif