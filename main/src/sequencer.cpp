#include "Arduino.h"
#include "../include/sequencer.h"

Sequencer::Sequencer(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    _size = 0;
    _recording = false;
    _record_timestamp = 0;
    _playback_timestamp = 0;
    _playback_index = 0;
}

void Sequencer::record_event(MidiEvent event)
{
    if (_size < SEQUENCER_MEMORY_SIZE)
    {
        _memory[_size] = event;
        _size++;
    }
}

void Sequencer::note_on(Note note)
{
    if (_recording)
    {
        unsigned long now = millis();
        if (_size == 0)
        {
            _record_timestamp = now;
        }
        record_event({now - _record_timestamp, true, note});
    }
    _allocator->note_on(note);
}

void Sequencer::note_off(Note note)
{
    if (_recording)
    {
        record_event({millis(), false, note});
    }
    _allocator->note_off(note);
}

void Sequencer::update_state(bool recording)
{
    _recording = recording;
    if (_recording)
    {
        _size = 0;
    }
    else
    {
        _playback_timestamp = millis();
        _playback_index = 0;
    }
}

void Sequencer::update()
{
    if (!_recording && _playback_index < _size)
    {
        unsigned long elapsed = _config->get_sequencer_time_factor() * (millis() - _playback_timestamp);
        int i = _playback_index;
        while (_memory[i].timestamp <= elapsed)
        {
            if (_memory[i].type)
            {
                _allocator->note_on(_memory[i].note);
            }
            else
            {
                _allocator->note_off(_memory[i].note);
            }
            i++;
            if (i >= _size)
            {
                i = 0;
                _playback_timestamp = millis();
                elapsed = 0;
            }
        }
    }
}