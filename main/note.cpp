#include "Arduino.h"
#include "note.h"

bool operator==(const Note &l_note, const Note &r_note)
{
    return l_note.channel == r_note.channel && l_note.pitch == r_note.pitch;
}
