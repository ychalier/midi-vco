#include "Arduino.h"
#include "config.h"

Config::Config() {

}

byte Config::get_priority_mode() {
	return _priority_mode;
}

byte Config::get_polyphony_mode() {
	return _polyphony_mode;
}