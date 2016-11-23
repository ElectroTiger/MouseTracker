/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IORpi.h
 * Author: Developer
 *
 * Created on October 16, 2016, 5:17 PM
 */

// This directive allows allows this file to compile only if WiringPi exists.
#pragma once

//#if __has_include("wiringPi.h")
#include "IO.h"
#include "wiringPi.h"
#include "wiringPiSPI.h"
#include <exception>



namespace IO {
    class SPI : public IO::SPIBase {
        /**
         * 
         * @param freq The SPI bus frequency, which must be between 500,000 Hz and 32,000,000 Hz.
         * @param isMSBFirst Whether the MSB or the LSB is transmitted first.
         * @param CPOL The Clock polarity.
         * @param CPHA The Clock phase.
         * @param channel Raspberry-Pi Specific: Which SPI channel to use.
         * @return The RpiSPI object.
         */
        SPI(uint32_t freq, bool isMSBFirst, bool CPOL, bool CPHA, bool channel = 0)
        : channel(channel) {
            if (freq < 500000 || freq > 32000000) {
                throw new std::invalid_argument("Frequency must be between 500,000 and 32,000,000");
            }
            
            // We convert the CPOL and CPHA arguments into the "mode" parameter.
            uint8_t mode =
                    (CPOL == 0 && CPHA == 0) ? 0 :
                    (CPOL == 0 && CPHA == 1) ? 1 :
                    (CPOL == 1 && CPHA == 0) ? 2 :
                    3;
                
            int32_t fileDescriptor = wiringPiSPISetupMode(channel, freq, mode);
            if (fileDescriptor == -1) {
                throw new std::runtime_error("Opening wiringPiSPI Channel Failed.");
            }
        }
        
        virtual ~SPI();
        
        /**
         * @brief Transfer a single byte of data.
         * @param data The byte to be transfered.
         * @return The byte read back.
         */
        byte transfer(byte data) override{
            wiringPiSPIDataRW(channel, reinterpret_cast<byte*>(&data), 1);
        }
        /**
         * @brief Transfer an array (technically a vector) of data.
         * @param data The data to be transferred.
         * @return The data read back from the transfer.
         */
        std::vector<byte> transfer(const std::vector<byte> &data) override {
            std::vector<byte> dataCopy(data);
            wiringPiSPIDataRW(channel, reinterpret_cast<byte*>(dataCopy.data()), data.size());
        }
        /**
         * @brief Ground the SPI lines to minimize power consumption, and to 
         * ensure that the line is not in use.
         */
        // TODO!!
        void disable() override{
            
        }  
        
        private:
        bool channel;
    };
    
    class Pin : IO::PinBase {
    public:
        Pin(int32_t pin, PinMode mode)
        : pin(pin) {
            pinMode(pin, static_cast<int32_t>(mode));
        }
        virtual ~Pin();
        /**
         * Set the mode of the GPIO as input or output.
         * @param mode The mode to set it to.
         */
        void setMode(PinMode mode) final {
            pinMode(pin, static_cast<int32_t>(mode));
        }
        /// Read from the pin.
        bool read() final {
            return digitalRead(pin);
        }
        /// Write to the pin.
        void write(bool b) final {
            digitalWrite(pin, b);
        }
    private:
        int32_t pin;
    };
    
    class System{
        static void wait_ms(uint32_t ms) {
            delay(ms);
        }
        static void wait_us(uint32_t us) {
            delayMicroseconds(us);
        }
    };
}
//#endif
#pragma once



