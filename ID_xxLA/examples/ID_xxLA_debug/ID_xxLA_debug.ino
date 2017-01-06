#include <ID_xxLA.h>

const int RXPin = 8;
const int unusedTXPin = 10;
const int TIRPin = 5;

ID_xxLA rfidReader(RXPin, unusedTXPin, TIRPin);
char tag[11];
char checksum[3];

void setup() {
    Serial.begin(9600);
    Serial.println("ID_xxLA Tester starting up...");
}

void loop() {
    if (digitalRead(TIRPin)) {
        Serial.println("Tag is in range.");
    } else {
        Serial.println("Tag is in not range.");
    }

    Serial.println("Reading tag...");
    bool isValid = rfidReader.getID(tag, checksum);

    if (isValid) {
        Serial.println("Data is valid.");
        Serial.print("Tag is: "), Serial.println(tag);
        Serial.print("Checksum is: "), Serial.println(checksum);
    } else {
        Serial.println("Data is not valid.");
    }
    Serial.println();
    delay(1000);
}
