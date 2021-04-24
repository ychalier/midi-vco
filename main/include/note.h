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

/**
 * Simple data structure holding information for a note glide.
 */
typedef struct Glide
{
    /// Whether the glide is currently active.
    bool active;

    /// Startpoint, as a DAC input value.
    int setpoint_start;

    /// Endpoint, as a DAC input value.
    int setpoint_end;

    /// Timestamp of when the glide started.
    unsigned long time_start;

    /// Target duration for the glide.
    unsigned long duration;
} Glide;

#endif