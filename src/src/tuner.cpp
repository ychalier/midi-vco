#include "Arduino.h"
#include "../include/tuner.h"
#include "../include/lane.h"
#include "../include/allocator.h"

Tuner::Tuner()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _tunings[i] = {0.0f, 1.0f};
    }
}

void Tuner::setup()
{
    byte eeprom_first_byte = EEPROM.read(0);
    if (eeprom_first_byte == 255)
    {
        EEPROM.write(0, 0);
        _write_tunings_to_eeprom();
    }
    else
    {
        _read_tunings_from_eeprom();
    }
    pinMode(PIN_CV_LOOPBACK_1, INPUT);
    pinMode(PIN_CV_LOOPBACK_2, INPUT);
    pinMode(PIN_CV_LOOPBACK_3, INPUT);
    pinMode(PIN_CV_LOOPBACK_4, INPUT);
    pinMode(PIN_CV_LOOPBACK_5, INPUT);
    pinMode(PIN_CV_LOOPBACK_6, INPUT);
    pinMode(PIN_CV_LOOPBACK_7, INPUT);
    pinMode(PIN_CV_LOOPBACK_8, INPUT);
    pinMode(PIN_CV_LOOPBACK_9, INPUT);
    pinMode(PIN_CV_LOOPBACK_10, INPUT);
    pinMode(PIN_LED_TUNING_FAST, OUTPUT);
    pinMode(PIN_LED_TUNING_FULL, OUTPUT);
    pinMode(PIN_LED_TUNING_RESET, OUTPUT);
}

Tuning* Tuner::get_tuning_ref(int lane_id)
{
    return &_tunings[lane_id];
}

void Tuner::reset_tunings()
{
    digitalWrite(PIN_LED_TUNING_RESET, HIGH);
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _tunings[i] = {0.0f, 1.0f};
    }
    _write_tunings_to_eeprom();
    digitalWrite(PIN_LED_TUNING_RESET, LOW);
}

void Tuner::_read_tunings_from_eeprom()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        int address = 1 + sizeof(Tuning) * i;
        Tuning tuning;
        EEPROM.get(address, tuning);

    }
}

void Tuner::_write_tunings_to_eeprom()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        int address = 1 + sizeof(Tuning) * i;
        EEPROM.put(address, _tunings[i]);
    }
}

void estimate_frequencies(float *frequencies)
{   
    int loopback_pins[LANE_COUNT] = {
        PIN_CV_LOOPBACK_1,
        PIN_CV_LOOPBACK_2,
        PIN_CV_LOOPBACK_3,
        PIN_CV_LOOPBACK_4,
        PIN_CV_LOOPBACK_5,
        PIN_CV_LOOPBACK_6,
        PIN_CV_LOOPBACK_7,
        PIN_CV_LOOPBACK_8,
        PIN_CV_LOOPBACK_9,
        PIN_CV_LOOPBACK_10
    };
    int v_prev[LANE_COUNT];
    int v_cur[LANE_COUNT];
    unsigned long t_first[LANE_COUNT];
    unsigned long t_last[LANE_COUNT];
    bool is_first[LANE_COUNT];
    int changes[LANE_COUNT];
    for (int i = 0; i < LANE_COUNT; i++)
    {
        v_prev[i] = 0;
        v_cur[i] = analogRead(loopback_pins[i]);
        is_first[i] = true;
        changes[i] = 0;
    }
    unsigned long t_prev;
    unsigned long t_start = micros();
    unsigned long t_cur = t_start;
    while (true)
    {
        t_prev = t_cur;
        t_cur = micros();
        if (t_cur - t_start > FREQUENCY_ESTIMATION_PERIOD)
        {
            break;
        }
        for (int i = 0; i < LANE_COUNT; i++)
        {
            v_prev[i] = v_cur[i];
            v_cur[i] = analogRead(loopback_pins[i]);
            if (v_prev[i] < FREQUENCY_THRESHOLD && v_cur[i] >= FREQUENCY_THRESHOLD)
            {
                changes[i]++;
                t_last[i] = (t_cur + t_prev) >> 1;
                if (is_first[i])
                {
                    t_first[i] = t_last[i];
                    is_first[i] = false;
                }
            }
        }
    }
    for (int i = 0; i < LANE_COUNT; i++)
    {
        if (changes[i] <= 1)
        {
            frequencies[i] = 0.0f;
        }
        else
        {
            frequencies[i] = (float)(changes[i] - 1) * 1000000.0f / (float)(t_last[i] - t_first[i]);
        }
    }
}

void Tuner::tune_fast(Allocator *allocator)
{
    digitalWrite(PIN_LED_TUNING_FAST, HIGH);
    allocator->reset();
    int tuning_setpoint = (int)Lane::base_pitch_to_voltage((float)TUNING_OFFSET_PITCH);
    if (tuning_setpoint < 0)
    {
        tuning_setpoint = 0;
    }
    else if (tuning_setpoint > DAC_VMAX)
    {
        tuning_setpoint = DAC_VMAX;
    }
    allocator->broadcast(tuning_setpoint, GATE_STATE_DURING_TUNING);
    float estimated_frequencies[LANE_COUNT];
    estimate_frequencies(estimated_frequencies);
    for (int i = 0; i < LANE_COUNT; i++)
    {
        float estimated_pitch = 12.0f * log(estimated_frequencies[i] / TUNING_REFERENCE_FREQUENCY) / LOG2 + 69.0f;
        float estimated_setpoint = Lane::base_pitch_to_voltage(estimated_pitch);
        _tunings[i].offset = (float)tuning_setpoint - estimated_setpoint;
    }
    _write_tunings_to_eeprom();
    allocator->reset();
    digitalWrite(PIN_LED_TUNING_FAST, LOW);
}