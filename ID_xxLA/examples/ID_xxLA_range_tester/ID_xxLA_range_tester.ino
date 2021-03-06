#include <ID_xxLA.h>

const int RXPin = 4;
const int unusedTXPin = 6;
const int TIRPin = 5;

ID_xxLA rfidReader(RXPin, unusedTXPin, TIRPin);
char tag[11];
char checksum[3];
bool isTagInRange;

void setup() {
    Serial.begin(9600);
    Serial.println("ID_xxLA Tester starting up...");
    isTagInRange = false;
}

void loop() {
    if (!isTagInRange && digitalRead(TIRPin)) {
        Serial.println("Tag is in range.");
        isTagInRange = true;
    } else if (isTagInRange && !digitalRead(TIRPin)) {
        Serial.println("Tag is in not range.");
        isTagInRange = false;
    }
//
//    Serial.println("Reading tag...");
//    bool isValid = rfidReader.getID(tag, checksum);
//
//    if (isValid) {
//        Serial.println("Data is valid.");
//        Serial.print("Tag is: "), Serial.println(tag);
//        Serial.print("Checksum is: "), Serial.println(checksum);
//    } else {
//        Serial.println("Data is not valid.");
//    }
//    Serial.println();
//    delay(1000);
}
