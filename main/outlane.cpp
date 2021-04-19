// https://www.arduino.cc/en/Hacking/libraryTutorial
// https://www.codecademy.com/learn/learn-c-plus-plus/modules/learn-cpp-references-and-pointers/cheatsheet


#include "Arduino.h"
#include "outlane.h"


OutLane::OutLane(MCP4822* dac, bool dac_channel, int gate_pin) {
    _dac = dac;
    _dac_channel = dac_channel;
    _gate_pin = gate_pin;
}


void OutLane::setup() {
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


void OutLane::play(double voltage, unsigned long duration) {
    int scaled_voltage = (int) 4096.0 * voltage / 7.54;
    if (_dac_channel) {
        _dac->setVoltageA(scaled_voltage);
    } else {
        _dac->setVoltageB(scaled_voltage);
    }
    _dac->updateDAC();
    digitalWrite(_gate_pin, HIGH);
    delay(duration);
    digitalWrite(_gate_pin, LOW);
}