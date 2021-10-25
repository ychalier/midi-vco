#ifndef frequency_detector_h
#define frequency_detector_h

#include "Arduino.h"
#include "constants.h"

class FrequencyDetector
{
public:
    FrequencyDetector(int threshold, unsigned long estimation_period);
    float get(int wave_in_pin);

private:
    int _threshold;
    unsigned long _estimation_period;
};

#endif