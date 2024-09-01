#ifndef router_h
#define router_h

#include "Arduino.h"
#include "constants.h"
#include "config.h"
#include "coupler.h"
#include "dac.h"
#include "lane.h"
#include <MCP48xx.h>
#include "tuner.h"

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
     * @param tuner A pointer to the tuner, holding all tuning parameters.
     */
    Router(Config *config, Tuner *tuner);

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

    /**
     * Artificially send an output to all lanes. This is used during tuning.
     * 
     * @param setpoint CV setpoint to broadcast to all lanes
     * @param gate Gate state to broadcast to all couplers
     */
    void broadcast(int setpoint, int gate);

    /**
     * Getter for the spare DAC channel. There are 5 velocity outputs and 6 DAC
     * channels available. Returns the unused one, for outputting MOD. 
     */
    Channel *get_spare_channel();

private:
    Config *_config;
    Dac *_dac_1;
    Dac *_dac_2;
    Dac *_dac_3;
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