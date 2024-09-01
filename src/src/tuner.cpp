#include "Arduino.h"
#include "../include/tuner.h"

Tuner::Tuner()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _tunings[i] = {0.0f, 1.0f};
    }
}

void Tuner::setup()
{
    byte eeprom_first_byte = EEPROM.read(0);
    if (eeprom_first_byte == 255)
    {
        EEPROM.write(0, 0);
        _write_tunings_to_eeprom();
    }
    else
    {
        _read_tunings_from_eeprom();
    }
}

Tuning* Tuner::get_tuning_ref(int lane_id)
{
    return &_tunings[lane_id];
}

void Tuner::reset_tunings()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        _tunings[i] = {0.0f, 1.0f};
    }
    _write_tunings_to_eeprom();
}

void Tuner::_read_tunings_from_eeprom()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        int address = 1 + sizeof(Tuning) * i;
        Tuning tuning;
        EEPROM.get(address, tuning);

    }
}

void Tuner::_write_tunings_to_eeprom()
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        int address = 1 + sizeof(Tuning) * i;
        EEPROM.put(address, _tunings[i]);
    }
}