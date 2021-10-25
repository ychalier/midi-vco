#include "frequency_detector.h"

FrequencyDetector::FrequencyDetector(int threshold, unsigned long estimation_period)
{
    _threshold = threshold;
    _estimation_period = estimation_period;
}

float FrequencyDetector::get(int wave_in_pin)
{
    unsigned long t_start = micros();
    unsigned long t_prev;
    unsigned long t_cur = t_start;
    int v_prev;
    int v_cur = analogRead(wave_in_pin);
    unsigned long t_first;
    unsigned long t_last;
    bool is_first = true;
    int changes = 0;
    while (true)
    {
        t_prev = t_cur;
        v_prev = v_cur;
        t_cur = micros();
        v_cur = analogRead(wave_in_pin);
        if (t_cur - t_start > _estimation_period)
        {
            break;
        }
        if (v_prev < _threshold && v_cur >= _threshold)
        {
            changes++;
            t_last = (t_cur + t_prev) >> 1;
            if (is_first)
            {
                t_first = t_last;
                is_first = false;
            }
        }
    }
    if (changes <= 1)
    {
        return 0;
    }
    else
    {
        return (float)(changes - 1) * 1000000. / (float)(t_last - t_first);
    }
}
