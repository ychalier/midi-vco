#ifndef lane_h
#define lane_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "constants.h"

class Lane {
    public:
        Lane(MCP4822* dac, bool dac_channel, int gate_pin);
        void setup();
        void play(int setpoint, unsigned long duration);
        void set(int setpoint);
        void start(int setpoint);
        void stop();
        static int pitch_to_voltage(byte pitch, int bend);
    private:
        MCP4822* _dac;
        bool _dac_channel;
        int _gate_pin;
};

#endif