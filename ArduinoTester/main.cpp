#include <Arduino.h>
#include "../IO/IOArduino.h"

extern HardwareSerial Serial;

void setup() {
    //IO::SPI(120000, 1, false, false);
    IO::SoftUART(1, 2);

}

void loop() {

}