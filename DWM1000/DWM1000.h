/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * @file DWM1000.cpp
 * @author Weimen Li
 * @date October 5th, 2016
 * @class DWM1000 The DWM1000 Class encapsulates the functionality of the
 * DWM1000 Ultra-Wide-Band transceiver.
 */

#ifndef DWM1000_H
#define DWM1000_H

#include "../IO/IO.h"
#include <functional>
#include <vector>
#include <memory>

class DWM1000 {
public:
    DWM1000(std::unique_ptr<IO::SPI> spi,
            std::unique_ptr<IO::Pin> EXTnPin,
            std::unique_ptr<IO::Pin> wakeupPin,
            std::unique_ptr<IO::Pin> RSTnPin,
            std::unique_ptr<IO::InterruptPin> IRQPin);
    DWM1000(const DWM1000& orig);
    virtual ~DWM1000();

    /// Transmit the data contained in the vector data.
    void transmit(const std::vector<char> &data);
    /// Set the callback function upon receiving data from the DWM1000, which must return void and take an rvalue reference to a vector of chars. 
    void setRXcallback(std::function<void(std::vector<char>&&)>);

    /// Set the state of the DWM1000 to be asleep or not.
    void setIsAsleep(const bool b);
    /// Get the state of whether the DWM1000 is asleep or not.
    bool getIsAsleep();
    
public:
    
/// The register subclass encapsulates bounds information about the register, including its ID, length, and read/write access.
    struct Register {

        enum Type {
            RO, WO, RW
        };

        enum ID : uint8_t {
            DEV_ID = 0,
            EUI = 1,
            PANADR = 3,
            SYS_CFG = 4,
            SYS_TIME = 6,
            TX_FCTRL = 8,
            TX_BUFFER = 9,
            DX_TIME = 0x0A,
            RX_FWTO = 0x0C,
            SYS_CTRL = 0x0D,
            SYS_MASK = 0x0E,
            SYS_STATUS = 0x0F,
            RX_FINFO = 0x10,
            RX_BUFFER = 0x11,
            RX_FQUAL = 0x12,
            RX_TTCKI = 0x13,
            RX_TTCKO = 0x14,
            RX_TIME = 0x15,
            TX_TIME = 0x17,
            TX_ANTD = 0x18,
            SYS_STATE = 0X19,
            ACK_RESP_T = 0X1A,
            RX_SNIFF = 0X1D,
            TX_POWER = 0X1E,
            CHAN_CTRL = 0X1F,
            USR_SFD = 0X21,
            AGC_CTRL = 0X23,
            EXT_SYNC = 0X24,
            ACC_MEM = 0X25,
            GPIO_CTRL = 0X26,
            DRX_CONF = 0X27,
            RF_CONF = 0X28,
            TX_CAL = 0X2a,
            FS_CTRL = 0X2B,
            AON = 0X2C,
            OTP_IF = 0X2D,
            LDE_CTRL = 0X2E,
            DIG_DIAG = 0X2F,
            PMSC = 0X36
        };


        // Instance variables:
        uint8_t ID;
        uint16_t length;
        Type type;
    };


    const Register
    DEV_ID{0, 4, Register::Type::RO},
    EUI{1, 8, Register::Type::RW},
    PANADR{3, 4, Register::Type::RW},
    SYS_CFG{4, 4, Register::Type::RW},
    SYS_TIME{6, 5, Register::Type::RO},
    TX_FCTRL{8, 5, Register::Type::RW},
    TX_BUFFER{9, 1024, Register::Type::WO},
    DX_TIME{0x0A, 5, Register::Type::RW},
    RX_FWTO{0x0C, 2, Register::Type::RW},
    SYS_CTRL{0x0D, 4, Register::Type::RW},
    SYS_MASK{0x0E, 4, Register::Type::RW},
    SYS_STATUS{0x0F, 5, Register::Type::RW},
    RX_FINFO{0x10, 4, Register::Type::RO},
    RX_BUFFER{0x11, 1024, Register::Type::RO},
    RX_FQUAL{0x12, 8, Register::Type::RO},
    RX_TTCKI{0x13, 4, Register::Type::RO},
    RX_TTCKO{0x14, 5, Register::Type::RO},
    RX_TIME{0x15, 14, Register::Type::RO},
    TX_TIME{0x17, 10, Register::Type::RO},
    TX_ANTD{0x18, 2, Register::Type::RW},
    SYS_STATE{0X19, 5, Register::Type::RO},
    ACK_RESP_T{0X1A, 4, Register::Type::RW},
    RX_SNIFF{0X1D, 4, Register::Type::RW},
    TX_POWER{0X1E, 4, Register::Type::RW},
    CHAN_CTRL{0X1F, 4, Register::Type::RW},
    USR_SFD{0X21, 41, Register::Type::RW},
    AGC_CTRL{0X23, 32, Register::Type::RW},
    EXT_SYNC{0X24, 12, Register::Type::RW},
    ACC_MEM{0X25, 4064, Register::Type::RO},
    GPIO_CTRL{0X26, 44, Register::Type::RW},
    DRX_CONF{0X27, 44, Register::Type::RW},
    RF_CONF{0X28, 58, Register::Type::RW},
    TX_CAL{0X2A, 52, Register::Type::RW},
    FS_CTRL{0X2B, 21, Register::Type::RW},
    AON{0X2C, 12, Register::Type::RW},
    OTP_IF{0X2D, 18, Register::Type::RW},
    LDE_CTRL{0X2E, 1, Register::Type::RW},
    DIG_DIAG{0X2F, 41, Register::Type::RW},
    PMSC{0X36, 48, Register::Type::RW};


    // IO Operations
protected:
    /**
     * @brief Read 'length' bytes from regFileID starting at index 0.
     * @param regFileID The address of the register file to be read from.
     * @warning regFileID must NEVER exceed 63 (0x3F), or undefined behavior will result.
     * @param length The number of bytes to read.
     * @param subaddr The index of the register within the regfile to read from.
     * @warning The subaddr + length should NEVER exceed the published length of the selected register file. 
     * @return A vector containing the read data.
     */
    std::vector<char> read(const DWM1000::Register &regFileID, const uint16_t length, const uint32_t subaddr = 0);
    /**
     * @brief Write 'data' to regFileID starting at index subaddr, which defaults to zero.
     * @param regFileID The register file ID to write to.
     * @param data The data to write.
     * @param subaddr The index within the register file to write to.
     */
    void write(const DWM1000::Register &regFileID, const std::vector<char> &data, const uint32_t subaddr = 0);

    // Command routines
protected:




    // Pins
private:
    /// The SPI object that the DWM1000 module uses to access the SPI bus.
    std::unique_ptr<IO::SPI> spi;
    /// Input from DWM1000: External device enable: Is asserted high during DWM1000 wakeup process, and held high until the DWM1000 enters sleep mode.
    std::unique_ptr<IO::Pin> EXTnPin;
    /// Output to DWM1000: When asserted to active high, this pin brings the DW1000 out of sleep or deepsleep. Should be tied to ground if not in use.
    std::unique_ptr<IO::Pin> wakeupPin;
    /// Output to DWM1000: May be pulled low to reset the DWM1000. Otherwise, it should be left floating. 
    std::unique_ptr<IO::Pin> RSTnPin;
    /// Input from DWM1000: Interrupt line which is asserted under conditions which may be programmed.
    std::unique_ptr<IO::InterruptPin> IRQPin;

    // Instance variables
public:
    std::function<void(std::vector<char>&&)> rxCallback;
    
    
};

#endif /* DWM1000_H */

