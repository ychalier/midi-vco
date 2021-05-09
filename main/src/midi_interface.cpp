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
    bool changed = _config->read();
    if (changed)
    {
        _allocator->set_masks();
        _allocator->display_state();
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
    switch (_config->handle_midi_control(channel, number, value))
    {
    case CONFIG_CHANGE_SEQUENCER_RECORD:
        _sequencer->update_state(_config->should_sequencer_record());
        break;
    case CONFIG_CHANGE_SOURCE:
        _allocator->reset();
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
        break;
    case CONFIG_CHANGE_HOLD:
        if (_config->get_hold())
        {
            _allocator->hold_on();
        }
        else
        {
            _allocator->hold_off();
        }
        break;
    }
}