#include "Arduino.h"
#include "../include/sequencer.h"

Sequencer::Sequencer(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    _size = 0;
    _recording = false;
    _playing = true;
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

void Sequencer::record_note_on(Note note)
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
}

void Sequencer::record_note_off(Note note)
{
    if (_recording)
    {
        record_event({millis(), false, note});
    }
}

void Sequencer::start_recording()
{
    if (!_recording)
    {
        _size = 0;
        _recording = true;
    }
}

void Sequencer::stop_recording()
{
    _recording = false;
}

void Sequencer::start_playback()
{
    if (!_playing)
    {
        _playback_timestamp = millis();
        _playback_index = 0;
        _playing = true;
    }
}

void Sequencer::stop_playback()
{
    _playing = false;
}

void Sequencer::update_playback()
{
    if (_playing && _playback_index < _size)
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

bool Sequencer::is_recording()
{
    return _recording;
}

bool Sequencer::is_playing()
{
    return _playing;
}