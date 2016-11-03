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
serial(RXPin, TXPin), tagInRangePin(tagInRangePin) {
    serial.begin(9600);
}

bool ID_xxLA::getID(char* buffer) {
    if (digitalRead(tagInRangePin)) {
        
        // Two start bytes..
        serial.read();
        serial.read();
        
        // Read 5 bytes
        serial.readBytes(buffer, dataLen);
        readChecksum = serial.read();
        
        // One line feed, one newline, then three end bytes..
        serial.read();
        serial.read();
        serial.read();
        serial.read();
        serial.read();
        
        // Compute the checksum.
        char dataXOR = buffer[0];
        for (int i = 1; i < dataLen; i++) {
            dataXOR ^= buffer[i];
        } 
        // Null terminate the buffer.
        buffer[dataLen] = '\0';
        if (dataXOR == readChecksum) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}