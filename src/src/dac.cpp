#include "Arduino.h"
#include "../include/dac.h"

Dac::Dac(int dac_pin)
{
    _dac = new MCP4822(dac_pin);
    _channel_a = new Channel(_dac, CHANNEL_A);
    _channel_b = new Channel(_dac, CHANNEL_B);
}

void Dac::setup()
{
    _dac->init();
    _dac->turnOnChannelA();
    _dac->setGainA(MCP4822::High);
    _dac->turnOnChannelB();
    _dac->setGainB(MCP4822::High);
}

Channel* Dac::get(bool channel) {
    if (channel) {
        return _channel_a;
    } else {
        return _channel_b;
    }
}