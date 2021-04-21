#include "Arduino.h"
#include "lane.h"


Lane::Lane(MCP4822* dac, bool dac_channel, int gate_pin, int led_pin) {
    _dac = dac;
    _dac_channel = dac_channel;
    _gate_pin = gate_pin;
    _led_pin = led_pin;
}


void Lane::setup() {
    if (_dac_channel) {
        _dac->turnOnChannelA();
        _dac->setGainA(MCP4822::High);
    } else {
        _dac->turnOnChannelB();
        _dac->setGainB(MCP4822::High);
    }
    pinMode(_gate_pin, OUTPUT);
    digitalWrite(_gate_pin, LOW);
}


void Lane::play(int setpoint, unsigned long duration) {
    start(setpoint);
    delay(duration);
    stop();
}


void Lane::start(int setpoint) {
    set(setpoint);
    digitalWrite(_gate_pin, HIGH);
    digitalWrite(_led_pin, HIGH);
}


void Lane::set(int setpoint) {
    if (_dac_channel) {
        _dac->setVoltageA(setpoint);
    } else {
        _dac->setVoltageB(setpoint);
    }
    _dac->updateDAC();
}


void Lane::stop() {
    digitalWrite(_gate_pin, LOW);
    digitalWrite(_led_pin, LOW);
}


int Lane::pitch_to_voltage(byte pitch, int bend) {
    double bent_pitch = (double) pitch + (double) bend * PITCH_BEND_RANGE / 8192.0;
    double voltage = (bent_pitch - MIDI_MIN_PITCH) / 12.0 / AMP_GAIN;
    if (voltage < 0) {
        voltage = 0;
    } else if (voltage > DAC_VMAX) {
        voltage = DAC_VMAX;
    }
    return (int) (1000 * voltage);
}
