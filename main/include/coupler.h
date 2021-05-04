#ifndef coupler_h
#define coupler_h

#include "Arduino.h"
#include <MCP48xx.h>

#define COUPLER_A true
#define COUPLER_B false

class Coupler
{
    public:
        Coupler(int dac_pin, int gate_pin);
        void setup();
        void set(bool channel, int setpoint);
        void activate(bool channel);
        void deactivate(bool channel);
        void update();

    private:
        MCP4822 *_dac;
        bool _active_a;
        bool _active_b;
        int _setpoint_a;
        int _setpoint_b;
        int _cv_a;
        int _cv_b;
        int _gate;
        int _gate_pin;
};

#endif