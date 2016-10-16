/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO.h
 * Author: Developer
 *
 * Created on October 7, 2016, 8:50 PM
 
 */

#ifndef IO_H
#define IO_H

#include <config/_avr.h>
#include <vector>
#include <memory>
#include <functional>

namespace IO {
    /**
     * @brief This interface class is used in the DWM1000 library to access the 
     * SPI communications bus.
     * @remark Any combination of clock polarity and phase is supported.
     */    
    class SPI {
    public:
        virtual ~SPI() = 0;
        SPI(const SPI&) = delete;
        SPI& operator=(const SPI&) = delete;
        /**
         * @brief Transfer a single byte of data.
         * @param data The byte to be transfered.
         * @return The byte read back.
         */
        virtual char transfer(char data) = 0;
        /**
         * @brief Transfer an array (technically a vector) of data.
         * @param data The data to be transferred.
         * @return The data read back from the transfer.
         */
        virtual std::vector<char> transfer(const std::vector<char> &data) = 0;
        virtual std::vector<char> transfer(std::vector<char> &&data) = 0;
        /**
         * @brief Ground the SPI lines to minimize power consumption, and to 
         * ensure that the line is not in use.
         */
        virtual void disable() = 0;        
    };
    
    enum PinMode {INPUT, OUTPUT};
    
    /**
     * @brief This class is used to allow the DWM1000 class to communicate with 
     * low-level GPIO.
     */
    class Pin {
    public:
        virtual ~Pin() = 0;
        /**
         * Set the mode of the GPIO as input or output.
         * @param mode The mode to set it to.
         */
        virtual void setMode(PinMode mode);
        /// Read from the pin.
        virtual bool read();
        /// Write to the pin.
        virtual void write(bool);
    };
    
    enum InterruptPinMode {HIGH, LOW, CHANGE, RISING, FALLING};
    class InterruptPin {
    public:
        virtual ~InterruptPin() = 0;
        virtual void setIsEnabled(bool b) = 0;
        virtual bool getIsEnabled() = 0;
        virtual void setMode(InterruptPinMode pinmode);
        virtual void setISR(std::function<void()> f);
    };
}

#endif /* IO_H */

