#ifndef display_h
#define display_h

#include "Arduino.h"


class Display {

    public:
        Display();
        void setup();
        void start(byte target, unsigned long duration);
        void set();
        void update();
        int get_led_state(byte mask);
    private:
        bool _started;
        byte _target;
        unsigned long _start;
        unsigned long _duration;

};


#endif