#include <ID_xxLA.h>

const int RXPin = 8;
const int unusedTXPin = 10;
const int TIRPin = 5;

ID_xxLA rfidReader(RXPin, unusedTXPin, TIRPin);
char readData[11];

void setup() {
  Serial.begin(9600);
  Serial.println("ID_xxLA Tester starting up...");
}

void loop() {
  if(rfidReader.getID(readData)) {
    Serial.println(readData);
  }
  delay(1000);
}
