/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ID_xxLA.h
 * Author: Developer
 *
 * Created on October 31, 2016, 6:58 PM
 */

#ifndef ID_XXLA_H
#define ID_XXLA_H
#include <Arduino.h>
#include <SoftwareSerial.h>

class ID_xxLA {
    const unsigned char checksumLen = 2;
    const unsigned char dataLen = 10;
public:
    ID_xxLA(int RXPin, int TXPin, int tagInRangePin);
    ID_xxLA(const ID_xxLA& orig) = delete;
    virtual ~ID_xxLA(){};
    void getID(char* buffer);
private:
    void read(char* buffer);
private:


    SoftwareSerial serial;
    int tagInRangePin;
    unsigned char readChecksum[2];
};

#endif /* ID_XXLA_H */

