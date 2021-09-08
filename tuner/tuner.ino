/**
 * TUNER
 * Arduino firmware providing an automated tuning feature for a VCO.
 * 
 * @author Yohan Chalier
 * @version 0.1.0 2021-09-07
 */

#define MODE_SELECTOR_PIN_A A1
#define MODE_SELECTOR_PIN_B A2
#define WAVE_IN_PIN A0

#define MODE_OFF 0
#define MODE_TUNE 1
#define MODE_SCALE 2

#define SAMPLE_SIZE 128
#define SAMPLE_FREQUENCY 1024 // Hz

byte current_mode;
byte samples[SAMPLE_SIZE];
const double sampling_period = round(1000000 * (1.0 / SAMPLE_FREQUENCY));

void setup()
{
    pinMode(MODE_SELECTOR_PIN_A, INPUT);
    pinMode(MODE_SELECTOR_PIN_B, INPUT);
    current_mode = MODE_OFF;
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