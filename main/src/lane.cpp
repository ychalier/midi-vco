#include "Arduino.h"
#include "../include/lane.h"

Lane::Lane(Display *display, MCP4822 *dac, bool dac_channel, int gate_pin, int led_id)
{
    _display = display;
    _dac = dac;
    _dac_channel = dac_channel;
    _gate_pin = gate_pin;
    _led_id = led_id;
    _current_setpoint = 0;
    _last_set_time = 0;
    _glide = {false, 0, 0, 0, 0};
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
    _current_setpoint = setpoint;
    _last_set_time = millis();
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
    start(setpoint, 0);
    delay(duration);
    stop();
}

void Lane::start(int setpoint, unsigned long glide_duration)
{
    if (glide_duration == 0 ||
        (millis() - _last_set_time) > GLIDE_TIME_TRIGGER ||
        abs(_current_setpoint - setpoint) > GLIDE_PITCH_TRIGGER)
    {
        set(setpoint);
    }
    else
    {
        _glide.active = true;
        _glide.setpoint_start = _current_setpoint;
        _glide.setpoint_end = setpoint;
        _glide.duration = glide_duration;
        _glide.time_start = millis();
        update();
    }
    digitalWrite(_gate_pin, HIGH);
    _display->set_led_state(_led_id, HIGH);
}

void Lane::stop()
{
    digitalWrite(_gate_pin, LOW);
    _display->set_led_state(_led_id, LOW);
}

void Lane::update()
{
    if (_glide.active)
    {
        float duration = 50.0 + (float)abs(_glide.setpoint_end - _glide.setpoint_start) * 3.0 / 250.0 / 60.0 * 450.0;
        if (duration > 500) {
            duration = 500;
        }
        float progress = (float)(millis() - _glide.time_start) / duration;  // (float)_glide.duration;
        if (progress > 1)
        {
            progress = 1;
        }
        float setpoint = (1 - progress) * _glide.setpoint_start + progress * _glide.setpoint_end;
        set((int)setpoint);
        if (progress >= 1)
        {
            _glide.active = false;
        }
    }
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
