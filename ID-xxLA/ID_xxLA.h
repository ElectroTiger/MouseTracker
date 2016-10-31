/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * @author Weimen Li
 * @date October 29th, 2016
 * @class ID_xxLA
 * The ID_xxLA class is a driver for the ID-2LA, ID-12LA, and ID-20LA RFID
 * reader modules.
 */

#ifndef ID_XXLA_H
#define ID_XXLA_H
#include "../IO/IO.h"

class ID_xxLA {
public:
    ID_xxLA(std::unique_ptr<IO::UARTBase> pUART, std::unique_ptr<IO::InterruptPinBase> pInterPin);
    ID_xxLA(const ID_xxLA& orig);
    virtual ~ID_xxLA();
    
    
private:
    std::unique_ptr<IO::UARTBase> pUART;
    std::unique_ptr<IO::InterruptPinBase> pInterPin;

};

#endif /* ID_XXLA_H */

