#include <ID_xxLA.h>


ID_xxLA rfidReader(2,3,4);

void setup() {
  Serial.begin(9600);

}

void loop() {
  char readData[11];
  rfidReader.getID(readData);
  Serial.println(readData);
}
