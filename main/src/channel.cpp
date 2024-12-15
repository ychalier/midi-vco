#include "Arduino.h"
#include "../include/channel.h"

Channel::Channel(MCP4822* dac, bool channel)
{
    _dac = dac;
    _channel = channel;
}

void Channel::set(int setpoint)
{
    if (_channel)
    {
        _dac->setVoltageA(setpoint);
    }
    else
    {
        _dac->setVoltageB(setpoint);
    }
    _dac->updateDAC();
}
