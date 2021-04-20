#include "Arduino.h"
#include "lane.h"


Lane::Lane(MCP4822* dac, bool dac_channel, int gate_pin) {
    _dac = dac;
    _dac_channel = dac_channel;
    _gate_pin = gate_pin;
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


void Lane::play(double voltage, unsigned long duration) {
    start(voltage);
    delay(duration);
    stop();
}


void Lane::start(double voltage) {
    set(voltage);
    digitalWrite(_gate_pin, HIGH);
}


void Lane::set(double voltage) {
    int scaled_voltage = (int) ((double) DAC_BITS * voltage / DAC_RANGE);
    if (_dac_channel) {
        _dac->setVoltageA(scaled_voltage);
    } else {
        _dac->setVoltageB(scaled_voltage);
    }
    _dac->updateDAC();
}


void Lane::stop() {
    digitalWrite(_gate_pin, LOW);
}