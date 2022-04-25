#ifndef constants_h
#define constants_h

// PINs wiring

#define PIN_SOURCE A8            // Midi, Arp, Seq
#define PIN_POLYPHONY_MODE A9    // Mono, Poly 2, Poly 4, Poly 8
#define PIN_PITCH_BEND_RANGE A10 // Also impacts aftertouch
#define PIN_GLIDE_INTENSITY A11
#define PIN_ARPEGGIATOR_MODE A12 // Up, Down, Up/Down
#define PIN_DETUNE A13           // Not implemented
#define PIN_TIME A14             // For the sequencer & the arpeggiator

#define PIN_PRIORITY_MODE 42  // Toggle replacement priority newest/oldest
#define PIN_TUNE 35
#define PIN_REC 39

#define PIN_SS_CV_1 29
#define PIN_SS_CV_2 27
#define PIN_SS_CV_3 25
#define PIN_SS_CV_4 23
#define PIN_SS_BEND 31

#define PIN_GATE_1 28
#define PIN_GATE_2 26
#define PIN_GATE_3 24
#define PIN_GATE_4 22

#define PIN_LED 41

// MIDI Controls

#define MIDI_CONTROL_GLIDE_INTENSITY 73     // S1 OFF
#define MIDI_CONTROL_GLIDE_CHROMATIC 80     // B1 ON
#define MIDI_CONTROL_GLIDE_LEGATO 81        // B2 ON
#define MIDI_CONTROL_PITCH_BEND_RANGE 75    // S2 OFF
#define MIDI_CONTROL_SEQUENCER_RECORD 82    // B3 OFF
#define MIDI_CONTROL_SOURCE 74              // R1 OFF
#define MIDI_CONTROL_TIME 71                // R2 OFF
#define MIDI_CONTROL_ARPEGGIATOR_MODE 65    // R3 OFF
#define MIDI_CONTROL_ARPEGGIATOR_SUSTAIN 72 // S3 ON
#define MIDI_CONTROL_HOLD 83                // B4 ON
#define MIDI_CONTROL_VOLTAGE_OFFSET 2       // R4 ON

// Enumerations

#define MODE_MONOPHONIC 0
#define MODE_DUOPHONIC 1
#define MODE_QUADROPHONIC 2
#define MODE_OCTOPHONIC 3

#define PRIORITY_KEEP_FIRST 0
#define PRIORITY_REPLACE_OLDEST 1
#define PRIORITY_REPLACE_NEWEST 2

#define SOURCE_DIRECT 0
#define SOURCE_SEQUENCER 1
#define SOURCE_ARPEGGIATOR 2

#define ARPEGGIATOR_MODE_UP 0
#define ARPEGGIATOR_MODE_DOWN 1
#define ARPEGGIATOR_MODE_UP_DOWN 2
#define ARPEGGIATOR_MODE_RANDOM 3 // Ignored

// Flags

#define CONFIG_CHANGE_SOURCE 1
#define CONFIG_CHANGE_POLYPHONY_MODE 2
#define CONFIG_CHANGE_PRIORITY_MODE 4
#define CONFIG_CHANGE_SEQUENCER_RECORD 8
#define CONFIG_CHANGE_TUNING 16
#define CONFIG_CHANGE_HOLD 32

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

#define DAC_VMAX 4.096       // Volts
#define AMP_GAIN 1.9531      // 8V / 4.096V
#define MIDI_MIN_PITCH 20    // Semitons
#define PITCH_BEND_RANGE 2.0 // Semitons

#define LANE_COUNT 8

#define BUFFER_SIZE 8

#define GLIDE_MAX_RATE 50  // Milliseconds per semitones
#define GLIDE_MAX_TIME 500 // Milliseconds
#define GLIDE_INTENSITY_POWER 2

#define SEQUENCER_CHANNEL_COUNT 3
#define SEQUENCER_BEATS_PER_LOOP 4
#define SEQUENCER_DIVISIONS_PER_BEAT 4
#define SEQUENCER_MAX_EVENTS_PER_DIVISION 16
const int SEQUENCER_DIVISIONS_PER_LOOP = SEQUENCER_DIVISIONS_PER_BEAT * SEQUENCER_BEATS_PER_LOOP;
const int SEQUENCER_MEMORY_SIZE = SEQUENCER_DIVISIONS_PER_LOOP * SEQUENCER_MAX_EVENTS_PER_DIVISION;

#define AFTERTOUCH_COEFF 8

#define TIME_MIN_BPM 60
#define TIME_MAX_BPM 600

#define OUTPUT_PITCH_BEND_FACTOR 3.2766 // @see MidiInterface.handle_pitch_bend
#define OUTPUT_AFTER_TOUCH_FACTOR 39    // @see MidiInterface.handle_after_touch_poly or MidiInterface.handle_after_touch_channel

#define DETUNE_RANGE 2 // Semitons (in both directions)

#define GATE_STATE_DURING_TUNING LOW

#endif