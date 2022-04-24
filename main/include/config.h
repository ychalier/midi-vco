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
     * @param channel The MIDI channel concerned with the change.
     * @param number The control identifier.
     * @param value The current control value, a 7-bit integer.
     * @return A bit-encoding of which inner parameter changed, defined by
     *   constants named `CONFIG_CHANGE_*`
     */
    int handle_midi_control(byte channel, byte number, byte value);

    /**
     * Getter for the _glide_proportional attribute.
     */
    bool is_glide_proportional();

    /**
     * Getter for the _active_source attribute.
     */
    byte get_active_source();

    /**
     * Getter for the _sequencer_record attribute.
     */
    bool should_sequencer_record();

    /**
     * Getter for the _arpeggiator_mode attribute.
     */
    byte get_arpeggiator_mode();

    /**
     * Getter for the _hold attribute.
     */
    bool get_hold();

    /**
     * Getter for the _voltage_offset attribute.
     */
    float get_voltage_offset();

    /**
     * Getter for the _time_period attribute.
     */
    unsigned long get_time_period();

    /**
     * Getter for the _arpeggiator_sustain attribute.
     */
    float get_arpeggiator_sustain();

    /**
     * Getter for the _detune attribute.
     */
    int get_detune();

    /**
     * Getter for the _tuning attribute.
     */
    bool is_tuning();

    byte get_pool_mask();

    int get_sequencer_channel();

    bool get_sequencer_quantize();

    int get_sequencer_steps();

    unsigned long get_time_div();

    int get_sequencer_divisions();


private:
    byte _polyphony_mode;
    byte _priority_mode;
    byte _glide_flags;
    float _glide_intensity;
    bool _glide_proportional;
    float _pitch_bend_range;
    byte _active_source;
    bool _sequencer_record;
    int _button_sequencer_record;
    byte _arpeggiator_mode;
    bool _hold;
    float _voltage_offset;
    unsigned long _time_period;
    float _arpeggiator_sustain;
    int _detune;
    bool _tuning;
    byte _pool_mask;
    int _sequencer_channel;
    bool _sequencer_quantize;
    int _sequencer_steps;

    bool _read_source();
    bool _read_polyphony_mode();
    void _read_pitch_bend_range();
    void _read_glide_intensity();
    void _read_arpeggiator_mode();
    void _read_detune();
    void _read_time();
    bool _read_priority_mode();
    bool _read_sequencer_record();
    bool _read_tuning();
    bool _read_sequencer_channel();
    void _derive_pool_mask();

};

#endif