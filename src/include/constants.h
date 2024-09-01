#ifndef constants_h
#define constants_h

// Analog input

#define PIN_POLYPHONY_MODE   A0
#define PIN_PITCH_BEND_RANGE A1
#define PIN_GLIDE_INTENSITY  A2
#define PIN_DETUNE           A3           
#define PIN_MINIMUM_VELOCITY A4
#define PIN_CV_LOOPBACK_1    A5
#define PIN_CV_LOOPBACK_2    A6
#define PIN_CV_LOOPBACK_3    A7
#define PIN_CV_LOOPBACK_4    A8
#define PIN_CV_LOOPBACK_5    A9
#define PIN_CV_LOOPBACK_6    A10
#define PIN_CV_LOOPBACK_7    A11
#define PIN_CV_LOOPBACK_8    A12
#define PIN_CV_LOOPBACK_9    A13
#define PIN_CV_LOOPBACK_10   A14

// Digital input

#define PIN_PRIORITY_MODE        1
#define PIN_BEND_CHANNEL_SWITCH  2
#define PIN_GLIDE_CHANNEL_SWITCH 3
#define PIN_BUTTON_TUNING_FAST   4
#define PIN_BUTTON_TUNING_FULL   5
#define PIN_BUTTON_TUNING_RESET  6

// Digital output

#define PIN_SS_CV_1              7
#define PIN_SS_CV_2              8
#define PIN_SS_CV_3              9
#define PIN_SS_CV_4              10
#define PIN_SS_CV_5              11
#define PIN_SS_VEL_1             12
#define PIN_SS_VEL_2             13
#define PIN_SS_VEL_3             14
#define PIN_GATE_1               15
#define PIN_GATE_2               16
#define PIN_GATE_3               17
#define PIN_GATE_4               18
#define PIN_GATE_5               19
#define PIN_LED_MIDI             20
#define PIN_LED_TUNING_FAST      21
#define PIN_LED_TUNING_FULL      22
#define PIN_LED_TUNING_RESET     23

// MIDI Controls

#define MIDI_CONTROL_MOD 1

// Enumerations

#define MODE_MONOPHONIC 0
#define MODE_POLY5      1
#define MODE_POLY10     2

#define PRIORITY_KEEP_FIRST     0
#define PRIORITY_REPLACE_OLDEST 1
#define PRIORITY_REPLACE_NEWEST 2

// Flags

#define CONFIG_CHANGE_POLYPHONY_MODE 1
#define CONFIG_CHANGE_PRIORITY_MODE  2
#define CONFIG_CHANGE_HOLD           4
#define CONFIG_CHANGE_TUNING_FAST    8
#define CONFIG_CHANGE_TUNING_FULL   16
#define CONFIG_CHANGE_TUNING_RESET  32

#define GLIDE_FLAG_CHROMATIC 1 // Glide goes through semitones
#define GLIDE_FLAG_LEGATO    2 // Glide only active when notes overlap

// Constants

#define PITCH_A0 21  // 27.5 Hz
#define PITCH_A1 33  // 55 Hz
#define PITCH_A2 45  // 110 Hz
#define PITCH_A3 57  // 220 Hz
#define PITCH_A4 69  // 440 Hz
#define PITCH_A5 81  // 880 Hz
#define PITCH_A6 93  // 1760 Hz
#define PITCH_A7 105 // 3520 Hz
#define PITCH_A8 117 // 7040 Hz

#define CHANNEL_A true
#define CHANNEL_B false

// Pre-computed numbers

#define LOG2 0.69314718056f

// Parameters

#define POOL_COUNT                  10
#define LANE_COUNT                  10
#define BUFFER_SIZE                 10
#define DAC_VMAX                    4096    // millivolts
#define AMP_GAIN                    1.9531  // 8V / 4.096V
#define MIDI_MIN_PITCH              20      // Semitons
#define DEFAULT_PITCH_BEND_RANGE    2.0     // Semitons
#define GLIDE_MAX_RATE              50      // Milliseconds per semitones
#define GLIDE_MAX_TIME              500     // Milliseconds
#define GLIDE_INTENSITY_POWER       2
#define GLIDE_FLAGS                 0
#define GATE_STATE_DURING_TUNING    LOW
#define FREQUENCY_ESTIMATION_PERIOD 2000000 // Microseconds (2s)
#define FREQUENCY_THRESHOLD         614     // On a 0-1023 scale
#define TUNING_OFFSET_PITCH         81      // A5 880 Hz
#define TUNING_REFERENCE_FREQUENCY  440.0f  // Hz
#define MIDI_LED_TIMEOUT            300     // milliseconds
#define DETUNE_VALUE_COUNT 15
const int DETUNE_VALUES[DETUNE_VALUE_COUNT] = {-24, -12, -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10, 12, 24};  // Semitones

#endif