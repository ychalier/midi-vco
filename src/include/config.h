#ifndef config_h
#define config_h

#include "Arduino.h"
#include "constants.h"
#include <EEPROM.h>
#include "structs.h"

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
     *         See flags in constants.h to interpret this value.
     */
    int read();

    /**
     * Getter for the `_polyphony_mode` attribute. Values are defined
     * in constants.h.
     * - 0: MONO
     * - 1: POLY 5
     * - 2: POLY 10
     */
    byte get_polyphony_mode();

    /**
     * Getter for the `_priority_mode` attribute. Values are defined in
     * constants.h.
     * - 0: KEEP FIRST
     * - 1: REPLACE OLDEST
     * - 2: REPLACE NEWEST
     */
    byte get_priority_mode();

    /**
     * Getter for the `_glide_intensity` attribute. Value between -1 and 1.
     * 0 means glide is off.
     */
    float get_glide_intensity();

    /**
     * Getter for the `_pitch_bend_range` attribute. Value in semitons, with a
     * step of 4: 0, 4, 8 and 12.
     */
    float get_pitch_bend_range();

    /**
     * Getter for the `_glide_proportional` attribute.
     */
    bool is_glide_proportional();

    /**
     * Getter for for the `_detune` attribute. Value is between 0 and
     * `DETUNE_VALUE_COUNT`. It is the index for the `DETUNE_VALUES` constant
     * array defined in `constants.h`.
     */
    int get_detune();

    /**
     * Getter for the `_bend_channel_switch` attribute.
     * Represents the state of a toggle switch. `true` means `HIGH`.
     */
    bool get_bend_channel_switch();

    /**
     * Getter for the `_glide_channel_switch` attribute.
     * Represents the state of a toggle switch. `true` means `HIGH`.
     */
    bool get_glide_channel_switch();

    /**
     * Getter for the `_minimum_velocity` attribute. Value is between 0 and 127.
     */
    byte get_minimum_velocity();

    /**
     * Getter the `_tuning_fast` attribute.
     * Represents the state of an instantaneous button. `true` means `HIGH`.
     */
    bool get_tuning_fast();

    /**
     * Getter the `_tuning_fast` attribute.
     * Represents the state of an instantaneous button. `true` means `HIGH`.
     */
    bool get_tuning_full();

    /**
     * Getter the `_tuning_reset` attribute.
     * Represents the state of an instantaneous button. `true` means `HIGH`.
     */
    bool get_tuning_reset();

private:
    byte _polyphony_mode;
    byte _priority_mode;
    float _glide_intensity;
    bool _glide_proportional;
    float _pitch_bend_range;
    int _detune;
    bool _bend_channel_switch;
    bool _glide_channel_switch;
    byte _minimum_velocity;
    bool _tuning_fast;
    bool _tuning_full;
    bool _tuning_reset;

    bool _read_polyphony_mode();
    void _read_pitch_bend_range();
    void _read_glide_intensity();
    void _read_detune();
    bool _read_priority_mode();
    void _read_bend_channel_switch();
    void _read_glide_channel_switch();
    void _read_minimum_velocity();
    bool _read_tuning_fast();
    bool _read_tuning_full();
    bool _read_tuning_reset();
};

#endif