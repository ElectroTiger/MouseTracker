/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IOArduino.h
 * Author: Developer
 *
 * Created on October 16, 2016, 5:14 PM
 */

// This define only compiles for Arduino systems.

#ifndef IOARDUINO_H
#define IOARDUINO_H
//#ifdef ARDUINO
#include "IO.h"
#include <Arduino.h>
#include <avr/../../../libraries/SPI/src/SPI.h>
#include <avr/../../../libraries/SoftwareSerial/src/SoftwareSerial.h>

extern HardwareSerial Serial;

namespace IO {

    /**
     * @brief This class is used to provide other system functionalities.
     * Provides a singleton method that is used to ensure any IO initialization that needs to occur should occur.
     */
    class System {
    public:

        static void init() {
            // Arduino has no init; this is empty.
        }

        void wait_ms(uint32_t ms) {
            delay(ms);
        }

        static void wait_us(uint32_t us) {
            delayMicroseconds(us);
        }
    };

    /**
     * @brief This class is used to allow the DWM1000 class to communicate with 
     * low-level GPIO.
     */
    class Pin : public PinBase {
    public:

        Pin(uint32_t pin, PinMode mode)
        : pin(pin), mode(mode) {
            setMode(mode);
        }
        virtual ~Pin();

        /**
         * Set the mode of the GPIO as input or output.
         * @param mode The mode to set it to.
         */
        void setMode(PinMode mode) final {
            pinMode(pin, mode);
        }
        /// Read from the pin.

        bool read() final {
            return digitalRead(pin);
        }
        /// Write to the pin.

        void write(bool b) final {
            if (mode == OUT) {
                digitalWrite(pin, b ? HIGH : LOW);
            }
        }
    private:
        uint32_t pin;
        PinMode mode;
    };

    class InterruptPin : public InterruptPinBase {
    public:
        InterruptPin(uint32_t pin) {
            
        }
        virtual ~InterruptPin() {}
        virtual void setIsEnabled(bool b) final {
            isEnabled = b;
        }
        virtual bool getIsEnabled() final {
            return isEnabled;
        }
        virtual void setMode(InterruptPinMode pinmode) final {
            
        }
        virtual void setISR(boost::function<void()> f) final {
            
        }
    private:
        bool isEnabled;
    };

    /**
     * @brief This interface class is used in the DWM1000 library to access the 
     * SPI communications bus.
     * @remark Any combination of clock polarity and phase is supported.
     */
    class SPI : public SPIBase {
    public:

        SPI(uint32_t freq, bool isMSBFirst, bool cpol, bool cpha)
        : spiSettings(
        freq,
        isMSBFirst ? MSBFIRST : LSBFIRST,
        (cpol == 0 && cpha == 0) ? SPI_MODE0 :
        (cpol == 0 && cpha == 1) ? SPI_MODE1 :
        (cpol == 1 && cpha == 0) ? SPI_MODE2 : SPI_MODE3
        )
 {
            //SPIClass::begin();
        }

        virtual ~SPI() {

        }

        /**
         * @brief Transfer a single byte of data.
         * @param data The byte to be transfered.
         * @return The byte read back.
         */
        unsigned char transfer(unsigned char data) final {
            return SPIClass::transfer(data);
        }

        /**
         * @brief Transfer an array (technically a vector) of data.
         * @param data The data to be transferred.
         * @return The data read back from the transfer.
         */
        std::vector<unsigned char> transfer(const std::vector<unsigned char> &data) final {
            std::vector<unsigned char> dataCopy = data;
            SPIClass::transfer(&dataCopy.front(), dataCopy.size());
            return dataCopy;
        }

        /**
         * @brief Ground the SPI lines to minimize power consumption, and to 
         * ensure that the line is not in use.
         */
        void disable() final {

        }
    private:
        SPISettings spiSettings;
    };

//    class UART : public UARTBase {
//    public:
//
//        UART(uint32_t RXPin, uint32_t TXPin) {};
//        virtual ~UART();
//        /// Read one byte from UART.
//
//        unsigned char read() final {
//            return HardwareSerial.read();
//        }
//        /// Read "length" bytes from UART.
//        std::vector<unsigned char> read(uint32_t length) final {
//                        std::vector<unsigned char> buffer(length);
//            HardwareSerial.readBytes(&buffer.front, length);
//            return buffer;
//        }
//        /// Read from UART until the "terminator" byte is detected.
//        std::vector<unsigned char> read(unsigned char terminator) final {
//                        std::vector<unsigned char> buffer;
//            int32_t c = HardwareSerial.read();
//            while(c != -1) {
//                buffer.push_back(reinterpret_cast<unsigned char>(c));
//            }
//            return buffer;
//        }
//        /// Write a byte to UART.
//        void write(unsigned char data) final;
//        /// Write many bytes to UART.
//        void write(std::vector<unsigned char> data) final;
//    };

    class SoftUART : public UARTBase {
    public:

        SoftUART(uint8_t RXPin, uint8_t TXPin) :
        softSerialObj(RXPin, TXPin, false) {

        }
        virtual ~SoftUART(){};
        /// Read one byte from UART.
        
        virtual uint32_t available() final {
            return softSerialObj.available();
        }

        virtual unsigned char read() final {
            return softSerialObj.read();
        }
        /// Read "length" bytes from UART.
        virtual std::vector<unsigned char> read(uint32_t length) final {
            std::vector<unsigned char> buffer;
            for(uint32_t i = 0; i < length; i++) {
                buffer.push_back(softSerialObj.read());
            }
            return buffer;
        }
        /// Read from UART until the "terminator" byte is detected.
        virtual std::vector<unsigned char> read(unsigned char terminator, uint32_t length = UINT32_MAX) final {
            std::vector<unsigned char> buffer;
            int32_t c = softSerialObj.read();
            for(uint32_t i = 0; (i < length) && (c != -1); i++, c = softSerialObj.read()) {
                buffer.push_back(static_cast<unsigned char>(c));
            }
            return buffer;
        }
        /// Write a byte to UART.
        virtual void write(unsigned char data) final {
            softSerialObj.write(data);
        }
        /// Write many bytes to UART.
        virtual void write(const std::vector<unsigned char> &data) final {
            softSerialObj.write(&data.front(), data.size());
        }

    private:
        SoftwareSerial softSerialObj;
    };

}

//#endif
#endif /* IOARDUINO_H */