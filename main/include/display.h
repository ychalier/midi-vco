#ifndef display_h
#define display_h

#include "Arduino.h"
#include "constants.h"

/**
 * Wrapper for the LED display.
 * 
 * Other components in the code should use this to update the state of an LED.
 * It allows for non-blocking pattern display on top of such updates.
 */
class Display
{

public:
    Display();

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Set all LEDs to LOW.
     */
    void reset_leds();

    /**
     * Load and display a new pattern.
     * 
     * @param pattern Mask for which LED to lit. Least significant bits map to
     *     first LEDs.
     * @param duration How long to maintain the pattern for, in milliseconds.
     */
    void start(byte pattern, unsigned long duration);

    /**
     * Set the LED pins state from the current pattern.
     */
    void set_pattern();

    /**
     * Check if the current pattern has expired. If it is the case, LEDs are
     * reset and the lock is removed. This function is safe for repeated calls
     * in the main loop.
     */
    void update();

    /**
     * Manually set the state of an LED. This has no effect if the display is
     * locked. External components must use this function to alter the LED
     * display.
     * 
     * @param led_id Id of the LED to alter.
     * @param state Either HIGH (lit) or LOW (off).
     */
    void set_led_state(int led_id, int state);

    /**
     * Retrieve the state that a given LED has within the current pattern.
     *
     * @param led_id Id of the LED to retrieve the target state of.
     * @return Either HIGH (lit) or LOW (off).
     */
    int get_led_state(int led_id);

    /**
     * A quick animation for testing the LEDs.
     */
    void demo();

private:
    /// Array containing LED pins.
    int _leds[LANE_COUNT];

    /// If true, external components may not alter LEDs states.
    bool _locked;

    /// Mask encoding which LEDs are lit in the display pattern.
    /// Least significant bits map to firsts LEDs.
    byte _pattern;

    /// Timestamp of when the target display started, in milliseconds.
    unsigned long _start;

    /// How long to display the target display for, in milliseconds.
    unsigned long _duration;
};

#endif