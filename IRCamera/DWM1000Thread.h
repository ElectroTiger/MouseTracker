/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DWM1000Thread.h
 * Author: Developer
 *
 * Created on November 24, 2016, 10:39 PM
 */

#ifndef DWM1000THREAD_H
#define DWM1000THREAD_H
#include "deca_device_api.h"
#include "wiringPi.h"
#include "wiringPiSPI.h"

// TODO; Research interrupt functionality. 

class DWM1000Thread {
public:
    /// Returns the singleton instance of DWM1000Thread. 
    static DWM1000Thread* Instance();
    /// Starts execution of the thread.
    void start();
    /// Stops execution of the thread.
    void stop();
    
    
private:
    DWM1000Thread();
    DWM1000Thread(const DWM1000Thread& orig) = delete; // No copies allowed. 
    virtual ~DWM1000Thread();
    static DWM1000Thread* m_pInstance;
    void threadFunc();
    
public:
    static const int SPI_channel = 0; // Pi's SPI Channels. may be 0 or 1.

public:
    // TODO: Think about actual default config options.
    dwt_config_t default_dwt_config {
        1, // Channel
        DWT_PRF_16M, // Pulse repetition frequency
                DWT_PLEN_128, // txPreambleLength
                128, // Acquisition Chunk Size
                8, // TX preamble code
                8, // RX preamble code
                false, // Boolean
                DWT_BR_110K, // Data rate
                0x0, // PHR mode
                8 // sfd timeout value
    };
};

#endif /* DWM1000THREAD_H */

