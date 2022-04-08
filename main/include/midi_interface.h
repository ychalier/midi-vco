#ifndef midi_interface_h
#define midi_interface_h

#include "Arduino.h"
#include <MIDI.h>
#include "config.h"
#include "sequencer.h"
#include "allocator.h"
#include "arpeggiator.h"


/**
 * Interface between the MIDI messages and the system components.
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
     */
    MidiInterface(Config *config, Allocator *allocator, Sequencer *sequencer, Arpeggiator *arpeggiator);

    /**
     * Depending on the currently active source, update the sequencer or the
     * arpeggiator. Should be called once per main loop.
     */
    void update();

    /**
     * Callback for the MIDI note-on message.
     *
     * @param channel MIDI channel associated with the note.
     * @param pitch Note pitch in semitons (7-bit integer).
     * @param velocity Velocity associated with the note.
     */
    void handle_note_on(byte channel, byte pitch, byte velocity);

    /**
     * Callback for the MIDI note-off message.
     *
     * @param channel MIDI channel associated with the note.
     * @param pitch Note pitch in semitons (7-bit integer).
     * @param velocity Velocity associated with the note.
     */
    void handle_note_off(byte channel, byte pitch, byte velocity);

    /**
     * Callback for the MIDI pitch-bend message.
     *
     * @param channel MIDI channel to bend the pitch of.
     * @param bend Signed 14-bit encoding of the current position of the
     *   pitch-bend control.
     */
    void handle_pitch_bend(byte channel, int bend);

    /**
     * Callback to handle MIDI controls.
     * 
     * @param channel The MIDI channel concerned with the change.
     * @param number The control identifier.
     * @param value The current control value, a 7-bit integer.
     */
    void handle_control_change(byte channel, byte number, byte value);

    /**
     * Callback for the polyphonic MIDI after-touch message.
     *
     * @param channel Channel of the note to modulate.
     * @param note Note associated with the modulation.
     * @param pressure 7-bit encoding of the amount of pressure.
     */
    void handle_after_touch_poly(byte channel, byte note, byte pressure);

    /**
     * Callback for the channel MIDI after-touch message.
     *
     * @param channel Channel to modulate.
     * @param pressure 7-bit encoding of the amount of pressure.
     */
    void handle_after_touch_channel(byte channel, byte pressure);

private:
    Config *_config;
    Allocator *_allocator;
    Sequencer *_sequencer;
    Arpeggiator *_arpeggiator;
    int _pitch_bend_value;
    int _after_touch_value;

    int _get_total_bend_value();
};

#endif