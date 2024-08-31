#include "Arduino.h"
#include "../include/lane.h"

Lane::Lane(Config *config, Coupler *coupler, bool channel)
{
    _config = config;
    _coupler = coupler;
    _channel = channel;
    _current_cv_setpoint = 0;
    _glide = {false, 0, 0, 0, 0};
    _active = false;
}

void Lane::setup()
{
}

void Lane::set(int cv_setpoint, int vel_setpoint)
{
    _current_cv_setpoint = cv_setpoint;
    _current_vel_setpoint = vel_setpoint;
    _coupler->set(_channel, cv_setpoint, vel_setpoint);
}

void Lane::start(int cv_setpoint, int vel_setpoint)
{
    bool ignore_glide = !_channel && _config->get_glide_channel_switch();
    float glide_intensity;
    if (!ignore_glide)
    {
        glide_intensity = _config->get_glide_intensity();
    }
    if (ignore_glide || glide_intensity == 0 ||
        (!_active && (GLIDE_FLAG_LEGATO & _config->get_glide_flags())))
    {
        set(cv_setpoint, vel_setpoint);
    }
    else
    {
        _glide.active = true;
        _glide.setpoint_start = _current_cv_setpoint;
        _glide.setpoint_end = cv_setpoint;
        _glide.time_start = millis();
        if (_config->is_glide_proportional())
        {
            float gap = setpoint_to_pitch(fabs(_current_cv_setpoint - cv_setpoint)) - MIDI_MIN_PITCH;
            _glide.duration = pow(glide_intensity, GLIDE_INTENSITY_POWER) * GLIDE_MAX_RATE * gap;
        }
        else
        {
            _glide.duration = pow(glide_intensity, GLIDE_INTENSITY_POWER) * GLIDE_MAX_TIME;
        }
        update();
    }
    _coupler->activate(_channel);
    _active = true;
}

void Lane::set_note(Note note, int bend, bool ignore_detune)
{
    if (!_channel && _config->get_bend_channel_switch())
    {
        bend = 0;
    }
    if (_channel || ignore_detune)
    {
        set(pitch_to_voltage(_config, note.pitch, bend), velocity_to_voltage(note.velocity));
    }
    else
    {
        set(pitch_to_voltage(_config, note.pitch + _config->get_detune(), bend), velocity_to_voltage(note.velocity));
    }
}

void Lane::start_note(Note note, int bend, bool ignore_detune)
{
    if (!_channel && _config->get_bend_channel_switch())
    {
        bend = 0;
    }
    if (_channel || ignore_detune)
    {
        start(pitch_to_voltage(_config, note.pitch, bend), velocity_to_voltage(note.velocity));
    }
    else
    {
        start(pitch_to_voltage(_config, note.pitch + _config->get_detune(), bend), velocity_to_voltage(note.velocity));
    }
}

void Lane::stop()
{
    _coupler->deactivate(_channel);
    _active = false;
}

void Lane::update()
{
    if (_glide.active)
    {
        float progress = (float)(millis() - _glide.time_start) / _glide.duration;
        if (progress > 1)
        {
            progress = 1;
        }
        float setpoint = (1 - progress) * _glide.setpoint_start + progress * _glide.setpoint_end;
        if (GLIDE_FLAG_CHROMATIC & _config->get_glide_flags() && progress < 1)
        {
            setpoint = pitch_to_voltage(_config, setpoint_to_pitch(setpoint), 0);
        }
        set((int)setpoint, _current_vel_setpoint);
        if (progress >= 1)
        {
            _glide.active = false;
        }
    }
}

int Lane::pitch_to_voltage(Config *config, byte pitch, int bend)
{
    float bent_pitch = (float)pitch + (float)bend * config->get_pitch_bend_range() / 8192.0;
    float voltage = (bent_pitch - MIDI_MIN_PITCH) / 12.0 / AMP_GAIN + config->get_voltage_offset();
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

int Lane::velocity_to_voltage(byte velocity) {
    return map(velocity, 0, 127, 0, 4096);
}

byte Lane::setpoint_to_pitch(int setpoint)
{
    float voltage = (float)setpoint / 1000.0;
    return (byte)(voltage * AMP_GAIN * 12.0) + MIDI_MIN_PITCH;
}
