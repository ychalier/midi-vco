#ifndef tuner_h
#define tuner_h

#include "Arduino.h"
#include <EEPROM.h>
#include "constants.h"
#include "structs.h"

class Allocator;

class Tuner
{
public:
    Tuner();

    void setup();

    Tuning *get_tuning_ref(int lane_id);

    void reset_tunings();

    void tune_fast(Allocator *allocator);

private:
    Tuning _tunings[LANE_COUNT];

    void _read_tunings_from_eeprom();

    void _write_tunings_to_eeprom();
};

#endif