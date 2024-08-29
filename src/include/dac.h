#ifndef dac_h
#define dac_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "constants.h"
#include "channel.h"

/**
 * Implements a DAC with two channels, A and B.
 */
class Dac
{
public:
    /**
     * Constructor.
     *
     * @param dac_pin
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
     * @param channel 
     */
    Channel *get(bool channel);

private:
    /// A DAC interface.
    MCP4822 *_dac;

    Channel *_channel_a;

    Channel *_channel_b;

};

#endif