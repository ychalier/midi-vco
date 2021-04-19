#ifndef outlane_h
#define outlane_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "constants.h"

class OutLane {
    public:
        OutLane(MCP4822* dac, bool dac_channel, int gate_pin);
        void setup();
        void play(double voltage, unsigned long duration);
        void set(double voltage);
        void start(double voltage);
        void stop();
    private:
        MCP4822* _dac;
        bool _dac_channel;
        int _gate_pin;
};

#endif