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

#include <vector>
#include <memory>
#include <functional>

namespace IO {
    
    /**
     * @brief This class is used to provide other system functionalities.
     * Provides a singleton method that is used to ensure any IO initialization that needs to occur should occur.
     */
    class SystemBase {
    public:
        static void init();
        static void wait_ms(uint32_t ms);
        static void wait_us(uint32_t us);
    };

    // As much as I would like these to be INPUT and OUTPUT, the Arduino library has these 
    // terms as macro definitions, which interfere with those keywords.
    enum PinMode {IN, OUT};
    
    /**
     * @brief This class is used to allow the DWM1000 class to communicate with 
     * low-level GPIO.
     */
    class PinBase {
    public:
        virtual ~PinBase() = 0;
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
    
    // I would like to spell out HIGH, LOW, CHANGE, but macro definitions ruin that.
    enum InterruptPinMode {HI, LO, CHNG, RSING, FLING};
    class InterruptPinBase {
    public:
        virtual ~InterruptPinBase(){};
        virtual void setIsEnabled(bool b) = 0;
        virtual bool getIsEnabled() = 0;
        virtual void setMode(InterruptPinMode pinmode);
        virtual void setISR(std::function<void()> f) = 0;
    };
    
        /**
     * @brief This interface class is used in the DWM1000 library to access the 
     * SPI communications bus.
     * @remark Any combination of clock polarity and phase is supported.
     */    
    class SPIBase {
    public:
        virtual ~SPIBase(){};
        /**
         * @brief Transfer a single byte of data.
         * @param data The byte to be transfered.
         * @return The byte read back.
         */
        virtual unsigned char transfer(unsigned char data) = 0;
        /**
         * @brief Transfer an array (technically a vector) of data.
         * @param data The data to be transferred.
         * @return The data read back from the transfer.
         */
        virtual std::vector<unsigned char> transfer(const std::vector<unsigned char> &data) = 0;
        /**
         * @brief Ground the SPI lines to minimize power consumption, and to 
         * ensure that the line is not in use.
         */
        virtual void disable() = 0;        
    };
    
    /**
     @brief This interface class is used to provide UART/Serial communications 
     * support.
     */
    class UARTBase {
    public:
        virtual ~UARTBase(){};
        /// Returns the number of bytes available to read.
        virtual uint32_t available() = 0;
        /// Read one byte from UART.
        virtual unsigned char read() = 0;
        /// Read "length" bytes from UART.
        virtual std::vector<unsigned char> read(uint32_t length) = 0;
        /// Read from UART until the "terminator" byte is detected.
        virtual std::vector<unsigned char> read(unsigned char terminator, uint32_t length = UINT32_MAX) = 0;
        /// Write a byte to UART.
        virtual void write(unsigned char data) = 0;
        /// Write many bytes to UART.
        virtual void write(const std::vector<unsigned char> &data) = 0;
    };
    
}

#endif /* IO_H */

