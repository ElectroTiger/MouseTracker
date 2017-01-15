/* 
 * File:   ID_xxLA.cpp
 * Author: Weimen Li
 * 
 * Created on October 31, 2016, 6:58 PM
 */

#include "ID_xxLA.h"

//#define DEBUG

#ifdef DEBUG
#define debugPrint(s) do{Serial.println(s); Serial.flush();} while(0)
#else
#define debugPrint(s) do{} while(0);
#endif

ID_xxLA::ID_xxLA(int RXPin, int TXPin, int tagInRangePin) :
serial(RXPin, TXPin), tagInRangePin(tagInRangePin) {
    serial.begin(9600);
}

bool ID_xxLA::getID(char *buffer, char *checksum) {
    if ((digitalRead(tagInRangePin)) && (serial.available() == 16)) { // Branch where tag is in range.
        debugPrint("In tagInRangePin branch.");
        bool isValid = true;

        // If checksum is null, assign a temporary one to use.
        char checksumTemp[3];
        if (checksum == 0) {
            checksum = checksumTemp;
        }

        // Data transmission begins with 2.
        debugPrint("Reading first character, which should be 2...");
        if (serial.read() != 2) isValid = false;
        if (!isValid) return isValid;
        
        // Read 10 bytes, which constitute the ASCII characters in the tag.
        debugPrint("Reading 10 tag characters...");
        serial.readBytes(buffer, 10);
        // Read the last two bytes, which constitutes the checksum.
        debugPrint("Reading 2 checksum characters...");
        checksum[0] = serial.read();
        checksum[1] = serial.read();

        // One carriage return, one newline, then 3.
        debugPrint("Reading CR...");
        if (serial.read() != '\r') isValid = false;
        debugPrint("Reading NL...");
        if (serial.read() != '\n') isValid = false;
        debugPrint("Reading 3...");
        if (serial.read() != 3) isValid = false;
        if (!isValid) return isValid;

        // Compute the checksum.
        debugPrint("Computing even checksum...");
        if (
                (
                charToBinary(buffer[0]) ^
                charToBinary(buffer[2]) ^
                charToBinary(buffer[4]) ^
                charToBinary(buffer[6]) ^
                charToBinary(buffer[8])
                )
                !=
                charToBinary(checksum[0])
                )
            isValid = false;
        debugPrint("Computing odd checksum...");
        if (
                (
                charToBinary(buffer[1]) ^
                charToBinary(buffer[3]) ^
                charToBinary(buffer[5]) ^
                charToBinary(buffer[7]) ^
                charToBinary(buffer[9]))
                !=
                charToBinary(checksum[1])
                )
            isValid = false;
        if (!isValid) return isValid;

        // Null terminate the buffer and checksum.
        debugPrint("Null terminating buffers...");
        buffer[10] = '\0';
        checksum[2] = '\0';
        debugPrint("Returning...");
        return isValid;
    } else { // Branch where tag is not in range.
        while(serial.available() != 0) {
            serial.read();
        }
        return false;
    }
}

unsigned char ID_xxLA::charToBinary(char c) {
    if ((c >= '0') && (c <= '9')) {
        return (c & 0x0F);
    } else if (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f'))) {
        // 4 MSB and add 9 to convert to binary.
        return (c & 0x0F) + 9;
    } else {
        return -1;
    }
}
