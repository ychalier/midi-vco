#include "Arduino.h"
#include "../include/coupler.h"

Coupler::Coupler(int dac_pin, int gate_pin, Channel* vel_channel)
{
    _dac = new MCP4822(dac_pin);
    _vel_channel = vel_channel;
    _active_a = false;
    _active_b = false;
    _cv_setpoint_a = 0;
    _cv_setpoint_b = 0;
    _cv_a = 0;
    _cv_b = 0;
    _vel_setpoint_a = 0;
    _vel_setpoint_b = 0;
    _vel = 0;
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

void Coupler::set(bool channel, int setpoint_cv, int setpoint_vel)
{
    if (channel)
    {
        _cv_setpoint_a = setpoint_cv;
        _vel_setpoint_a = setpoint_vel;
    }
    else
    {
        _cv_setpoint_b = setpoint_cv;
        _vel_setpoint_b = setpoint_vel;
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
    int cv_a = _cv_setpoint_a;
    int cv_b = _cv_setpoint_b;
    bool gate = _active_a || _active_b;
    bool update_dac = false;
    if (gate && !_active_a)
    {
        cv_a = _cv_setpoint_b;
    }
    else if (gate && !_active_b)
    {
        cv_b = _cv_setpoint_a;
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
    int vel = _vel_setpoint_a;
    if (!_active_a && _active_b) {
        vel = _vel_setpoint_b;
    } else if (_active_b) {
        vel = (_vel_setpoint_a + _vel_setpoint_b) / 2;
    }
    if (vel != _vel) {
        _vel = vel;
        _vel_channel->set(vel);
    }
}

void Coupler::broadcast(int setpoint_cv, int gate)
{
    digitalWrite(_gate_pin, gate);
    _dac->setVoltageA(setpoint_cv);
    _dac->setVoltageB(setpoint_cv);
    _dac->updateDAC();
}