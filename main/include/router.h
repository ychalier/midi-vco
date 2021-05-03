#ifndef router_h
#define router_h

#include "Arduino.h"
#include <MCP48xx.h>
#include "constants.h"
#include "lane.h"
#include "display.h"

/**
 * Holder for the different lanes.
 */
class Router
{
public:
    /**
     * Constructor.
     * 
     * @param display A pointer to the LED display wrapper.
     */
    Router(Config *config, Display *display);

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

private:
    MCP4822 *_dac_1;
    MCP4822 *_dac_2;
    MCP4822 *_dac_3;
    MCP4822 *_dac_4;
    Lane *_lane_1;
    Lane *_lane_2;
    Lane *_lane_3;
    Lane *_lane_4;
    Lane *_lane_5;
    Lane *_lane_6;
    Lane *_lane_7;
    Lane *_lane_8;
};

#endif