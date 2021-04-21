#ifndef allocator_h
#define allocator_h

#include "Arduino.h"
#include "config.h"
#include "note.h"
#include "pool.h"
#include "router.h"
#include "display.h"


class Allocator {

    public:
        Allocator(Config* config);
        void note_on(Note note);
        void note_off(Note note);
        void pitch_bend(byte channel, int bend);
        void setup();
        void set_masks();
        void start_display(Display* display);
    private:
        Config* _config;
        Pool* _pools[POOL_COUNT];
        Router* _router;

};

#endif