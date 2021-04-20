#ifndef pool_h
#define pool_h

#include "Arduino.h"
#include "constants.h"
#include "router.h"
#include "note.h"


class Pool {

	public:
		Pool(Router* router);
		void set_masks(byte lane_mask, unsigned int channel_mask);
		bool accepts(Note note);
		bool free();
		void load(Note note);
		void unload();
		unsigned long get_era();
		Note current();

	private:
		Router* _router;
		byte _lane_mask;
		unsigned int _channel_mask;
		Note _current_note;
		unsigned long _era;
		bool _active;
};

#endif