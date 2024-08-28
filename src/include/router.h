#ifndef router_h
#define router_h

#include "Arduino.h"
#include "constants.h"
#include "lane.h"
#include "coupler.h"

/**
 * Holder for the different lanes.
 */
class Router
{
public:
    /**
     * Constructor.
     *
     * @param config A pointer to the global config.
     */
    Router(Config *config);

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Update all the lanes (for the glide effect).
     */
    void update();

    /**
     * Getter for the lanes.
     *
     * @param lane_id The lane to select. Indices start at 0.
     * @return A pointer to the selected lane.
     */
    Lane *select(int lane_id);

    void broadcast(byte pitch, int gate);

private:
    Config *_config;
    Coupler *_coupler_1;
    Coupler *_coupler_2;
    Coupler *_coupler_3;
    Coupler *_coupler_4;
    Coupler *_coupler_5;
    Lane *_lane_1;
    Lane *_lane_2;
    Lane *_lane_3;
    Lane *_lane_4;
    Lane *_lane_5;
    Lane *_lane_6;
    Lane *_lane_7;
    Lane *_lane_8;
    Lane *_lane_9;
    Lane *_lane_10;
};

#endif