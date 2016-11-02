/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ID_xxLA.cpp
 * Author: Developer
 * 
 * Created on October 31, 2016, 6:58 PM
 */

#include "ID_xxLA.h"

ID_xxLA::ID_xxLA(int RXPin, int TXPin, int tagInRangePin) :
serial(RXPin, TXPin), tagInRangePin(tagInRangePin)
{
    serial.begin(9600);
}

void ID_xxLA::getID(char* buffer) {
    read(buffer);
}

void ID_xxLA::read(char* buffer) {
    if (digitalRead(tagInRangePin)) {
        serial.read();
        serial.read();
        serial.readBytes(buffer, dataLen);
        serial.readBytes(readChecksum, checksumLen);
        serial.read();
        serial.read();
        serial.read();
        serial.read();
        serial.read();
    }
}