#include "Arduino.h"
#include "../include/midi_interface.h"

MidiInterface::MidiInterface(Config *config, Allocator *allocator, Sequencer *sequencer)
{
    _config = config;
    _allocator = allocator;
    _sequencer = sequencer;
}

void MidiInterface::update()
{
    bool changed = _config->read();
    if (changed)
    {
        _allocator->set_masks();
        _allocator->display_state();
    }
    if (_config->get_active_source() == SOURCE_SEQUENCER)
    {
        _sequencer->update_playback();
    }
}

void MidiInterface::handle_note_on(byte channel, byte pitch, byte velocity)
{
    if (velocity == 0)
    {
        handle_note_off(channel, pitch, velocity);
    }
    else
    {
        Note note = {channel, pitch};
        _allocator->note_on(note);
        _sequencer->record_note_on(note);
    }
}

void MidiInterface::handle_note_off(byte channel, byte pitch, byte velocity)
{
    Note note = {channel, pitch};
    _allocator->note_off(note);
    _sequencer->record_note_off(note);
}

void MidiInterface::handle_pitch_bend(byte channel, int bend)
{
    _allocator->pitch_bend(channel, bend);
}

void MidiInterface::handle_control_change(byte channel, byte number, byte value)
{
    _config->handle_midi_control(channel, number, value);
    if (_sequencer->is_recording() != _config->should_sequencer_record())
    {
        if (_config->should_sequencer_record())
        {
            _sequencer->start_recording();
        }
        else
        {
            _sequencer->stop_recording();
        }
    }
    if (_sequencer->is_playing() != (_config->get_active_source() == SOURCE_SEQUENCER))
    {
        if (_sequencer->is_playing())
        {
            _sequencer->stop_playback();
        }
        else
        {
            _sequencer->start_playback();
        }
    }
}