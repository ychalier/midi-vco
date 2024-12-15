#ifndef structs_h
#define structs_h

#include "Arduino.h"

/**
 * Structure holding a MIDI note.
 */
typedef struct Note
{
    /// MIDI pitch, between 0 and 127 (G9).
    byte pitch;

    /// Velocity attached to the note, between 0 and 127.
    byte velocity;
} Note;

/**
 * Tuning parameters, relative to the DAC setpoint (signed 14 bit integers).
 */
typedef struct Tuning
{
    /// Affine offset. Default is 0.0f (no offset).
    float offset;

    /// Linear scaling (untested). Default is 1.0f (no scaling).
    float scale;
} Tuning;

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