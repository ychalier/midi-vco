#include "Arduino.h"
#include "../include/tuner.h"
#include "../include/lane.h"
#include "../include/allocator.h" // Prevent circular dependencies

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
        // TODO: uncomment this!
        // int address = 1 + sizeof(Tuning) * i;
        // Tuning tuning;
        // EEPROM.get(address, tuning);
        // _tunings[i] = {tuning.offset, tuning.scale, tuning.bonusOffset};
        _tunings[i] = {0.0f, 1.0f, 0.0f};
    }
}

void Tuner::_write_tunings_to_eeprom()
{
    // TODO: uncomment this!
    // for (int i = 0; i < LANE_COUNT; i++)
    // {
    //     int address = 1 + sizeof(Tuning) * i;
    //     EEPROM.put(address, _tunings[i]);
    // }
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
    for (int i = 0; i < LANE_COUNT; i++)
    {
        t_start = micros();
        t_cur = t_start;
        while (true)
        {
            t_prev = t_cur;
            t_cur = micros();
            if (t_cur - t_start > TUNING_FAST_PERIOD)
            {
                break;
            }
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
        #ifdef DEBUG
        Serial.print("Lane ");
        Serial.print(i);
        Serial.println(" detected ");
        Serial.print(changes[i]);
        Serial.println(" changes");
        #endif
    }
    for (int i = 0; i < LANE_COUNT; i++)
    {
        if (changes[i] <= 1)
        {
            frequencies[i] = 0.0f;
            #ifdef DEBUG
            Serial.print("Lane ");
            Serial.print(i);
            Serial.println(": no change detected!");
            #endif
        }
        else
        {
            frequencies[i] = (float)(changes[i] - 1) * 1000000.0f / (float)(t_last[i] - t_first[i]);
            #ifdef DEBUG
            Serial.print("Lane ");
            Serial.print(i);
            Serial.print(": ");
            Serial.print(frequencies[i]);
            Serial.println(" Hz");
            #endif
        }
    }
}

void run_estimations(Allocator *allocator, int pitch, int &expected_setpoint, float *measured_setpoints)
{
    int tuning_setpoint = (int)Lane::base_pitch_to_voltage((float)pitch);
    expected_setpoint = (int)Lane::base_pitch_to_voltage((float)pitch - 12);
    if (tuning_setpoint < 0)
    {
        tuning_setpoint = 0;
    }
    else if (tuning_setpoint > DAC_VMAX)
    {
        tuning_setpoint = DAC_VMAX;
    }
    #ifdef DEBUG
    Serial.print("Tuning pitch: ");
    Serial.print(pitch);
    Serial.print(" Setpoint: ");
    Serial.print(tuning_setpoint);
    Serial.print(" Expected: ");
    Serial.println(expected_setpoint);
    #endif
    allocator->reset();
    allocator->broadcast(tuning_setpoint, GATE_STATE_DURING_TUNING);
    float estimated_frequencies[LANE_COUNT];
    estimate_frequencies(estimated_frequencies);
    for (int i = 0; i < LANE_COUNT; i++)
    {
        float estimated_pitch = 12.0f * log(estimated_frequencies[i] / TUNING_REFERENCE_FREQUENCY) / LOG2 + 69.0f;
        measured_setpoints[i] = Lane::base_pitch_to_voltage(estimated_pitch);
        #ifdef DEBUG
        Serial.print("Lane: ");
        Serial.print(i);
        Serial.print(" Expected: ");
        Serial.print(pitch - 12);
        Serial.print("/");
        Serial.print(expected_setpoint);
        Serial.print(" Measured:");
        Serial.print(estimated_pitch);
        Serial.print("/");
        Serial.println(measured_setpoints[i]);
        #endif
    }
    allocator->reset();
}

#ifdef DEBUG
void print_tuning(int lane, float scale, float offset, float bonusOffset)
{
    Serial.print("Lane: ");
    Serial.print(lane);
    Serial.print(" Scale: ");
    Serial.print(scale);
    Serial.print(" Offset: ");
    Serial.print(offset);
    Serial.print(" Bonus Offset: ");
    Serial.println(bonusOffset);
}
#endif

void Tuner::tune_fast(Allocator *allocator)
{
    #ifdef DEBUG
    Serial.println("Entering Tuner::tune_fast");
    #endif
    digitalWrite(PIN_LED_TUNING_FAST, HIGH);
    int expected_setpoint;
    float measured_setpoints[LANE_COUNT];
    run_estimations(allocator, TUNING_FAST_PITCH, expected_setpoint, measured_setpoints);
    for (int i = 0; i < LANE_COUNT; i++)
    {
        // _tunings[i].scale = 1.0f;
        // _tunings[i].offset = 0.0f;
        _tunings[i].bonusOffset = (float)expected_setpoint - measured_setpoints[i];
        #ifdef DEBUG
        print_tuning(i, _tunings[i].scale, _tunings[i].offset, _tunings[i].bonusOffset);
        #endif
        
    }
    _write_tunings_to_eeprom();
    digitalWrite(PIN_LED_TUNING_FAST, LOW);
    #ifdef DEBUG
    Serial.println("Leaving Tuner::tune_fast");
    #endif
}

void Tuner::tune_full(Allocator *allocator)
{
    #ifdef DEBUG
    Serial.println("Entering Tuner::tune_full");
    #endif
    digitalWrite(PIN_LED_TUNING_FULL, HIGH);
    int expected_setpoint_low;
    float measured_setpoints_low[LANE_COUNT];
    run_estimations(allocator, TUNING_FULL_PITCH_LOW, expected_setpoint_low, measured_setpoints_low);
    int expected_setpoint_high;
    float measured_setpoints_high[LANE_COUNT];
    run_estimations(allocator, TUNING_FULL_PITCH_HIGH, expected_setpoint_high, measured_setpoints_high);
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _tunings[i].scale = ((float)expected_setpoint_high - (float)expected_setpoint_low) / (measured_setpoints_high[i] - measured_setpoints_low[i]);
        _tunings[i].offset = (float)expected_setpoint_high - _tunings[i].scale * measured_setpoints_high[i];
        _tunings[i].bonusOffset = 0.0f;
        #ifdef DEBUG
        print_tuning(i, _tunings[i].scale, _tunings[i].offset, , _tunings[i].bonusOffset);
        #endif
    }
    _write_tunings_to_eeprom();
    digitalWrite(PIN_LED_TUNING_FULL, LOW);
    tune_fast(allocator);
    #ifdef DEBUG
    Serial.println("Leaving Tuner::tune_full");
    #endif
}