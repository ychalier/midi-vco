#include "Arduino.h"
#include "allocator.h"
#include "constants.h"
#include "router.h"


Allocator::Allocator(Config* config) {
    _config = config;
    _router = new Router();
    for (int i = 0; i < POOL_COUNT; i++) {
        _pools[i] = new Pool(_router);
    }
}


void Allocator::set_masks() {
    for (int i = 0; i < POOL_COUNT; i++) {
        _pools[i]->set_masks(0, 0);
    }
    switch (_config->get_polyphony_mode()) {
    case MODE_MONOPHONIC:
        _pools[0]->set_masks(0xFF, 0xFFFF);
        break;
    case MODE_DUOPHONIC:
        _pools[0]->set_masks(0x0F, 0xFFFF);
        _pools[1]->set_masks(0xF0, 0xFFFF);
        break;
    case MODE_QUADROPHONIC:
        _pools[0]->set_masks(0x03, 0xFFFF);
        _pools[1]->set_masks(0x0C, 0xFFFF);
        _pools[2]->set_masks(0x30, 0xFFFF);
        _pools[3]->set_masks(0xC0, 0xFFFF);
        break;
    case MODE_OCTOPHONIC:
        for (int i = 0; i < POOL_COUNT; i++) {
            _pools[i]->set_masks(1 << i, 0xFFFF);
        }
        break;
    default:
        break;
    }
}


void Allocator::note_on(Note note) {
    bool accepted = false;
    for (int i = 0; i < POOL_COUNT; i++) {
        if (_pools[i]->free() && _pools[i]->accepts(note)) {
            _pools[i]->load(note);
            accepted = true;
            break;
        }
    }
    if (!accepted) {
        byte priority = _config->get_priority_mode();
        if (priority == PRIORITY_LAST_OLDEST || priority == PRIORITY_LAST_YOUNGEST) {
            int optimal_index = -1;
            unsigned long optimal_era = 0;
            for (int i = 0; i < POOL_COUNT; i++) {
                if (_pools[i]->accepts(note) && (optimal_index == -1 || (priority == PRIORITY_LAST_OLDEST && optimal_era > _pools[i]->get_era()) || (priority == PRIORITY_LAST_YOUNGEST && optimal_era < _pools[i]->get_era()))) {
                    optimal_index = i;
                    optimal_era = _pools[i]->get_era();
                }
            }
            if (optimal_index >= 0) {
                _pools[optimal_index]->load(note);
            }
        }
    }
}


void Allocator::note_off(Note note) {
    for (int i = 0; i < POOL_COUNT; i++) {
        if (!_pools[i]->free() && _pools[i]->current() == note) {
            _pools[i]->unload();
            break;
        }
    }
}

void Allocator::setup() {
    _router->setup();
}