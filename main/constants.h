#ifndef constants_h
#define constants_h

// PINs wiring

#define PIN_SELECT_MODE A0
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

// Enumerations

#define MODE_MONOPHONIC 0
#define MODE_DUOPHONIC 1
#define MODE_QUADROPHONIC 2
#define MODE_OCTOPHONIC 3

#define PRIORITY_KEEP_FIRST 0
#define PRIORITY_REPLACE_FIRST 1
#define PRIORITY_REPLACE_LAST 2

#define CHANNEL_FILTER_OFF 0
#define CHANNEL_FILTER_ON 1

// General constants

#define DAC_VMAX 4.096        // Volts
#define AMP_GAIN 1.8408203125 // 7.54V / 4.096V
#define MIDI_MIN_PITCH 20     // Semitons
#define PITCH_BEND_RANGE 2.0  // Semitons

#define DEFAULT_DISPLAY_DURATION 1000 // Milliseconds

#define LANE_COUNT 8

#endif