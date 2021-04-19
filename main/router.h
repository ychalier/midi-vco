#ifndef router_h
#define router_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "constants.h"
#include "outlane.h"


class Router {
    public:
        Router();
        void setup();
        OutLane* select(int lane_id);
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