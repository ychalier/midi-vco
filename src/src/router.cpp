#include "Arduino.h"
#include "../include/router.h"

Router::Router(Config *config)
{
    _config = config;
    _dac_1 = new Dac(PIN_SS_VEL_1);
    _dac_2 = new Dac(PIN_SS_VEL_2);
    _dac_3 = new Dac(PIN_SS_VEL_3);
    _coupler_1 = new Coupler(PIN_SS_CV_1, PIN_GATE_1, _dac_1->get(CHANNEL_A));
    _coupler_2 = new Coupler(PIN_SS_CV_2, PIN_GATE_2, _dac_1->get(CHANNEL_B));
    _coupler_3 = new Coupler(PIN_SS_CV_3, PIN_GATE_3, _dac_2->get(CHANNEL_A));
    _coupler_4 = new Coupler(PIN_SS_CV_4, PIN_GATE_4, _dac_2->get(CHANNEL_B));
    _coupler_5 = new Coupler(PIN_SS_CV_5, PIN_GATE_5, _dac_3->get(CHANNEL_A));
    _lane_1  = new Lane(config, _coupler_1, CHANNEL_A);
    _lane_2  = new Lane(config, _coupler_1, CHANNEL_B);
    _lane_3  = new Lane(config, _coupler_2, CHANNEL_A);
    _lane_4  = new Lane(config, _coupler_2, CHANNEL_B);
    _lane_5  = new Lane(config, _coupler_3, CHANNEL_A);
    _lane_6  = new Lane(config, _coupler_3, CHANNEL_B);
    _lane_7  = new Lane(config, _coupler_4, CHANNEL_A);
    _lane_8  = new Lane(config, _coupler_4, CHANNEL_B);
    _lane_9  = new Lane(config, _coupler_5, CHANNEL_A);
    _lane_10 = new Lane(config, _coupler_5, CHANNEL_B);
}

void Router::setup()
{
    _dac_1->setup();
    _dac_2->setup();
    _dac_3->setup();
    _coupler_1->setup();
    _coupler_2->setup();
    _coupler_3->setup();
    _coupler_4->setup();
    _coupler_5->setup();
    _lane_1->setup();
    _lane_2->setup();
    _lane_3->setup();
    _lane_4->setup();
    _lane_5->setup();
    _lane_6->setup();
    _lane_7->setup();
    _lane_8->setup();
    _lane_9->setup();
    _lane_10->setup();
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
    _lane_9->update();
    _lane_10->update();
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
    case 8:
        return _lane_9;
        break;
    case 9:
        return _lane_10;
        break;
    default:
        break;
    }
    return _lane_1;
}

void Router::broadcast(byte pitch, int gate)
{
    int setpoint = Lane::pitch_to_voltage(_config, pitch, 0);
    _coupler_1->broadcast(setpoint, gate);
    _coupler_2->broadcast(setpoint, gate);
    _coupler_3->broadcast(setpoint, gate);
    _coupler_4->broadcast(setpoint, gate);
    _coupler_5->broadcast(setpoint, gate);
}

Channel* Router::get_spare_channel() {
    return _dac_3->get(CHANNEL_B);
}