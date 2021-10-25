#include "digital_potentiometer.h"

#include <math.h>

#define SCALE_POW2 1.7188  // 55 / 32
#define LOG2 0.69314718056  // math.log(2)

float get_closest_a440(float frequency)
{
    /**
     * Given a frequency (in Hertz), returns the frequency (in Hertz) of the
     * closest A note, on an equal-tempered scale where A4 is 440 Hz. The
     * frequency distance measure is based on a logarithmic scale.
     */
    return pow(2, round(log(frequency / SCALE_POW2) / LOG2)) * SCALE_POW2;
}

DigitalPotentiometer::DigitalPotentiometer(FrequencyDetector *frequency_detector, int wave_pin, int dac_pin)
{
    _current_value = 128;
    _dac = new MCP41xxx(dac_pin);
    _frequency_detector = frequency_detector;
    _wave_pin = wave_pin;
}

DigitalPotentiometer::setup()
{
    _dac->begin();
    _dac->analogWrite(_current_value);
}

DigitalPotentiometer::tune()
{
    float frequency = _frequency_detector->get(_wave_pin);
    float target = get_closest_a440(frequency);
    float error = abs(frequency - target);
    if (error <= 0.5)
    {
        // pass
    }
    else if (error <= 10)
    {
        if (frequency > target)
        {
            _current_value--;
        }
        else
        {
            _current_value++;
        }
    }
    else
    {
        if (frequency > target)
        {
            _current_value = _current_value - 10;
        }
        else
        {
            _current_value = _current_value + 10;
        }
    }
    if (_current_value < 0)
    {
        _current_value = 0;
    }
    if (_current_value > 255)
    {
        _current_value = 255;
    }
    _dac->analogWrite(_current_value);
    #ifdef DEBUG
    Serial.print(_wave_pin);
    Serial.print(": ");
    Serial.print(frequency);
    Serial.print(" ");
    Serial.print(target);
    Serial.print(" ");
    Serial.println(_current_value);
    #endif
}