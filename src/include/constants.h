#ifndef constants_h
#define constants_h

// PINs wiring

#define PIN_POLYPHONY_MODE A9    // Mono, Poly 5, Poly 10
#define PIN_PITCH_BEND_RANGE A10
#define PIN_GLIDE_INTENSITY A11
#define PIN_DETUNE A13           
#define PIN_MINIMUM_VELOCITY A7
#define PIN_CV_LOOPBACK_1  A0
#define PIN_CV_LOOPBACK_2  A1
#define PIN_CV_LOOPBACK_3  A2
#define PIN_CV_LOOPBACK_4  A3
#define PIN_CV_LOOPBACK_5  A4
#define PIN_CV_LOOPBACK_6  A5
#define PIN_CV_LOOPBACK_7  A6
#define PIN_CV_LOOPBACK_8  A7
#define PIN_CV_LOOPBACK_9  A8
#define PIN_CV_LOOPBACK_10 A9

#define PIN_PRIORITY_MODE 37 // Toggle replacement priority newest/oldest
#define PIN_BEND_CHANNEL_SWITCH 39
#define PIN_GLIDE_CHANNEL_SWITCH 40

#define PIN_BUTTON_TUNING_FAST 31
#define PIN_BUTTON_TUNING_FULL  32
#define PIN_BUTTON_TUNING_RESET 33

#define PIN_SS_CV_1 29
#define PIN_SS_CV_2 27
#define PIN_SS_CV_3 25
#define PIN_SS_CV_4 23
#define PIN_SS_CV_5 21
#define PIN_SS_BEND 31
#define PIN_SS_VEL_1 0 // TBD
#define PIN_SS_VEL_2 0 // TBD
#define PIN_SS_VEL_3 0 // TBD

#define PIN_GATE_1 28
#define PIN_GATE_2 26
#define PIN_GATE_3 24
#define PIN_GATE_4 22
#define PIN_GATE_5 23

#define PIN_LED 41

#define PIN_LED_TUNING_FAST 42
#define PIN_LED_TUNING_FULL 43
#define PIN_LED_TUNING_RESET 44

// MIDI Controls

#define MIDI_CONTROL_MOD 1
#define MIDI_CONTROL_GLIDE_INTENSITY 73  // S1 OFF
#define MIDI_CONTROL_GLIDE_CHROMATIC 80  // B1 ON
#define MIDI_CONTROL_GLIDE_LEGATO 81     // B2 ON
#define MIDI_CONTROL_PITCH_BEND_RANGE 75 // S2 OFF
#define MIDI_CONTROL_HOLD 83             // B4 ON
#define MIDI_CONTROL_VOLTAGE_OFFSET 2    // R4 ON

// Enumerations

#define MODE_MONOPHONIC 0
#define MODE_POLY5 1
#define MODE_POLY10 2

#define PRIORITY_KEEP_FIRST 0
#define PRIORITY_REPLACE_OLDEST 1
#define PRIORITY_REPLACE_NEWEST 2

// Flags

#define CONFIG_CHANGE_POLYPHONY_MODE 1
#define CONFIG_CHANGE_PRIORITY_MODE  2
#define CONFIG_CHANGE_HOLD           4
#define CONFIG_CHANGE_TUNING_FAST    8
#define CONFIG_CHANGE_TUNING_FULL   16
#define CONFIG_CHANGE_TUNING_RESET  32

#define GLIDE_FLAG_PROPORTIONAL 1
#define GLIDE_FLAG_CHROMATIC 2
#define GLIDE_FLAG_LEGATO 4

// Notes

#define PITCH_A0 21  // 27.5 Hz
#define PITCH_A1 33  // 55 Hz
#define PITCH_A2 45  // 110 Hz
#define PITCH_A3 57  // 220 Hz
#define PITCH_A4 69  // 440 Hz
#define PITCH_A5 81  // 880 Hz
#define PITCH_A6 93  // 1760 Hz
#define PITCH_A7 105 // 3520 Hz
#define PITCH_A8 117 // 7040 Hz

// General constants

#define DAC_VMAX 4096        // millivolts
#define AMP_GAIN 1.9531      // 8V / 4.096V
#define MIDI_MIN_PITCH 20    // Semitons
#define PITCH_BEND_RANGE 2.0 // Semitons

#define POOL_COUNT 10
#define LANE_COUNT 10

#define BUFFER_SIZE 10

#define GLIDE_MAX_RATE 50  // Milliseconds per semitones
#define GLIDE_MAX_TIME 500 // Milliseconds
#define GLIDE_INTENSITY_POWER 2

#define OUTPUT_PITCH_BEND_FACTOR 3.2766 // @see MidiInterface.handle_pitch_bend

#define DETUNE_VALUE_COUNT 15
const int DETUNE_VALUES[DETUNE_VALUE_COUNT] = {-24, -12, -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10, 12, 24};  // Semitones

#define GATE_STATE_DURING_TUNING LOW

#define CHANNEL_A true
#define CHANNEL_B false

#define FREQUENCY_ESTIMATION_PERIOD 2000000 // Microseconds (2s)
#define FREQUENCY_THRESHOLD         614     // On a 0-1023 scale
#define TUNING_OFFSET_PITCH         81      // A5 880 Hz
#define TUNING_REFERENCE_FREQUENCY  440.0f  // Hz


// Pre-computed constants

#define LOG2                        0.69314718056f

#endif