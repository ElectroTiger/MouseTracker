/* 
 * File:   ID_xxLA.h
 * Author: Weimen Li
 *
 * Created on October 31, 2016, 6:58 PM
 * Header file for the ID_xxLA Library written to interface with the ID_xxLA chip.
 */

#ifndef ID_XXLA_H
#define ID_XXLA_H
#include <Arduino.h>
#include <SoftwareSerial.h>

class ID_xxLA {
    // User interface code: Relevant to you!
public:
    /// Create an instance of the ID_xxLA class. TXPin is unused, and RXPin and tagInRangePin may be connected to any digital IO.
    ID_xxLA(int RXPin, int TXPin, int tagInRangePin);

    /// Read the 10-byte ID from the reader as a null-terminated char array, as well as the checksum if desired.
    /// @arg buffer An 11-byte long array of chars, like char data[11]; 
    /// @arg checksum Optional: A 3-byte long array of chars, like char data[3];
    /// @returns true if the read is successful, and false if it is unsuccessful.
    /// Unsuccessful read may occur beacuse tag is too far away, or because checksum did not match.
    bool getID(char* buffer, char* checksum = 0);
    /// Read the 2-byte checksum that was last acquired.
    /// So: Pass in a 3-byte long array of chars, like char checksum[3];
    void getChecksum(char* checksum);
    /// Converts an ASCII character, '0' to '9' and 'A' to 'F' into its binary representation 0000 to 1111
    static unsigned char charToBinary(char c);
    // Boilerplate code: You can ignore this.
public:
    ID_xxLA(const ID_xxLA& orig) = delete;
    virtual ~ID_xxLA(){};
private:
    SoftwareSerial serial;
    int tagInRangePin;
};

#endif /* ID_XXLA_H */

