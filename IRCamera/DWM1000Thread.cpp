/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DWM1000Thread.cpp
 * Author: Developer
 * 
 * Created on November 24, 2016, 10:39 PM
 */

#include <stdexcept>
#include <thread>
#include <string>
#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <cstring>

#include "DWM1000Thread.h"
#include "Utilities.h"

DWM1000Thread* DWM1000Thread::m_pInstance = nullptr;

DWM1000Thread* DWM1000Thread::Instance() {
    if (m_pInstance == nullptr) {
        m_pInstance = new DWM1000Thread();
    }
    return m_pInstance;
}

void DWM1000Thread::start() {

}

void DWM1000Thread::stop() {

}

// Primary thread function which is executed on start().
void DWM1000Thread::threadFunc() {
    bool terminate = false;
    while(!terminate) {
        // Setup lambda function to handle errors. 
        auto fatalError = [&terminate](auto errorMsg) {
            std::cerr << Utilities::getCurrentTime() << __FILE__ 
                    << errorMsg << std::endl;
            terminate = true;
        };
        // Initialize thread.
        
        // Initialize SPI Bus. 
        if(wiringPiSPISetup(SPI_channel, 32000000) == -1) {
            fatalError(std::strerror(errno));
        }
        
        
        
        
        // Reset the DWM1000 to clear its previous settings
        dwt_softreset();
        // Initialize the DWM1000, don't load LDE microcode from the ROM.
        if(!dwt_initialise(DWT_LOADNONE)) {
            fatalError("Device initialization failed.");
        }
        // Configure options for it.
        dwt_configure(&default_dwt_config);




        // Begin thread execution.
        while(!terminate) {

        }
        // De-initialize thread.
    }
}



DWM1000Thread::DWM1000Thread() {
}

//DWM1000Thread::DWM1000Thread(const DWM1000Thread& orig) {
//}

DWM1000Thread::~DWM1000Thread() {
}

// This section contains the implementation of platform - specific functions 
// starting line 1618 of deca_device_api.h

int writetospi(uint16 headerLength, 
        const uint8* headerBuffer, 
        uint32 bodyLength, 
        const uint8* bodyBuffer) {
    // Unfortunately, since writetospi mandates const-ness, but wiringPi does 
    // not, we are forced to pointlessly make a copy of the buffers.
    
    errno = 0; // Reset errno value to monitor for SPI errors. 
    
    uint8 *headerBufferCopy = new uint8[headerLength];
    std::memcpy(headerBufferCopy, headerBuffer, headerLength);
    wiringPiSPIDataRW(DWM1000Thread::SPI_channel, headerBufferCopy, headerLength);
    delete [] headerBufferCopy;
    if (errno != 0) return DWT_ERROR;
    
    uint8 *bodyBufferCopy = new uint8[bodyLength];
    std::memcpy(bodyBufferCopy, bodyBuffer, bodyLength);
    wiringPiSPIDataRW(DWM1000Thread::SPI_channel, bodyBufferCopy, bodyLength);
    delete [] bodyBufferCopy;
    if (errno != 0) return DWT_ERROR;
    
    return DWT_SUCCESS;
}

int readfromspi(uint16 headerLength, 
        const uint8* headerBuffer, 
        uint32 readlength, 
        uint8* readBuffer) {
    errno = 0; // Reset errno value to monitor for SPI errors.
    //wiringPiSPIDataRW(DWM1000Thread::SPI_channel, headerBuffer, headerLength);
    uint8 *headerBufferCopy = new uint8[headerLength];
    std::memcpy(headerBufferCopy, headerBuffer, headerLength);
    wiringPiSPIDataRW(DWM1000Thread::SPI_channel, headerBufferCopy, headerLength);
    delete [] headerBufferCopy;
    if (errno != 0) return DWT_ERROR;
    
    wiringPiSPIDataRW(DWM1000Thread::SPI_channel, readBuffer, readlength);
    if (errno != 0) return DWT_ERROR;
    return DWT_SUCCESS;
}

decaIrqStatus_t decamutexon() {
    // TODO
    return 1;
}

void decamutexoff(decaIrqStatus_t s) {
    // TODO
}

void deca_sleep(unsigned int time_ms) {
    // TODO: delay is dangerous! What will other threads do when delay is called?
    delay(time_ms);
}


