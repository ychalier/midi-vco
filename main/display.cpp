#include "Arduino.h"
#include "display.h"

#define DEMO_DELAY 50

Display::Display()
{
    _leds[0] = PIN_LED_1;
    _leds[1] = PIN_LED_2;
    _leds[2] = PIN_LED_3;
    _leds[3] = PIN_LED_4;
    _leds[4] = PIN_LED_5;
    _leds[5] = PIN_LED_6;
    _leds[6] = PIN_LED_7;
    _leds[7] = PIN_LED_8;
    _start = 0;
    _locked = false;
    _duration = 0;
    _pattern = 0;
}

void Display::setup()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        pinMode(_leds[i], OUTPUT);
    }
    reset_leds();
}

void Display::reset_leds()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        digitalWrite(_leds[i], LOW);
    }
}

void Display::start(byte pattern, unsigned long duration)
{
    _locked = true;
    _duration = duration;
    _start = millis();
    _pattern = pattern;
    set_pattern();
}

void Display::set_pattern()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        digitalWrite(_leds[i], get_led_state(i));
    }
}

void Display::update()
{
    if (_locked)
    {
        unsigned long now = millis();
        if ((now - _start) >= _duration)
        {
            reset_leds();
            _locked = false;
        }
    }
}

void Display::set_led_state(int led_id, int state)
{
    if (!_locked)
    {
        digitalWrite(_leds[led_id], state);
    }
}

int Display::get_led_state(int led_id)
{
    byte mask = 1 << led_id;
    if ((_pattern & mask) == 0)
    {
        return LOW;
    }
    else
    {
        return HIGH;
    }
}

void Display::demo()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        digitalWrite(_leds[i], HIGH);
        delay(DEMO_DELAY);
        digitalWrite(_leds[i], LOW);
    }
    delay(DEMO_DELAY * 4);
    for (int i = 0; i < LANE_COUNT; i++)
    {
        digitalWrite(_leds[i], HIGH);
    }
    delay(DEMO_DELAY * 4);
    reset_leds();
}