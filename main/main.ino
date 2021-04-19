#include <MIDI.h>
#include "constants.h"
#include "router.h"

MIDI_CREATE_DEFAULT_INSTANCE();
Router router;

void setup() {
    pinMode(PIN_LED_7, OUTPUT);
    digitalWrite(PIN_LED_7, LOW);
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handler_note_on);
    MIDI.setHandleNoteOff(handler_note_off);
    router.setup();
}

void loop() {
    MIDI.read();
}

double pitch_to_voltage(byte pitch) {
    double voltage = (double) DAC_MAX_VOLTAGE_TARGET * ((double) pitch - (double) MIDI_PITCH_MIN) / (double) MIDI_PITCH_RANGE;
    if (voltage < 0) {
        voltage = 0;
    } else if (voltage > DAC_MAX_VOLTAGE_TARGET) {
        voltage = DAC_MAX_VOLTAGE_TARGET;
    }
    return voltage;
}

void handler_note_on(byte channel, byte pitch, byte velocity) { 
    digitalWrite(PIN_LED_7, HIGH);
    double voltage = pitch_to_voltage(pitch);
    router.select(1)->start(voltage);
}

void handler_note_off(byte channel, byte pitch, byte velocity) { 
    digitalWrite(PIN_LED_7, LOW);
    router.select(1)->stop();
}
