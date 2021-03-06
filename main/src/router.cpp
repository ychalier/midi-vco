#include "Arduino.h"
#include "../include/router.h"

Router::Router(Config *config, Display *display)
{
    _coupler_1 = new Coupler(PIN_SS_1, PIN_GATE_1);
    _coupler_2 = new Coupler(PIN_SS_2, PIN_GATE_3);
    _coupler_3 = new Coupler(PIN_SS_3, PIN_GATE_5);
    _coupler_4 = new Coupler(PIN_SS_4, PIN_GATE_7);
    _lane_1 = new Lane(config, display, _coupler_1, COUPLER_A, 0);
    _lane_2 = new Lane(config, display, _coupler_1, COUPLER_B, 1);
    _lane_3 = new Lane(config, display, _coupler_2, COUPLER_A, 2);
    _lane_4 = new Lane(config, display, _coupler_2, COUPLER_B, 3);
    _lane_5 = new Lane(config, display, _coupler_3, COUPLER_A, 4);
    _lane_6 = new Lane(config, display, _coupler_3, COUPLER_B, 5);
    _lane_7 = new Lane(config, display, _coupler_4, COUPLER_A, 6);
    _lane_8 = new Lane(config, display, _coupler_4, COUPLER_B, 7);
}

void Router::setup()
{
    _coupler_1->setup();
    _coupler_2->setup();
    _coupler_3->setup();
    _coupler_4->setup();
    _lane_1->setup();
    _lane_2->setup();
    _lane_3->setup();
    _lane_4->setup();
    _lane_5->setup();
    _lane_6->setup();
    _lane_7->setup();
    _lane_8->setup();
}

void Router::update()
{
    _lane_1->update();
    _lane_2->update();
    _lane_3->update();
    _lane_4->update();
    _lane_5->update();
    _lane_6->update();
    _lane_7->update();
    _lane_8->update();
}

Lane *Router::select(int lane_id)
{
    switch (lane_id)
    {
    case 0:
        return _lane_1;
        break;
    case 1:
        return _lane_2;
        break;
    case 2:
        return _lane_3;
        break;
    case 3:
        return _lane_4;
        break;
    case 4:
        return _lane_5;
        break;
    case 5:
        return _lane_6;
        break;
    case 6:
        return _lane_7;
        break;
    case 7:
        return _lane_8;
        break;
    default:
        break;
    }
}
