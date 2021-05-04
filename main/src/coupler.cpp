#include "Arduino.h"
#include "../include/coupler.h"

Coupler::Coupler(int dac_pin, int gate_pin)
{
    _dac = new MCP4822(dac_pin);
    _active_a = false;
    _active_b = false;
    _setpoint_a = 0;
    _setpoint_b = 0;
    _cv_a = 0;
    _cv_b = 0;
    _gate = false;
    _gate_pin = gate_pin;
}

void Coupler::setup()
{
    _dac->init();
    _dac->turnOnChannelA();
    _dac->setGainA(MCP4822::High);
    _dac->turnOnChannelB();
    _dac->setGainB(MCP4822::High);
    pinMode(_gate_pin, OUTPUT);
    digitalWrite(_gate_pin, LOW);
}

void Coupler::set(bool channel, int setpoint)
{
    if (channel)
    {
        _setpoint_a = setpoint;
    }
    else
    {
        _setpoint_b = setpoint;
    }
    update();
}

void Coupler::activate(bool channel)
{
    if (channel)
    {
        _active_a = true;
    }
    else
    {
        _active_b = true;
    }
    update();
}

void Coupler::deactivate(bool channel)
{
    if (channel)
    {
        _active_a = false;
    }
    else
    {
        _active_b = false;
    }
    update();
}

void Coupler::update()
{
    int cv_a = _setpoint_a;
    int cv_b = _setpoint_b;
    bool gate = _active_a || _active_b;
    bool update_dac = false;
    if (gate && !_active_a)
    {
        cv_a = _setpoint_b;
    }
    else if (gate && !_active_b)
    {
        cv_b = _setpoint_a;
    }
    if (_cv_a != cv_a)
    {
        _cv_a = cv_a;
        _dac->setVoltageA(cv_a);
        update_dac = true;
    }
    if (_cv_b != cv_b)
    {
        _cv_b = cv_b;
        _dac->setVoltageB(cv_b);
        update_dac = true;
    }
    if (update_dac)
    {
        _dac->updateDAC();
    }
    if (_gate != gate)
    {
        _gate = gate;
        if (gate)
        {
            digitalWrite(_gate_pin, HIGH);
        }
        else
        {
            digitalWrite(_gate_pin, LOW);
        }
    }
}