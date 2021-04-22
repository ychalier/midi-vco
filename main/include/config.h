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

private:
    byte _polyphony_mode;
    byte _priority_mode;
    byte _channel_filter;
};

#endif