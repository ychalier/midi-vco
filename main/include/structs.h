#ifndef structs_h
#define structs_h

#include "Arduino.h"


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

/**
 * Data representation of a sequencer event, either a note-on or a note-off
 * message.
 */
typedef struct MidiEvent
{
    int division;
    bool type;
    byte pitch;
    byte mask;
} MidiEvent;

#endif