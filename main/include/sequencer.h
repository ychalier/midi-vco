#ifndef sequencer_h
#define sequencer_h

#include "Arduino.h"
#include "constants.h"
#include "structs.h"
#include "allocator.h"
#include "config.h"

class Sequencer
{
public:
    Sequencer(Config *config, Allocator *allocator);
    void note_on(Note note);
    void note_off(Note note);
    void update_state(bool recording);
    void update();

private:
    Config *_config;
    Allocator *_allocator;
    bool _recording;
    int _size;
    MidiEvent _memory[SEQUENCER_MEMORY_SIZE];
    unsigned long _record_timestamp;
    unsigned long _playback_timestamp;
    int _playback_index;
    void record_event(MidiEvent event);
};

#endif