#include "Arduino.h"
#include "../include/sequencer.h"

Sequencer::Sequencer(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    _recording = false;
    _loopback_timestamp = 0;
    for (int i = 0; i < SEQUENCER_CHANNEL_COUNT; i++)
    {
        _channels[i] = new SequencerChannel(config, allocator);
    }
}

void Sequencer::note_on(Note note)
{
    byte pool_mask = _config->get_pool_mask();
    if (_recording)
    {
        if (_loopback_timestamp == 0)
        {
            _loopback_timestamp = millis();
        }
        _channels[_config->get_sequencer_channel()]->record(
            true,
            note,
            pool_mask,
            _loopback_timestamp
        );
    }
    _allocator->note_on_masked(note, pool_mask);
}

void Sequencer::note_off(Note note)
{
    byte pool_mask = _config->get_pool_mask();
    if (_recording)
    {
        if (_loopback_timestamp == 0)
        {
            _loopback_timestamp = millis();
        }
        _channels[_config->get_sequencer_channel()]->record(
            false,
            note,
            pool_mask,
            _loopback_timestamp
        );
    }
    _allocator->note_off_masked(note, pool_mask);
}

void Sequencer::update_record_state(bool recording)
{
    _recording = recording;
    if (_recording)
    {
        _allocator->reset_masked(_config->get_pool_mask());
        _channels[_config->get_sequencer_channel()]->reset_memory();
    }
    else
    {
        _channels[_config->get_sequencer_channel()]->reset_playback();
    }
}

void Sequencer::update_source_activation(bool activated)
{
    if (activated)
    {
        _loopback_timestamp = 0;
        _allocator->reset();
    }
    else
    {

    }
}

bool Sequencer::update_loopback(unsigned long now)
{
    unsigned long ms_per_loop = _config->get_time_period() * _config->get_sequencer_steps();
    if (now - _loopback_timestamp > ms_per_loop)
    {
        _loopback_timestamp = now;
        return true;
    }
    else
    {
        return false;
    }
}

void Sequencer::update()
{
    unsigned long now = millis();
    bool loopback = update_loopback(now);
    int until_division = round( (double)(now - _loopback_timestamp) / (double)_config->get_time_div());
    int sequencer_channel = _config->get_sequencer_channel();
    for (int i = 0; i < SEQUENCER_CHANNEL_COUNT; i++)
    {
        if (_recording && i == sequencer_channel)
        {
            //Do not play sequence if it is recording
        }
        else
        {
            _channels[i]->play(until_division, loopback);
        }
    }
}