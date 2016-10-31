#include <Arduino.h>
#include "../IO/IOArduino.h"
#include <functional>
#include "../ID-xxLA/ID_xxLA.h"
#include <memory>

extern HardwareSerial Serial;

void setup() {
    //IO::SPI(120000, 1, false, false);
    std::unique_ptr<IO::SoftUART> pUART(new IO::SoftUART(5, 7));
    std::unique_ptr<IO::InterruptPin> pUARTInterPin(new IO::InterruptPin(8));
    ID_xxLA rfid(std::move(pUART), std::move(pUARTInterPin));
    //std::un;

}

void loop() {

}