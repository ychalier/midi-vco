#ifndef midi_interface_h
#define midi_interface_h

#include "Arduino.h"
#include <MIDI.h>
#include <MCP48xx.h>
#include "config.h"
#include "sequencer.h"
#include "allocator.h"
#include "arpeggiator.h"

/**
 * Provides handlers for the MIDI library callbacks. It currently supports
 * note-on, note-off, pitch-bend, control-change, after-touch-poly and
 * after-touch-channel.
 */
class MidiInterface
{
public:
    /**
     * Constructor.
     *
     * @param config A pointer to the global config.
     * @param allocator A pointer to the global allocator.
     * @param sequencer A pointer to a sequencer object.
     * @param arpeggiator A pointer to an arpeggiator object.
     * @param bend_dac_pin PIN for the DAC outputting pitchbend and aftertouch
     */
    MidiInterface(Config *config, Allocator *allocator, Sequencer *sequencer, Arpeggiator *arpeggiator, int bend_dac_pin);

    /**
     * Initialize hardware connections. Must be called once in the main program
     * setup phase.
     */
    void setup();

    /**
     * Callback for the MIDI note-on message.
     *
     * @param pitch Note pitch in semitons (7-bit integer).
     */
    void handle_note_on(byte pitch);

    /**
     * Callback for the MIDI note-off message.
     *
     * @param pitch Note pitch in semitons (7-bit integer).
     */
    void handle_note_off(byte pitch);

    /**
     * Callback for the MIDI pitch-bend message. Also updates the channel A of
     * the MidiInteface DAC. The pitch bend value (in the -8192, 8191 range) is
     * mapped to 0, 5000 (corresponding to a 0V-5V ouptut range).
     *
     * @param bend Signed 14-bit encoding of the current position of the
     *   pitch-bend control.
     */
    void handle_pitch_bend(int bend);

    /**
     * Callback to handle MIDI controls.
     *
     * @param number The control identifier.
     * @param value The current control value, a 7-bit integer.
     */
    void handle_control_change(byte number, byte value);

    /**
     * Callback for the polyphonic MIDI after-touch message. Also updates the
     * channel B of the MidiInteface DAC. The pressure value (in the 0, 127
     * range) is mapped to 0, 5000 (corresponding to a 0V-5V ouptut range).
     *
     * @param channel Channel of the note to modulate.
     * @param note Note associated with the modulation.
     * @param pressure 7-bit encoding of the amount of pressure.
     */
    void handle_after_touch_poly(byte pitch, byte pressure);

    /**
     * Callback for the channel MIDI after-touch message. Also updates the
     * channel B of the MidiInteface DAC. The pressure value (in the 0, 127
     * range) is mapped to 0, 5000 (corresponding to a 0V-5V ouptut range).
     *
     * @param pressure 7-bit encoding of the amount of pressure.
     */
    void handle_after_touch_channel(byte pressure);

private:
    Config *_config;
    Allocator *_allocator;
    Sequencer *_sequencer;
    Arpeggiator *_arpeggiator;
    int _pitch_bend_value;
    int _after_touch_value;
    MCP4822 *_dac;

    int _get_total_bend_value();
};

#endif