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
     * Associate a polyphony mode given an analog user input.
     * 
     * @param input_value Analog input, a number between 0 and 1023.
     * @return The associated polyphony mode.
     */
    static byte categorize_polyphony_mode(int input_value);

    /**
     * Associate a priority mode given an digital user input.
     * 
     * @param input_value Digital input, either HIGH or LOW.
     * @return The associated priority mode.
     */
    static byte categorize_priority_mode(int input_value);

    /**
     * Associate a channel filter given an digital user input.
     * 
     * @param input_value Digital input, either HIGH or LOW.
     * @return The associated channel filter.
     */
    static byte categorize_channel_filter(int input_value);

    /**
     * Check all connected inputs for their current value and store it in the
     * configuration.
     * 
     * @return Whether any change in configuration occured during the reading.
     */
    bool read();

    /**
     * Getter for the _polyphony_mode attribute.
     */
    byte get_polyphony_mode();

    /**
     * Getter for the _priority_mode attribute.
     */
    byte get_priority_mode();

    /**
     * Getter for the _channel_filter attribute.
     */
    byte get_channel_filter();

    /**
     * Getter for the _glide_flags attribute.
     */
    byte get_glide_flags();

    /**
     * Getter for the _glide_intensity attribute.
     */
    float get_glide_intensity();

    /**
     * Getter for the _pitch_bend_range function.
     */
    float get_pitch_bend_range();

    int handle_midi_control(byte channel, byte number, byte value);

    bool is_glide_proportional();

    byte get_active_source();

    bool should_sequencer_record();

    float get_sequencer_time_factor();

    byte get_arpeggiator_mode();

    unsigned long get_arpeggiator_period();

private:
    byte _polyphony_mode;
    byte _priority_mode;
    byte _channel_filter;
    byte _glide_flags;
    float _glide_intensity;
    bool _glide_proportional;
    float _pitch_bend_range;
    byte _active_source;
    bool _sequencer_record;
    float _sequencer_time_factor;
    byte _arpeggiator_mode;
    unsigned long _arpeggiator_period;
};

#endif