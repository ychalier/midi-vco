#include <SPI.h>
#include <MCP48xx.h>  // https://www.arduino.cc/reference/en/libraries/mcp48xx-dac-library/

#define pin_ss_1 47
#define pin_ss_2 45
#define pin_ss_3 43
#define pin_ss_4 41

#define gate_1a 37
#define gate_1b 35
#define gate_2a 33
#define gate_2b 31
#define gate_3a 29
#define gate_3b 27
#define gate_4a 25
#define gate_4b 23

MCP4822 dac1(pin_ss_1);
MCP4822 dac2(pin_ss_2);
MCP4822 dac3(pin_ss_3); 
MCP4822 dac4(pin_ss_4);

int voltages[] = {0, 543, 1086, 1630, 2173, 2716};
int voltagesIndex = 10;

void setup() {
    Serial.begin(9600);
    Serial.println("setup");
    dac1.init();
    dac1.turnOnChannelA();
    dac1.turnOnChannelB();
    dac1.setGainA(MCP4822::High);
    dac1.setGainB(MCP4822::High);
    dac2.init();
    dac2.turnOnChannelA();
    dac2.turnOnChannelB();
    dac2.setGainA(MCP4822::High);
    dac2.setGainB(MCP4822::High);
    dac3.init();
    dac3.turnOnChannelA();
    dac3.turnOnChannelB();
    dac3.setGainA(MCP4822::High);
    dac3.setGainB(MCP4822::High);
    dac4.init();
    dac4.turnOnChannelA();
    dac4.turnOnChannelB();
    dac4.setGainA(MCP4822::High);
    dac4.setGainB(MCP4822::High);
    pinMode(gate_1a, OUTPUT);
    pinMode(gate_1b, OUTPUT);
    pinMode(gate_2a, OUTPUT);
    pinMode(gate_2b, OUTPUT);
    pinMode(gate_3a, OUTPUT);
    pinMode(gate_3b, OUTPUT);
    pinMode(gate_4a, OUTPUT);
    pinMode(gate_4b, OUTPUT);
    digitalWrite(gate_1a, LOW);
    digitalWrite(gate_1b, LOW);
    digitalWrite(gate_2a, LOW);
    digitalWrite(gate_2b, LOW);
    digitalWrite(gate_3a, LOW);
    digitalWrite(gate_3b, LOW);
    digitalWrite(gate_4a, LOW);
    digitalWrite(gate_4b, LOW);
}

int scaleVoltage(double target) {
    // At full power, the amplified DAC sends a 7.54V signal,
    // and by default the DAC has 4096 possible values.
    return (int) 4096.0 * target / 7.54;
}

void loop() {
    Serial.println("loop");

    dac1.setVoltageA(scaleVoltage(4.0));
    dac1.updateDAC();
    digitalWrite(gate_1a, HIGH);
    delay(1000);
    digitalWrite(gate_1a, LOW);
    delay(200);

    dac1.setVoltageA(scaleVoltage(5.0));
    dac1.updateDAC();
    digitalWrite(gate_1a, HIGH);
    delay(200);
    digitalWrite(gate_1a, LOW);
    delay(200);

    dac1.setVoltageA(scaleVoltage(5.0));
    dac1.updateDAC();
    digitalWrite(gate_1a, HIGH);
    delay(200);
    digitalWrite(gate_1a, LOW);
    delay(1000);
}