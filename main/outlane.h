#ifndef outlane_h
#define outlane_h

#include "Arduino.h"
#include <MCP48xx.h>

class OutLane {
    public:
        OutLane(MCP4822* dac, bool dac_channel, int gate_pin);
        void setup();
        void play(double voltage, unsigned long duration);  // voltage in volts, duration in milliseconds
    private:
        MCP4822* _dac;
        bool _dac_channel;
        int _gate_pin;
};

#endif