#ifndef note_h
#define note_h

#include "Arduino.h"

/**
 * Simple data structure holding a MIDI note.
 */
typedef struct Note
{
    /// MIDI channel (from 1 to 16)
    byte channel;

    /// Note pitch in semitons
    byte pitch;
} Note;

/**
 * Overloading equality check to compare two notes.
 */
bool operator==(const Note &l_note, const Note &r_note);

#endif