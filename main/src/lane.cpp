#include "Arduino.h"
#include "../include/lane.h"

Lane::Lane(Display *display, MCP4822 *dac, bool dac_channel, int gate_pin, int led_id)
{
    _display = display;
    _dac = dac;
    _dac_channel = dac_channel;
    _gate_pin = gate_pin;
    _led_id = led_id;
}

void Lane::setup()
{
    if (_dac_channel)
    {
        _dac->turnOnChannelA();
        _dac->setGainA(MCP4822::High);
    }
    else
    {
        _dac->turnOnChannelB();
        _dac->setGainB(MCP4822::High);
    }
    pinMode(_gate_pin, OUTPUT);
    digitalWrite(_gate_pin, LOW);
}

void Lane::set(int setpoint)
{
    if (_dac_channel)
    {
        _dac->setVoltageA(setpoint);
    }
    else
    {
        _dac->setVoltageB(setpoint);
    }
    _dac->updateDAC();
}

void Lane::play(int setpoint, unsigned long duration)
{
    start(setpoint);
    delay(duration);
    stop();
}

void Lane::start(int setpoint)
{
    set(setpoint);
    digitalWrite(_gate_pin, HIGH);
    _display->set_led_state(_led_id, HIGH);
}

void Lane::stop()
{
    digitalWrite(_gate_pin, LOW);
    _display->set_led_state(_led_id, LOW);
}

int Lane::pitch_to_voltage(byte pitch, int bend)
{
    float bent_pitch = (float)pitch + (float)bend * PITCH_BEND_RANGE / 8192.0;
    float voltage = (bent_pitch - MIDI_MIN_PITCH) / 12.0 / AMP_GAIN;
    if (voltage < 0)
    {
        voltage = 0;
    }
    else if (voltage > DAC_VMAX)
    {
        voltage = DAC_VMAX;
    }
    return (int)(1000 * voltage);
}
