#include "Arduino.h"
#include "../include/sequencer.h"

Sequencer::Sequencer(Config *config, Allocator *allocator)
{
    _config = config;
    _allocator = allocator;
    _recording = false;
    _first_beat_timestamp = 0;
    _playback_division = 0;
    _started = false;
    for (int i = 0; i < SEQUENCER_TRACK_COUNT; i++)
    {
        _tracks[i] = new SequencerTrack(config, allocator);
    }
}

void Sequencer::update_source_activation(bool activated)
{
    if (activated)
    {
        _started = false;
        _allocator->reset();
        for (int i = 0; i < SEQUENCER_TRACK_COUNT; i++)
        {
            _tracks[i]->reset();
        }
        _first_beat_timestamp = millis();
    }
}

void Sequencer::update_record_state(bool recording)
{
    _recording = recording;
    if (_recording)
    {
        _allocator->reset_masked(_config->get_pool_mask());
        _tracks[_config->get_active_sequencer_track()]->reset();
    }
}

int Sequencer::get_record_division()
{
    return round((double)(millis() - _first_beat_timestamp) / (double)_config->get_time_div()) % SEQUENCER_DIVISIONS_PER_LOOP;
}

void Sequencer::start()
{
    if (!_started)
    {
        _first_beat_timestamp = millis();
        _started = true;
        _playback_division = SEQUENCER_DIVISIONS_PER_LOOP - 1;
    }
}

void Sequencer::note_on(byte pitch)
{
    byte pool_mask = _config->get_pool_mask();
    if (_recording)
    {
        start();
        _tracks[_config->get_active_sequencer_track()]->record(
            {get_record_division(),
             true,
             pitch,
             pool_mask});
    }
    _allocator->note_on_masked(pitch, pool_mask);
}

void Sequencer::note_off(byte pitch)
{
    byte pool_mask = _config->get_pool_mask();
    if (_recording)
    {
        start();
        _tracks[_config->get_active_sequencer_track()]->record(
            {get_record_division(),
             false,
             pitch,
             pool_mask});
    }
    _allocator->note_off_masked(pitch, pool_mask);
}

int Sequencer::get_playback_division()
{
    return (int)floor((double)(millis() - _first_beat_timestamp) / (double)_config->get_time_div()) % SEQUENCER_DIVISIONS_PER_LOOP;
}

void Sequencer::set_led_state(int division)
{
    bool led_state = false;
    if ((division % SEQUENCER_DIVISIONS_PER_BEAT) == 0)
    {
        int beat_number = division / SEQUENCER_DIVISIONS_PER_BEAT;
        led_state = beat_number != (SEQUENCER_BEATS_PER_LOOP - 1);
    }
    digitalWrite(PIN_LED, led_state ^ _recording);
}

void Sequencer::update()
{
    int new_playback_division = get_playback_division();
    set_led_state(new_playback_division);
    int active_sequencer_track = _config->get_active_sequencer_track();
    for (int i = 0; i < SEQUENCER_TRACK_COUNT; i++)
    {
        if (_recording && i == active_sequencer_track)
        {
            // Do not play anything while recording
        }
        else
        {
            if (new_playback_division > _playback_division)
            {
                for (int division = _playback_division + 1; division <= new_playback_division; division++)
                {
                    _tracks[i]->play(division);
                }
            }
            else if (new_playback_division < _playback_division)
            {
                for (int division = _playback_division + 1; division < SEQUENCER_DIVISIONS_PER_LOOP; division++)
                {
                    _tracks[i]->play(division);
                }
                for (int division = 0; division <= new_playback_division; division++)
                {
                    _tracks[i]->play(division);
                }
            }
        }
    }
    _playback_division = new_playback_division;
}