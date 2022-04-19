#include "Arduino.h"
#include "../include/midi_interface.h"

MidiInterface::MidiInterface(Config *config, Allocator *allocator, Sequencer *sequencer, Arpeggiator *arpeggiator, int bend_dac_pin)
{
    _config = config;
    _allocator = allocator;
    _sequencer = sequencer;
    _arpeggiator = arpeggiator;
    _pitch_bend_value = 0;
    _after_touch_value = 0;
    _dac = new MCP4822(bend_dac_pin);
}

void MidiInterface::setup()
{
    _dac->init();
    _dac->turnOnChannelA();
    _dac->setGainA(MCP4822::High);
    _dac->turnOnChannelB();
    _dac->setGainB(MCP4822::High);
    _dac->setVoltageA(2500);
    _dac->setVoltageB(0);
    _dac->updateDAC();
}

void MidiInterface::update()
{
    int changed = _config->read();
    if (changed & CONFIG_CHANGE_POLYPHONY_MODE)
    {
        _allocator->set_masks();
    }
    if (changed & CONFIG_CHANGE_SOURCE)
    {
        _allocator->reset();
        _arpeggiator->reset();
        _sequencer->update_source_activation(_config->get_active_source() == SOURCE_SEQUENCER);
    }
    if (changed & CONFIG_CHANGE_SEQUENCER_RECORD)
    {
        _sequencer->update_record_state(_config->should_sequencer_record());
    }
    if (changed & CONFIG_CHANGE_TUNING)
    {
        _allocator->reset();
        _arpeggiator->reset();
        if (_config->is_tuning())
        {
            _allocator->broadcast_pitch(69); // Should be A4 (440 Hz)
        }
    }
    if (!_config->is_tuning())
    {
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
    _pitch_bend_value = bend;
    _allocator->pitch_bend(channel, _get_total_bend_value());
    _dac->setVoltageA((int) ((float)(bend + 8192) / OUTPUT_PITCH_BEND_FACTOR));
    _dac->updateDAC();
}

void MidiInterface::handle_control_change(byte channel, byte number, byte value)
{
    int changed = _config->handle_midi_control(channel, number, value);
    if (changed & CONFIG_CHANGE_SEQUENCER_RECORD)
    {
        _sequencer->update_state(_config->should_sequencer_record());
    }
    if (changed & CONFIG_CHANGE_SOURCE)
    {
        _allocator->reset();
        _arpeggiator->reset();
        _sequencer->update_source_activation(_config->get_active_source() == SOURCE_SEQUENCER);
    }
    if (changed & CONFIG_CHANGE_SEQUENCER_RECORD > 0)
    {
        _sequencer->update_record_state(_config->should_sequencer_record());
    }
    if (changed & CONFIG_CHANGE_HOLD > 0)
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
    _after_touch_value = AFTERTOUCH_COEFF * pressure;
    _allocator->after_touch_poly({channel, note}, _get_total_bend_value());
    _dac->setVoltageB(pressure * OUTPUT_AFTER_TOUCH_FACTOR);
    _dac->updateDAC();
}

void MidiInterface::handle_after_touch_channel(byte channel, byte pressure)
{
    _after_touch_value = AFTERTOUCH_COEFF * pressure;
    _allocator->after_touch_channel(channel, _get_total_bend_value());
    _dac->setVoltageB(pressure * OUTPUT_AFTER_TOUCH_FACTOR);
    _dac->updateDAC();
}

int MidiInterface::_get_total_bend_value()
{
    return _pitch_bend_value + _after_touch_value;
}