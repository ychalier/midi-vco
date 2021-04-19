#include "router.h"

Router router;

void setup() {
    router.setup();
}

void loop() {
    router.play(1, 5.0, 1000);
    delay(1000);
}