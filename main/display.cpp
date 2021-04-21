#include "Arduino.h"
#include "display.h"
#include "constants.h"


Display::Display() {
    _start = 0;
    _started = false;
    _duration = 0;
    _target = 0;
}


void Display::setup() {
    pinMode(PIN_LED_1, OUTPUT);
    pinMode(PIN_LED_2, OUTPUT);
    pinMode(PIN_LED_3, OUTPUT);
    pinMode(PIN_LED_4, OUTPUT);
    pinMode(PIN_LED_5, OUTPUT);
    pinMode(PIN_LED_6, OUTPUT);
    pinMode(PIN_LED_7, OUTPUT);
    pinMode(PIN_LED_8, OUTPUT);
    set();
}


void Display::start(byte target, unsigned long duration) {
    _started = true;
    _duration = duration;
    _start = millis();
    _target = target;
}


int Display::get_led_state(byte mask) {
    if ((_target & mask) == 0) {
        return LOW;
    } else {
        return HIGH;
    }
}


void Display::set() {
    digitalWrite(PIN_LED_1, get_led_state(0b00000001));
    digitalWrite(PIN_LED_2, get_led_state(0b00000010));
    digitalWrite(PIN_LED_3, get_led_state(0b00000100));
    digitalWrite(PIN_LED_4, get_led_state(0b00001000));
    digitalWrite(PIN_LED_5, get_led_state(0b00010000));
    digitalWrite(PIN_LED_6, get_led_state(0b00100000));
    digitalWrite(PIN_LED_7, get_led_state(0b01000000));
    digitalWrite(PIN_LED_8, get_led_state(0b10000000));
}


void Display::update() {
    if (_started) {
        unsigned long now = millis();
        if ((now - _start) < _duration) {
            set();
        } else {
            _target = 0;
            set();
            _started = false;
        }
    }
}