#ifndef channel_h
#define channel_h

#include "Arduino.h"
#include <MCP48xx.h>

/**
 * Implements a DAC with two channels, A and B.
 */
class Channel
{
public:
    /**
     * Constructor.
     *
     * @param dac Reference to the DAC interface
     * @param channel Either A (true) or B (false)
     */
    Channel(MCP4822* dac, bool channel);

    /**
     * Set the signal of the DAC on the specified channel.
     *
     * @param setpoint DAC setpoint value.
     */
    void set(int setpoint);

private:
    /// A DAC interface.
    MCP4822 *_dac;

    /// Channel
    bool _channel;

};

#endif