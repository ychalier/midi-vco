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
    _dac->setVoltageA(2048);
    _dac->setVoltageB(2048);
    _dac->updateDAC();
}

void MidiInterface::handle_note_on(byte pitch)
{
    switch (_config->get_active_source())
    {
    case SOURCE_DIRECT:
        _allocator->note_on(pitch);
        break;
    case SOURCE_SEQUENCER:
        _sequencer->note_on(pitch);
        break;
    case SOURCE_ARPEGGIATOR:
        _arpeggiator->note_on(pitch);
        break;
    }
}

void MidiInterface::handle_note_off(byte pitch)
{
    switch (_config->get_active_source())
    {
    case SOURCE_DIRECT:
        _allocator->note_off(pitch);
        break;
    case SOURCE_SEQUENCER:
        _sequencer->note_off(pitch);
        break;
    case SOURCE_ARPEGGIATOR:
        _arpeggiator->note_off(pitch);
        break;
    }
}

void MidiInterface::handle_pitch_bend(int bend)
{
    _pitch_bend_value = bend;
    _allocator->pitch_bend(_get_total_bend_value());
}

void MidiInterface::handle_control_change(byte number, byte value)
{
    if (number == MIDI_CONTROL_MOD)
    {
        _dac->setVoltageB(map(value, 0, 127, 0, 4096));
        _dac->updateDAC();
    }
    int changed = _config->handle_midi_control(number, value);
    if (changed & CONFIG_CHANGE_RECORD)
    {
        _sequencer->update_record_state(_config->is_recording());
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

void MidiInterface::handle_after_touch_poly(byte pitch, byte pressure)
{
    _after_touch_value = AFTERTOUCH_COEFF * pressure;
    _allocator->after_touch_poly(pitch, _get_total_bend_value());
    _dac->setVoltageA(map(pressure, 0, 127, 0, 4096));
    _dac->updateDAC();
}

void MidiInterface::handle_after_touch_channel(byte pressure)
{
    _after_touch_value = AFTERTOUCH_COEFF * pressure;
    _allocator->after_touch_channel(_get_total_bend_value());
    _dac->setVoltageA(map(pressure, 0, 127, 0, 4096));
    _dac->updateDAC();
}

int MidiInterface::_get_total_bend_value()
{
    return _pitch_bend_value + _after_touch_value;
}