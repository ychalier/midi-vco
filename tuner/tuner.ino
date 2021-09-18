/**
 * TUNER
 * Arduino firmware providing an automated tuning feature for a VCO.
 * 
 * @author Yohan Chalier
 * @version 0.1.0 2021-09-17
 */

#include <MCP41xxx.h>

#define MODE_SELECTOR_PIN_A A1
#define MODE_SELECTOR_PIN_B A2
#define WAVE_IN_PIN A0
#define DAC_TUNE_PIN 9
#define DAC_SCALE_PIN 10

#define MODE_OFF 0
#define MODE_TUNE 1
#define MODE_SCALE 2

#define SAMPLE_SIZE 512       // Arduino Nano has 2 Ko of RAM
#define SAMPLE_FREQUENCY 4096 // Hz

typedef struct DigitalPotentiometerState
{
    byte current_value;
    byte lbound;
    byte ubound;
} DigitalPotentiometerState;

byte current_mode;
int samples[SAMPLE_SIZE];
const double sampling_period = round(1000000 * (1.0 / SAMPLE_FREQUENCY));
MCP41xxx dac_tune(DAC_TUNE_PIN);
MCP41xxx dac_scale(DAC_SCALE_PIN);

DigitalPotentiometerState state_tune{127, 0, 255};
DigitalPotentiometerState state_scale{127, 0, 255};

void setup()
{
    pinMode(MODE_SELECTOR_PIN_A, INPUT);
    pinMode(MODE_SELECTOR_PIN_B, INPUT);
    current_mode = MODE_OFF;
    dac_tune.begin();
    dac_scale.begin();
    // TODO: read digital potentiometer current values from EEPROM
    dac_tune.analogWrite(state_tune.value);
    dac_scale.analogWrite(state_scale.value);
}

void loop()
{
    byte new_mode = get_current_mode();
    if (new_mode != current_mode)
    {
        // TODO: reset tuning variables here
        current_mode = new_mode;
    }
    switch (current_mode)
    {
    case MODE_TUNE:
        exec_mode_tune();
        break;
    case MODE_SCALE:
        exec_mode_scale();
        break;
    }
}

byte get_current_mode()
{
    int a = digitalRead(MODE_SELECTOR_PIN_A);
    int b = digitalRead(MODE_SELECTOR_PIN_B);
    if (a == LOW && b == LOW)
    {
        return MODE_OFF;
    }
    else if (a == HIGH && b == LOW)
    {
        return MODE_TUNE;
    }
    else if (a == LOW && b == HIGH)
    {
        return MODE_SCALE;
    }
    else
    {
        // In real life, this case should never occur.
        // Yet, better safe than sorry!
        return MODE_OFF;
    }
}

void exec_mode_tune()
{
    // TODO
}

void exec_mode_scale()
{
    // TODO
}

void acquire_samples()
{
    unsigned long now;
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        now = micros();
        samples[i] = analogRead(WAVE_IN_PIN);
        while (micros() < (now + sampling_period))
        {
            // pass
        }
    }
}

int count_crossings_rising(int frontier)
{
    int crossings = 0;
    for (int i = 0; i < SAMPLE_SIZE - 1; i++)
    {
        if (samples[i] <= frontier && samples[i + 1] >= frontier)
        {
            crossings++;
        }
    }
    return crossings;
}

float compute_frequency()
{
    int min = 1024;
    int max = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        if (samples[i] > max)
        {
            max = samples[i];
        }
        if (samples[i] < min)
        {
            min = samples[i];
        }
    }
    int crossings = count_crossings_rising((max + min) / 2);
    return (float)crossings / (float)(sampling_period * SAMPLE_SIZE);
}