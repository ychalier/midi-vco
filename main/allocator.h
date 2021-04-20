#ifndef allocator_h
#define allocator_h

#include "Arduino.h"
#include "config.h"
#include "note.h"
#include "pool.h"
#include "router.h"

class Allocator {

	public:
		Allocator(Config* config);
		void note_on(Note note);
		void note_off(Note note);
        void setup();
		void set_masks();
		// void pitch_bend(byte channel, int bend);  // TODO
	private:
		Config* _config;
		Pool* _pools[POOL_COUNT];
        Router* _router;

};

#endif