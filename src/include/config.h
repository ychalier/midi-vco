#ifndef config_h
#define config_h

#include "Arduino.h"
#include "constants.h"

/**
 * Main wrapper for the user interface.
 */
class Config
{
public:
    Config();

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Check all connected inputs for their current value and store it in the
     * configuration.
     *
     * @return Whether any change in configuration occured during the reading.
     */
    int read();

    /**
     * Getter for the _polyphony_mode attribute.
     */
    byte get_polyphony_mode();

    /**
     * Getter for the _priority_mode attribute.
     */
    byte get_priority_mode();

    /**
     * Getter for the _glide_flags attribute.
     */
    byte get_glide_flags();

    /**
     * Getter for the _glide_intensity attribute.
     */
    float get_glide_intensity();

    /**
     * Getter for the _pitch_bend_range attribute.
     */
    float get_pitch_bend_range();

    /**
     * Callback to handle MIDI controls.
     *
     * @param number The control identifier.
     * @param value The current control value, a 7-bit integer.
     * @return A bit-encoding of which inner parameter changed, defined by
     *   constants named `CONFIG_CHANGE_*`
     */
    int handle_midi_control(byte number, byte value);

    /**
     * Getter for the _glide_proportional attribute.
     */
    bool is_glide_proportional();

    /**
     * Getter for the _hold attribute.
     */
    bool get_hold();

    /**
     * Getter for the _voltage_offset attribute.
     */
    float get_voltage_offset();

    /**
     * Getter for the _detune attribute.
     */
    int get_detune();

    /**
     * Getter for the _tuning attribute.
     */
    bool is_tuning();

private:
    byte _polyphony_mode;
    byte _priority_mode;
    byte _glide_flags;
    float _glide_intensity;
    bool _glide_proportional;
    float _pitch_bend_range;
    bool _hold;
    float _voltage_offset;
    int _detune;
    bool _tuning;

    bool _read_polyphony_mode();
    void _read_pitch_bend_range();
    void _read_glide_intensity();
    void _read_detune();
    bool _read_priority_mode();
    bool _read_tuning();
};

#endif