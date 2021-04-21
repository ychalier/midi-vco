#ifndef router_h
#define router_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "constants.h"
#include "lane.h"


class Router {
    public:
        Router();
        void setup();
        Lane* select(int lane_id);
        void stop_all();
    private:
        MCP4822* _dac_1;
        MCP4822* _dac_2;
        MCP4822* _dac_3;
        MCP4822* _dac_4;
        Lane* _lane_1;
        Lane* _lane_2;
        Lane* _lane_3;
        Lane* _lane_4;
        Lane* _lane_5;
        Lane* _lane_6;
        Lane* _lane_7;
        Lane* _lane_8;
};

#endif