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
    void record_note_on(Note note);
    void record_note_off(Note note);
    void start_recording();
    void stop_recording();
    void start_playback();
    void stop_playback();
    void update_playback();
    bool is_recording();
    bool is_playing();

private:
    Config *_config;
    Allocator *_allocator;
    bool _recording;
    bool _playing;
    int _size;
    MidiEvent _memory[SEQUENCER_MEMORY_SIZE];
    unsigned long _record_timestamp;
    unsigned long _playback_timestamp;
    int _playback_index;
    void record_event(MidiEvent event);
};

#endif