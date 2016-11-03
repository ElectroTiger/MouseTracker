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
    // User interface code: Relevant to you!
public:
	/// Create an instance of the ID_xxLA class.
    ID_xxLA(int RXPin, int TXPin, int tagInRangePin);

    /// Read the 5-byte ID from the reader as a null-terminated char array.
    /// So: Pass in an 6-byte long array of chars, like char data[6];
    /// @returns true if the read is successful, and false if it is unsuccessful.
    /// Unsuccessful read may occur beacuse tag is too far away, or because checksum did not match.
    bool getID(char* buffer);

    // Boilerplate code: You can ignore this.
public:
	ID_xxLA(const ID_xxLA& orig) = delete;
    virtual ~ID_xxLA(){};
    const unsigned char dataLen = 5;
private:
    SoftwareSerial serial;
    int tagInRangePin;
    unsigned char readChecksum;
};

#endif /* ID_XXLA_H */

