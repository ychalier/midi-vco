#include "Arduino.h"
#include "../include/lane.h"

Lane::Lane(Config *config, Display *display, MCP4822 *dac, bool dac_channel, int gate_pin, int led_id)
{
    _config = config;
    _display = display;
    _dac = dac;
    _dac_channel = dac_channel;
    _gate_pin = gate_pin;
    _led_id = led_id;
    _current_setpoint = 0;
    _last_set_time = 0;
    _glide = {false, 0, 0, 0, 0};
    _active = false;
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

void Lane::start(int setpoint)
{
    float glide_intensity = _config->get_glide_intensity();
    byte glide_flags = _config->get_glide_flags();
    if (glide_intensity == 0 ||
        (!_active && (GLIDE_FLAG_LEGATO & glide_flags)))
    {
        set(setpoint);
    }
    else
    {
        _glide.active = true;
        _glide.setpoint_start = _current_setpoint;
        _glide.setpoint_end = setpoint;
        _glide.time_start = millis();
        if (GLIDE_FLAG_PROPORTIONAL & glide_flags)
        {
            float gap = setpoint_to_pitch(fabs(_current_setpoint - setpoint)) - MIDI_MIN_PITCH;
            _glide.duration = pow(glide_intensity, GLIDE_INTENSITY_POWER) * GLIDE_MAX_RATE * gap;
        }
        else
        {
            _glide.duration = pow(glide_intensity, GLIDE_INTENSITY_POWER) * GLIDE_MAX_TIME;
        }
        update();
    }
    digitalWrite(_gate_pin, HIGH);
    _display->set_led_state(_led_id, HIGH);
    _active = true;
}

void Lane::stop()
{
    digitalWrite(_gate_pin, LOW);
    _display->set_led_state(_led_id, LOW);
    _active = false;
}

void Lane::update()
{
    if (_glide.active)
    {
        float progress = (float)(millis() - _glide.time_start) / duration;
        if (progress > 1)
        {
            progress = 1;
        }
        float setpoint = (1 - progress) * _glide.setpoint_start + progress * _glide.setpoint_end;
        if (GLIDE_FLAG_CHROMATIC & _config->get_glide_flags() && progress < 1)
        {
            setpoint = pitch_to_voltage(setpoint_to_pitch(setpoint), 0);
        }
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

byte Lane::setpoint_to_pitch(int setpoint)
{
    float voltage = (float)setpoint / 1000.0;
    return (byte)(voltage * AMP_GAIN * 12.0) + MIDI_MIN_PITCH;
}
