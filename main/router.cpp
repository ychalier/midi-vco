#include "Arduino.h"
#include "router.h"


Router::Router() {
    _dac_1 = new MCP4822(PIN_SS_1);
    _dac_2 = new MCP4822(PIN_SS_2);
    _dac_3 = new MCP4822(PIN_SS_3);
    _dac_4 = new MCP4822(PIN_SS_4);
    _lane_1 = new OutLane(_dac_1, true, PIN_GATE_1);
    _lane_2 = new OutLane(_dac_1, false, PIN_GATE_2);
    _lane_3 = new OutLane(_dac_2, true, PIN_GATE_3);
    _lane_4 = new OutLane(_dac_2, false, PIN_GATE_4);
    _lane_5 = new OutLane(_dac_3, true, PIN_GATE_5);
    _lane_6 = new OutLane(_dac_3, false, PIN_GATE_6);
    _lane_7 = new OutLane(_dac_4, true, PIN_GATE_7);
    _lane_8 = new OutLane(_dac_4, false, PIN_GATE_8);
}


void Router::setup() {
    _dac_1->init();
    _dac_2->init();
    _dac_3->init();
    _dac_4->init();
    _lane_1->setup();
    _lane_2->setup();
    _lane_3->setup();
    _lane_4->setup();
    _lane_5->setup();
    _lane_6->setup();
    _lane_7->setup();
    _lane_8->setup();
}


OutLane* Router::select(int lane_id) {
    switch (lane_id) {
    case 1:
        return _lane_1;
        break;
    case 2:
        return _lane_2;
        break;
    case 3:
        return _lane_3;
        break;
    case 4:
        return _lane_4;
        break;
    case 5:
        return _lane_5;
        break;
    case 6:
        return _lane_6;
        break;
    case 7:
        return _lane_7;
        break;
    case 8:
        return _lane_8;
        break;
    default:
        break;
    }
}