#ifndef coupler_h
#define coupler_h

#include "Arduino.h"
#include <MCP48xx.h>

#define COUPLER_A true
#define COUPLER_B false

/**
 * Implements the paraphonic feature. Simulates two output lanes using two CV
 * signals and one shared GATE signal.
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
     */
    Coupler(int dac_pin, int gate_pin);

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
     * @param setpoint DAC setpoint value.
     */
    void set(bool channel, int setpoint);

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

    void broadcast(int setpoint, int gate);

private:
    /// A DAC interface to control the CV signals.
    MCP4822 *_dac;

    /// Whether the channel A is active or not.
    bool _active_a;

    /// Whether the channel B is active or not.
    bool _active_b;

    /// Logical CV setpoint for channel A. This ignores the value of channel B.
    int _setpoint_a;

    /// Logical CV setpoint for channel B. This ignores the value of channel A.
    int _setpoint_b;

    /// Actual CV setpoint for channel A. May differ from `_setpoint_a` if the
    /// channel is off while the other is on.
    int _cv_a;

    /// Actual CV setpoint for channel B. May differ from `_setpoint_b` if the
    /// channel is off while the other is on.
    int _cv_b;

    /// Current state of the GATE signal.
    bool _gate;

    int _gate_pin;
};

#endif