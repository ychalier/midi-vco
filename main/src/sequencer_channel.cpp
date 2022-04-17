#include "Arduino.h"
#include "../include/sequencer_channel.h"

SequencerChannel::SequencerChannel(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    _memory_index = 0;
    _playback_index = 0;
    _record_timestamp = 0;
    _first_note_index = 0;
}

bool SequencerChannel::record(bool type, Note note, byte mask, unsigned long loopback_timestamp)
{
    if (_memory_index < SEQUENCER_MEMORY_SIZE)
    {
        unsigned long now = millis();
        if (_memory_index == 0)
        {
            _record_timestamp = now;
            _first_note_index = 0;
        }
        int division = round((double)(now - loopback_timestamp) / (double)_config->get_time_div());
        if (division >= _config->get_sequencer_divisions())
        {
            division = _config->get_sequencer_divisions() - 1;
        }
        _memory[_memory_index] = { division, type, note, mask };
        if (division < _memory[_first_note_index].division)
        {
            _first_note_index = _memory_index;
        }
        _memory_index++;
        return true;
    }
    else
    {
        return false;
    }
}

void SequencerChannel::execute_event()
{
    if (_memory[_playback_index].type)
    {
        _allocator->note_on_masked(_memory[_playback_index].note, _memory[_playback_index].mask);
    }
    else
    {
        _allocator->note_off_masked(_memory[_playback_index].note, _memory[_playback_index].mask);
    }
    _playback_index++;
    if (_playback_index >= _memory_index)
    {
        _playback_index = 0;
    }
}

void SequencerChannel::play(int play_until_div, bool loopback)
{
    if (_playback_index < _memory_index && _memory_index > 0)
    {
        if (loopback)
        {
            while (_playback_index != _first_note_index)
            {
                execute_event();
            }
        }
        while (_memory[_playback_index].division <= play_until_div)
        {
            execute_event();
        }
    }
}

void SequencerChannel::reset_memory()
{
    _memory_index = 0;
    _record_timestamp = 0;
    _first_note_index = 0;
    _playback_index = 0;
}

void SequencerChannel::reset_playback()
{
    _playback_index = _first_note_index;
}
