#ifndef lane_h
#define lane_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "constants.h"
#include "display.h"
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
     * @param display A pointer to the LED display.
     * @param dac A pointer to the DAC controlling the CV signal of that lane.
     * @param dac_channel If true, use DAC channel A; otherwise use channel B.
     * @param gate_pin PIN controlling the GATE signal.
     * @param led_id Id of the LED to control in the display, mapped to GATE.
     */
    Lane(Config *config, Display *display, Coupler *dac, bool channel, int led_id);

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
    void set(int setpoint, bool update_mate);

    /**
     * Play a note for a given duration. Warning: this methods blocks the whole
     * program, use only for testing purposes.
     * 
     * @param setpoint DAC input value.
     * @param duration Note duration in milliseconds.
     */
    void play(int setpoint, unsigned long duration);

    /**
     * Set the CV signal to a setpoint and set the GATE to HIGH. Use this
     * method to start playing a note without blocking the main loop.
     * 
     * @param setpoint DAC input value.
     */
    void start(int setpoint);

    void set_pitch(byte pitch, int bend);

    void start_pitch(byte pitch, int bend);

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
     * @see [Conversion du MIDI en tension](https://github.com/ychalier/midi-vco/wiki/Conversion-du-MIDI-en-tension)
     * 
     * @param pitch Note pitch in semitons.
     * @param bend Signed 14-bit encoding of the pitch bend.
     * @return DAC input value corresponding to that note.
     */
    static int pitch_to_voltage(Config *config, byte pitch, int bend);

    static byte setpoint_to_pitch(int setpoint);

private:
    Config *_config;
    Display *_display;
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