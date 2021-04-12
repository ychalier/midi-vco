#include <SoftwareSerial.h>

#define pin_rx 00
#define pin_tx 01

SoftwareSerial mySerial(pin_rx, pin_tx);

void setup() {
    // pinMode(pin_rx, INPUT);
    // pinMode(pin_tx, OUTPUT);
    mySerial.begin(31250);  // Apparently, MIDI requires this specific baudrate
    Serial.begin(9600);
    Serial.println("setup");
}

void loop() {
    if (Serial1.available() > 0){
        Serial.println("Hello, World!");
        Serial.println(mySerial.read());
    }
}
