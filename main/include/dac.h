#ifndef dac_h
#define dac_h

#include "Arduino.h"
#include "channel.h"
#include "constants.h"
#include <MCP48xx.h>

/**
 * Implements a DAC with two channels, A and B.
 */
class Dac
{
public:
    /**
     * Constructor.
     *
     * @param dac_pin SS PIN connected to the DAC.
     *
     */
    Dac(int dac_pin);

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Channel getter
     * 
     * @param channel `true` for channel A, `false` for channel B.
     */
    Channel *get(bool channel);

private:
    /// A DAC interface.
    MCP4822 *_dac;

    /// Pointer to channel A.
    Channel *_channel_a;

    /// Pointer to channel B.
    Channel *_channel_b;

};

#endif