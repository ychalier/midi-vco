#include "Arduino.h"
#include "../include/midi_interface.h"

MidiInterface::MidiInterface(Config *config, Allocator *allocator, int bend_dac_pin)
{
    _config = config;
    _allocator = allocator;
    _pitch_bend_value = 0;
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

void MidiInterface::handle_note_on(Note note)
{
    _allocator->note_on(note);
}

void MidiInterface::handle_note_off(Note note)
{
    _allocator->note_off(note);
}

void MidiInterface::handle_pitch_bend(int bend)
{
    _pitch_bend_value = bend;
    _allocator->pitch_bend(_pitch_bend_value);
}

void MidiInterface::handle_control_change(byte number, byte value)
{
    if (number == MIDI_CONTROL_MOD)
    {
        _dac->setVoltageB(map(value, 0, 127, 0, 4096));
        _dac->updateDAC();
    }
    int changed = _config->handle_midi_control(number, value);
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
