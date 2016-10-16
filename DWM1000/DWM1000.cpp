/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * @file GWM1000.cpp
 * @author Weimen Li
 * @date October 5th, 2016
 */


#include "DWM1000.h"


DWM1000::DWM1000(std::unique_ptr<IO::SPI> spi, 
            std::unique_ptr<IO::Pin> EXTnPin, 
            std::unique_ptr<IO::Pin> wakeupPin, 
            std::unique_ptr<IO::Pin> RSTnPin, 
            std::unique_ptr<IO::InterruptPin> IRQPin) :
spi(std::move(spi)), EXTnPin(std::move(EXTnPin)), wakeupPin(std::move(wakeupPin)), RSTnPin(std::move(RSTnPin)), IRQPin(std::move(IRQPin)) 
{
    
}

DWM1000::DWM1000(const DWM1000& orig) {
}

DWM1000::~DWM1000() {
}

/// Transmit the data contained in the vector data.
void DWM1000::transmit(const std::vector<char> &data) {
    write(TX_BUFFER, data);
}
/// Set the callback function upon receiving data from the DWM1000, which must return void and take an rvalue reference to a vector of chars. 
void DWM1000::setRXcallback(std::function<void(std::vector<char>&&)> cb) {
    rxCallback = cb;
}

/// Set the state of the DWM1000 to be asleep or not.
void DWM1000::setIsAsleep(const bool b) {
    
}
/// Get the state of whether the DWM1000 is asleep or not.
bool DWM1000::getIsAsleep() {
    
}


std::vector<char> DWM1000::read(const DWM1000::Register &regFileID, const uint16_t length, const uint32_t subaddr) {
    std::vector<char> transmission;
    if (subaddr == 0) {
        transmission.push_back(0b10000000 | regFileID.ID);
    } else if (subaddr < 128) {
        transmission.push_back(0b11000000 | regFileID.ID);
        transmission.push_back(0b00000000 | subaddr);
    } else {
        transmission.push_back(0b11000000 | regFileID.ID);
        transmission.push_back(0b10000000 | (subaddr & 0b1111111));
        transmission.push_back(subaddr & ~(0b1111111));
        }
    std::vector<char> response = spi->transfer(std::move(transmission));
    return response;
}

void DWM1000::write(const DWM1000::Register &regFileID, const std::vector<char> &data, const uint32_t subaddr) {
    
    std::vector<char> transmission;
    if (subaddr == 0) {
        transmission.push_back(0b10000000 | regFileID.ID);
    } else if (subaddr < 128) {
        transmission.push_back(0b11000000 | regFileID.ID);
        transmission.push_back(0b00000000 | subaddr);
    } else {
        transmission.push_back(0b11000000 | regFileID.ID);
        transmission.push_back(0b10000000 | (subaddr & 0b1111111));
        transmission.push_back(subaddr & ~(0b1111111));
        }
    transmission.insert(transmission.end(), data.begin(), data.end());
    spi->transfer(std::move(transmission));
}

