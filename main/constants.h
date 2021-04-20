#ifndef constants_h
#define constants_h


#define PIN_SELECT_MODE 44
#define PIN_SELECT_PRIORITY 42
#define PIN_SELECT_CHANNEL 40

#define PIN_LED_1 22
#define PIN_LED_2 24
#define PIN_LED_3 26
#define PIN_LED_4 28
#define PIN_LED_5 30
#define PIN_LED_6 32
#define PIN_LED_7 34
#define PIN_LED_8 36

#define PIN_SS_1 47
#define PIN_SS_2 45
#define PIN_SS_3 43
#define PIN_SS_4 41

#define PIN_GATE_1 37
#define PIN_GATE_2 35
#define PIN_GATE_3 33
#define PIN_GATE_4 31
#define PIN_GATE_5 29
#define PIN_GATE_6 27
#define PIN_GATE_7 25
#define PIN_GATE_8 23

#define DAC_RANGE 7.54  // Volts
#define DAC_BITS 4096  // 2^12
#define DAC_MAX_VOLTAGE_TARGET 7.0  // Volts

#define MIDI_PITCH_MIN 21  // A1
#define MIDI_PITCH_RANGE 84  // 7 octaves * 12 semitones

#define MODE_MONOPHONIC 0
#define MODE_DUOPHONIC 1
#define MODE_QUADROPHONIC 2
#define MODE_OCTOPHONIC 3

#define PRIORITY_FIRST 0
#define PRIORITY_LAST_OLDEST 1
#define PRIORITY_LAST_YOUNGEST 2

#define POOL_COUNT 8

#endif