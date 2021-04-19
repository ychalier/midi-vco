#ifndef router_h
#define router_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "outlane.h"

#define pin_ss_1 47
#define pin_ss_2 45
#define pin_ss_3 43
#define pin_ss_4 41
#define pin_gate_1 37
#define pin_gate_2 35
#define pin_gate_3 33
#define pin_gate_4 31
#define pin_gate_5 29
#define pin_gate_6 27
#define pin_gate_7 25
#define pin_gate_8 23

class Router {
    public:
        Router();
        void setup();
        OutLane* select(int lane_id);
        void play(int lane_id, double voltage, unsigned long duration);
    private:
        MCP4822* _dac_1;
        MCP4822* _dac_2;
        MCP4822* _dac_3;
        MCP4822* _dac_4;
        OutLane* _lane_1;
        OutLane* _lane_2;
        OutLane* _lane_3;
        OutLane* _lane_4;
        OutLane* _lane_5;
        OutLane* _lane_6;
        OutLane* _lane_7;
        OutLane* _lane_8;
};

#endif