#ifndef midi_interface_h
#define midi_interface_h

#include "Arduino.h"
#include <MIDI.h>
#include <MCP48xx.h>
#include "config.h"
#include "allocator.h"

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
     * @param bend_dac_pin PIN for the DAC outputting pitchbend and aftertouch
     */
    MidiInterface(Config *config, Allocator *allocator, int bend_dac_pin);

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

private:
    Config *_config;
    Allocator *_allocator;
    int _pitch_bend_value;
    MCP4822 *_dac;

};

#endif