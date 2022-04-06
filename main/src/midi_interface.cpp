#include "Arduino.h"
#include "../include/midi_interface.h"

MidiInterface::MidiInterface(Config *config, Allocator *allocator, Sequencer *sequencer, Arpeggiator *arpeggiator)
{
    _config = config;
    _allocator = allocator;
    _sequencer = sequencer;
    _arpeggiator = arpeggiator;
}

void MidiInterface::update()
{
    int changed = _config->read();
    if (changed & (CONFIG_CHANGE_POLYPHONY_MODE | CONFIG_CHANGE_PRIORITY_MODE | CONFIG_CHANGE_CHANNEL_FILTER) > 0)
    {
        _allocator->set_masks();
        _allocator->display_state();
    }
    if (changed & CONFIG_CHANGE_SOURCE > 0)
    {
        _allocator->reset();
        _arpeggiator->reset();
        if (_config->get_active_source() == SOURCE_SEQUENCER)
        {
            _sequencer->update_state(false);
        }
    }
    if (changed & CONFIG_CHANGE_SEQUENCER_RECORD > 0)
    {
        _sequencer->update_state(_config->should_sequencer_record());
    }
    switch (_config->get_active_source())
    {
    case SOURCE_SEQUENCER:
        _sequencer->update();
        break;
    case SOURCE_ARPEGGIATOR:
        _arpeggiator->update();
        break;
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
        switch (_config->get_active_source())
        {
        case SOURCE_DIRECT:
            _allocator->note_on(note);
            break;
        case SOURCE_SEQUENCER:
            _sequencer->note_on(note);
            break;
        case SOURCE_ARPEGGIATOR:
            _arpeggiator->note_on(note);
            break;
        }
    }
}

void MidiInterface::handle_note_off(byte channel, byte pitch, byte velocity)
{
    Note note = {channel, pitch};
    switch (_config->get_active_source())
    {
    case SOURCE_DIRECT:
        _allocator->note_off(note);
        break;
    case SOURCE_SEQUENCER:
        _sequencer->note_off(note);
        break;
    case SOURCE_ARPEGGIATOR:
        _arpeggiator->note_off(note);
        break;
    }
}

void MidiInterface::handle_pitch_bend(byte channel, int bend)
{
    _allocator->pitch_bend(channel, bend);
}

void MidiInterface::handle_control_change(byte channel, byte number, byte value)
{
    int changed = _config->handle_midi_control(channel, number, value);
    if (changed & CONFIG_CHANGE_SEQUENCER_RECORD > 0)
    {
        _sequencer->update_state(_config->should_sequencer_record());
    }
    if (changed & CONFIG_CHANGE_SOURCE)
    {
        _allocator->reset();
        _arpeggiator->reset();
        switch (_config->get_active_source())
        {
        case SOURCE_DIRECT:
            break;
        case SOURCE_SEQUENCER:
            _sequencer->update_state(false);
            break;
        case SOURCE_ARPEGGIATOR:
            break;
        }
    }
    if (changed & CONFIG_CHANGE_HOLD)
    {
        if (_config->get_hold())
        {
            _allocator->hold_on();
        }
        else
        {
            _allocator->hold_off();
        }
    }
}

void MidiInterface::handle_after_touch_poly(byte channel, byte note, byte pressure)
{
    _allocator->after_touch_poly({channel, note}, pressure);
}

void MidiInterface::handle_after_touch_channel(byte channel, byte pressure)
{
    _allocator->after_touch_channel(channel, pressure);
}