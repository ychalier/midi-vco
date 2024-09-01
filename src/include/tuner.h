#ifndef tuner_h
#define tuner_h

#include "Arduino.h"
#include "constants.h"
#include <EEPROM.h>
#include "structs.h"

class Allocator; // Prevent circular dependencies

/**
 * A wrapper for storing and computing tuning parameters.
 */
class Tuner
{
public:
    Tuner();

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase. If this is the first time the board boots, it writes the
     * EEPROM with default values. Otherwise, reads previous tuning parameters
     * from the EEPROM.
     */
    void setup();

    /**
     * Getter for tuning parameters, used by `Lane` objects.
     * 
     * @param lane_id Lane id
     * @return A reference to the tuning parameters
     */
    Tuning *get_tuning_ref(int lane_id);
    
    /**
     * Reset all tuning parameters to default (no offset, no scaling).
     * Write the changes to EEPROM.
     */
    void reset_tunings();

    /**
     * Automatically compute the offset tuning parameters, for all lanes.
     * 
     * @param allocator A pointer to the allocator for broadcasting a signal.
     */
    void tune_fast(Allocator *allocator);

private:
    Tuning _tunings[LANE_COUNT];

    void _read_tunings_from_eeprom();

    void _write_tunings_to_eeprom();
};

#endif