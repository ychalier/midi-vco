#ifndef digital_potentiometer_h
#define digital_potentiometer_h

#include "Arduino.h"
#include <MCP41xxx.h>
#include "frequency_detector.h"
#include "constants.h"

class DigitalPotentiometer
{
public:
    DigitalPotentiometer(FrequencyDetector *frequency_detector, int wave_pin, int dac_pin);
    setup();
    tune();
private:
    FrequencyDetector *_frequency_detector;
    int _wave_pin;
    int _current_value;
    MCP41xxx *_dac;
};

#endif