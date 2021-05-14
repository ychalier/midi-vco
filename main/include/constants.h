#ifndef constants_h
#define constants_h

// PINs wiring

#define PIN_SELECT_MODE A8
#define PIN_SELECT_PRIORITY 42
#define PIN_SELECT_CHANNEL 40

#define PIN_LED_1 34
#define PIN_LED_2 32
#define PIN_LED_3 30
#define PIN_LED_4 28
#define PIN_LED_5 26
#define PIN_LED_6 24
#define PIN_LED_7 22
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

// MIDI Controls

#define MIDI_CONTROL_GLIDE_INTENSITY 73     // S1
#define MIDI_CONTROL_GLIDE_CHROMATIC 80     // B1
#define MIDI_CONTROL_GLIDE_LEGATO 81        // B2
#define MIDI_CONTROL_PITCH_BEND_RANGE 75    // S2
#define MIDI_CONTROL_SEQUENCER_RECORD 82    // B3
#define MIDI_CONTROL_SOURCE 74              // R1
#define MIDI_CONTROL_TIME 71                // R2
#define MIDI_CONTROL_ARPEGGIATOR_MODE 65    // R3
#define MIDI_CONTROL_ARPEGGIATOR_SUSTAIN 72 // S3
#define MIDI_CONTROL_HOLD 83                // B4
#define MIDI_CONTROL_VOLTAGE_OFFSET 2       // R4

// Enumerations

#define MODE_MONOPHONIC 0
#define MODE_DUOPHONIC 1
#define MODE_QUADROPHONIC 2
#define MODE_OCTOPHONIC 3

#define PRIORITY_KEEP_FIRST 0
#define PRIORITY_REPLACE_OLDEST 1
#define PRIORITY_REPLACE_NEWEST 2

#define CHANNEL_FILTER_OFF 0
#define CHANNEL_FILTER_ON 1

#define SOURCE_DIRECT 0
#define SOURCE_SEQUENCER 1
#define SOURCE_ARPEGGIATOR 2

#define ARPEGGIATOR_MODE_UP 0
#define ARPEGGIATOR_MODE_DOWN 1
#define ARPEGGIATOR_MODE_UP_DOWN 2
#define ARPEGGIATOR_MODE_RANDOM 3

// Flags

#define CONFIG_CHANGE_SOURCE 1
#define CONFIG_CHANGE_SEQUENCER_RECORD 2
#define CONFIG_CHANGE_HOLD 4

#define GLIDE_FLAG_PROPORTIONAL 1
#define GLIDE_FLAG_CHROMATIC 2
#define GLIDE_FLAG_LEGATO 4

// General constants

#define DAC_VMAX 4.096       // Volts
#define AMP_GAIN 1.9531      // 8V / 4.096V
#define MIDI_MIN_PITCH 20    // Semitons
#define PITCH_BEND_RANGE 2.0 // Semitons

#define DEFAULT_DISPLAY_DURATION 1000 // Milliseconds

#define LANE_COUNT 8

#define BUFFER_SIZE 8

#define GLIDE_MAX_RATE 50  // Milliseconds per semitones
#define GLIDE_MAX_TIME 500 // Milliseconds
#define GLIDE_INTENSITY_POWER 2

#define SEQUENCER_MEMORY_SIZE 64
#define SEQUENCER_WINDOW_SIZE 150 // Milliseconds

#define AFTERTOUCH_COEFF 8

#define TIME_MIN_BPM 60
#define TIME_MAX_BPM 600

#endif