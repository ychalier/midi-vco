#ifndef note_h
#define note_h

#include "Arduino.h"


typedef struct Note {
    byte channel;
    byte pitch;
} Note;


bool operator==(const Note &l_note, const Note &r_note);

#endif