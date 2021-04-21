#include "Arduino.h"
#include "pool.h"
#include "lane.h"
#include <MIDI.h>

Pool::Pool(Router* router) {
    _router = router;
    _lane_mask = 0;
    _channel_mask = 0;
    _current_note = { 1, 0 };
    _era = 0;
    _active = false;
}

unsigned int channel_to_mask(byte channel) {
    if (channel == MIDI_CHANNEL_OMNI) {
        return 0xFFFF;
    } else if (channel >= 1 && channel <= 16) {
        return 1 << (channel - 1);
    } else {
        return 0;
    }
}

void Pool::set_masks(byte lane_mask, unsigned int channel_mask) {
    _lane_mask = lane_mask;
    _channel_mask = channel_mask;
}


bool Pool::accepts(Note note) {
    return _lane_mask > 0 && (_channel_mask & channel_to_mask(note.channel)) > 0;
}


bool Pool::free() {
    return !_active;
}

void Pool::load(Note note) {
    _active = true;
    _era = millis();
    _current_note = note;
    for (byte lane_id = 0; lane_id < 8; lane_id++) {
        if ((_lane_mask & (1 << lane_id)) > 0) {
            _router->select(lane_id + 1)->start(Lane::pitch_to_voltage(note.pitch, 0));
        }
    }
}

void Pool::unload() {
    _active = false;
    for (byte lane_id = 0; lane_id < 8; lane_id++) {
        if ((_lane_mask & (1 << lane_id)) > 0) {
            _router->select(lane_id + 1)->stop();
        }
    }
}

unsigned long Pool::get_era() {
    return _era;
}

Note Pool::current() {
    return _current_note;
}

void Pool::bend(int bend_value) {
    for (byte lane_id = 0; lane_id < 8; lane_id++) {
        if ((_lane_mask & (1 << lane_id)) > 0) {
            _router->select(lane_id + 1)->set(Lane::pitch_to_voltage(_current_note.pitch, bend_value));
        }
    }
}