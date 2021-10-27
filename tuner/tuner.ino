/**
 * TUNER
 * Arduino firmware providing an automated tuning feature for a VCO.
 * 
 * @author Yohan Chalier
 * @version 0.2.0 2021-10-27
 */

// Arduino Nano is based on the ATmega328 microcontroller.
// The EEPROM has a size of 2 Ko.
// @see https://docs.rs-online.com/5775/0900766b80db4deb.pdf
// #include <EEPROM.h>

#include "constants.h"
#include "frequency_detector.h"
#include "digital_potentiometer.h"

#define MODE_SELECTOR_PIN A1
#define DAC_TUNE_PIN_A 4
#define DAC_TUNE_PIN_B 3
#define WAVE_IN_A A5
#define WAVE_IN_B A4

#define MODE_OFF 0
#define MODE_TUNE 1

FrequencyDetector* frequency_detector = new FrequencyDetector(614, 1000000);
DigitalPotentiometer* dgt_pot_a = new DigitalPotentiometer(frequency_detector, WAVE_IN_A, DAC_TUNE_PIN_A);
DigitalPotentiometer* dgt_pot_b = new DigitalPotentiometer(frequency_detector, WAVE_IN_B, DAC_TUNE_PIN_B);

byte current_mode = MODE_OFF;

void setup()
{
    pinMode(MODE_SELECTOR_PIN, INPUT);
    pinMode(DAC_TUNE_PIN_A, INPUT);
    pinMode(DAC_TUNE_PIN_B, INPUT);
    dgt_pot_a->setup();
    dgt_pot_b->setup();
    #ifdef DEBUG
    Serial.begin(9600);
    #endif
}

void loop()
{
    if (get_current_mode() == MODE_TUNE)
    {
        #ifdef DEBUG
        Serial.println("TUNE");
        #endif
        dgt_pot_a->tune();
        dgt_pot_b->tune();
    }
}

byte get_current_mode()
{
    if (digitalRead(MODE_SELECTOR_PIN) == HIGH)
    {
        return MODE_TUNE;
    }
    else
    {
        return MODE_OFF;
    }
}
