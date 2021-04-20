#ifndef config_h
#define config_h

#include "Arduino.h"

class Config {
    public:
        Config();
        byte get_polyphony_mode();
        byte get_priority_mode();
    private:
        byte _polyphony_mode;
        byte _priority_mode;
        byte _midi_channel_mode;
};

#endif