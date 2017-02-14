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

#include "DWM1000Thread.h"
#include "Utilities.h"

DWM1000Thread* DWM1000Thread::Instance() {
    if (!m_pInstance) {
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
        auto fatalError = [terminate](auto errorMsg) {
            std::cerr << Utilities::getCurrentTime() << __FILE__ 
                    << errorMsg << std::endl;
            terminate = true;
        };
        // Initialize thread.
        
        // Initialize SPI Bus. 
        if(wiringPiSPISetup(SPI_channel, 32000000) == -1) {
            fatalError(strerror(errno));
        }
        
        
        
        
        // Reset the DWM1000 to clear its previous settings
        dwt_softreset();
        // Initialize the DWM1000, don't load LDE microcode from the ROM.
        if(!dwt_initialise(DWT_LOADNONE)) {
            fatalError("Device initialization failed.");
        }
        // Configure options for it.
        dwt_configure(default_dwt_config);




        // Begin thread execution.
        while(!terminate) {

        }
        // De-initialize thread.
    }
}



DWM1000Thread::DWM1000Thread() {
}

DWM1000Thread::DWM1000Thread(const DWM1000Thread& orig) {
}

DWM1000Thread::~DWM1000Thread() {
}

// This section contains the implementation of platform - specific functions 
// starting line 1618 of deca_device_api.h

writetospi(uint16 headerLength, 
        const uint8* headerBuffer, 
        uint32 bodylength, 
        const uint8* bodyBuffer) {
    wiringPiSPIDataRW(DWM1000Thread::SPI_channel, headerBuffer, headerLength);
    wiringPiSPIDataRW(DWM1000Thread::SPI_channel, bodyBuffer, bodyLength);
    
}
