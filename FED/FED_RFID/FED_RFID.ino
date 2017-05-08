// The following initiatlize libraries used in FED code
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <SdFat.h>               
#include "RTClib.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <SoftwareSerial.h>
#include <Adafruit_MotorShield.h>
#include <ID_xxLA.h>

// Notes on which pins are used:
// The Motor shield uses the I2C pins, which are pins A4 and A5 on the Arduino pro.
// The data logging shield uses the SPI pins, which are D10, D11, D12, and D13. 
// Hence, the ID_xxLA RFID reader will use pins D4, D5, and D6, which should be unoccupied. 
// The Tx pin, however, is unused because the ID-12LA does not receive any commands.
const int PHOTO_INTERRUPTER_PIN = 2; // This initializes the pin on the Arduino that the photointerrupter is connected to
 const int TTL_DEBUG_PIN = 5; // This initiatlizes the pin on the Arduino that the BNC output is connected to
const int CS_pin = 10;  // This initializes the SD card on pin 10
const int rfidRxPin = 4;
const int rfidTxPin = 6; // Unused, but necessary to define.
const int rfidTIRPin = 3; // Must be pin 3, as rfidTIR pin must be interrupt-enabled.

// Use this to set when the FED system should drop a new pellet.
// PELLET_GONE: Drop a pellet if one is not present in the feed well.
// MOUSE_NEAR: Drop a pellet if a mouse is detected by an RFID tag. 
// BOTH: Drop a pellet when both conditions are met (Logical AND);
enum Mode {PELLET_GONE, MOUSE_NEAR, BOTH} mode = MOUSE_NEAR;

// Use this to allow only certain mice to activate a pellet drop, when MOUSE_NEAR or BOTH is used as the mode. 
// Leave as empty for any mouse to activate a pellet drop; otherwise, populate the IDs.
// Ensure the value of acceptable_IDs_len matches the number of elements in the array.
// Example: const char* acceptable_IDs = {"5A6D32F59B", "12B6DE4A62"};
const char* const acceptable_IDs[] = {""};
const int acceptable_IDs_len = 0;

// Intitialzing global variables
File dataFile;
#define FILENAME "FED_DATA.csv"  // Change this to alter CSV file name
int PIState = 1;
int lastState = 1;
int pelletCount = 0;
SdFat SD; // defining an object SD
RTC_DS1307 RTC;    // refer to the real-time clock on the SD shield
String time;
ID_xxLA rfidReader(rfidRxPin, rfidTxPin, rfidTIRPin);

// Defining constants for calculating timing
long previousMillis = 0;
long startTime = 0;
long timeElapsed = 0;
const long day2 = 86400000; // 86400000 milliseconds in a day
const long hour2 = 3600000; // 3600000 milliseconds in an hour
const long minute2 = 60000; // 60000 milliseconds in a minute
const long second2 =  1000; // 1000 milliseconds in a second

// Constants to understand the photointerrupter better.
const int PI_present = 0;

// Setting up the stepper motor 
const int STEPS_TO_INCREMENT = 64;
const int MOTOR_STEPS_PER_REVOLUTION = 513;
Adafruit_MotorShield gMotorShield = Adafruit_MotorShield();
// Set the second argument to 1 to use M1 and M2 on the motor shield, or set as 2 to use M3 and M4:
Adafruit_StepperMotor *gPtrToStepper = gMotorShield.getStepper(MOTOR_STEPS_PER_REVOLUTION,1); 

// This function returns a string that is the current YMD, HMS.
String getDateTimeString() {
  String year, month, day, hour, minute, second;
  DateTime datetime = RTC.now();
  year = String(datetime.year(), DEC);
  month = String(datetime.month(), DEC);
  day  = String(datetime.day(),  DEC);
  hour  = String(datetime.hour(),  DEC);
  minute = String(datetime.minute(), DEC);
  second = String(datetime.second(), DEC);

  // concatenates the strings defined above into date and time
  return time = month + "/" + day + " " + hour + ":" + minute + ":" + second;
}

// This creates the function "logData" that is used in the code
int logData(){
  power_twi_enable();
  power_spi_enable();

  // opens a file on the SD card and prints a new line with the
  // current reinforcement schedule, the time stamp,
  // the number of sucrose deliveries, the number of active and
  // inactive pokes, and the number of drinking well entries.
  String time = getDateTimeString();
  dataFile = SD.open(FILENAME, FILE_WRITE);
  if (dataFile) {
    Serial.println(F("File successfully written..."));
    Serial.println(time);
    dataFile.print(time);
    dataFile.print(",");
    dataFile.print(pelletCount);
    dataFile.print(",");
    dataFile.print(timeElapsed);
    // Weimen: Extra comma for RFID field.
    dataFile.println(",");
    dataFile.close();
  }
  power_twi_disable();  // this reduces power consumption
  power_spi_disable();  // this reduces power consumption
}

// Weimen Li - This function is like "logData", but logs RFID reading data instead of pellet data.
int logRFIDData(char* RFIDtag){
  power_twi_enable();
  power_spi_enable();

  // opens a file on the SD card and prints a new line with the
  // current reinforcement schedule, the time stamp,
  // the number of sucrose deliveries, the number of active and
  // inactive pokes, and the number of drinking well entries.
  String time = getDateTimeString();

  dataFile = SD.open(FILENAME, FILE_WRITE);
  if (dataFile) {
    Serial.println(F("File successfully written..."));
    Serial.println(time);
    dataFile.print(time);
    // Extra commas are there to maintain the order of the .CSV file.
    dataFile.print(",,,");
    dataFile.println(RFIDtag);
    dataFile.close();
  }
  power_twi_disable();  // this reduces power consumption
  power_spi_disable();  // this reduces power consumption
}

// Determine whether to drop a pellet.
// Warning: RFIDtag is assumed to have length = 11.
bool calculate_shouldDropPellet(bool PIState, bool lastPIState, char* RFIDtag, unsigned long lastDropTime) {
    Serial.println("In calculate_shouldDropPellet.");
  // Don't drop if it has been less than 500 ms since the last pellet was dropped.
  unsigned long timeElapsed = millis() - lastDropTime;
  if(timeElapsed < 500) {
    Serial.println("  False because time elapsed not long enough.");
    return false;
  }

  // Don't drop if a pellet is already there.
  if (PIState == PI_present) {
    Serial.println("  False because pellet is already present..");
    return false;
  }

  // Drop simply if a pellet is not present.
  if(mode == PELLET_GONE) {
    if (PIState != PI_present) {
      Serial.println("  True because mode is PELLET GONE and pellet is gone.");
      return true;
    } else {
      Serial.println("  True because mode is PELLET GONE and pellet is not gone.");
      return false;
    }
  } 

  else if (mode == MOUSE_NEAR || mode == BOTH) {
    if (mode == BOTH && PIState != PI_present) {
      Serial.println("  False because mode is BOTH and pellet is not gone.");
      return false;
    }
    // Proceed only if the RFID tag is not empty.
    if (RFIDtag[0] != '\0') {
      // Drop if we accept any mouse.
      if (acceptable_IDs_len == 0) {
        Serial.println("  True because mode is BOTH or PI_PRESENT and RFID tag detected.");
        return true;
      } 
      // Otherwise, check if a preferred mouse is present.
      else {
        for(int i = 0; i < acceptable_IDs_len; i++) {
          if(strncmp(RFIDtag, acceptable_IDs[i], 10) == 0) {
            Serial.println("  True because mode is BOTH or PI_PRESENT and RFID tag matched.");
            return true;
          }
        }
        Serial.println("  False because mode is BOTH and RFID tag not matched.");
        return false;
      }
    } 
    Serial.println("  False because mode is BOTH or PI_PRESENT and RFID tag is empty.");
    return false;
  }
}

void setup(){

  // make all unused pins inputs with pullups enabled by default, lowest power drain
  // leave pins 0 & 1 (Arduino RX and TX) as they are
  for (byte i=7; i <= 20; i++) {    
    pinMode(i, INPUT_PULLUP);     
  }
  
  // this saves power by disabling ADC as we won't be using it
  ADCSRA = 0;  
  power_adc_disable(); // ADC converter
  power_timer1_disable();// Timer 1
  power_timer2_disable();// Timer 2

  // This starts serial monitoring - With FED connected, open the Serial Monitor in the 
  // Arduino IDE to watch FED output in real-time for debugging
  Serial.begin(9600);
  Serial.println(F("Starting up..."));
  Serial.print(F("Operating mode: "));
  Serial.println(mode);

  // Set Arduino pins modes to input or output
  pinMode(PHOTO_INTERRUPTER_PIN, INPUT);
  pinMode(TTL_DEBUG_PIN, OUTPUT);
  pinMode(CS_pin, OUTPUT);

  //Starting the Wire, RTS and Motoshield libaries
  Wire.begin();
  RTC.begin(); // RTC library needs Wire
  gMotorShield.begin(); // use default I2C address of 0x40
  
  // Set stepper rpm, 255 max for this 5V stepper
  gPtrToStepper->setSpeed(255); 

  // Check if RTC is working correctly
  if (! RTC.isrunning()) {
    Serial.println(F("RTC is NOT running!"));
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Uncomment the below line to set the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  // This checks if a valid SD card is installed in FED.  If it can't find a valid card it will stop the code.
  if (!SD.begin(CS_pin)) {
    Serial.println(F("Card failed, or not present"));
    while (1) ;
  }
  Serial.println(F("Card initialized."));

  // Open the CSV file - if there is a problem the code will stop here. 
  dataFile = SD.open(FILENAME, FILE_WRITE);
  if (! dataFile) {
    Serial.println(F("Error opening datalog.txt"));
    while (1) ;
  }
  else {
    dataFile.print(time);
    dataFile.println(F("Time,Pellet Count,Pellet Drop Delay,RFID"));
    dataFile.close();
  }    
  delay (50);  // delay helps give the card a bit more time
  lastState = 0;}

//The following is the main loop of the FED code
  void loop(){
  // Read the photo interrupter pin to see if the pellet has been removed
    PIState = digitalRead(PHOTO_INTERRUPTER_PIN);
    // Read the RFID tag.
    char RFIDtag[11] = "";
    bool successfulRead = rfidReader.getID(RFIDtag);

  // These are debugging lines for serial monitoring
    Serial.print("Photointerrupter State: ");
    Serial.println(PIState);
    digitalWrite(TTL_DEBUG_PIN, LOW);

  // These lines log the RFID tag if it is present.
    if (successfulRead) {
     Serial.print("Read RFID: ");
     Serial.println(RFIDtag);
   }

  // The following checks if the pellet has been removed, and if it has it dispenses another pellet.  
  // The code contains protection against dispensing double pellets
    // If pellet is not present, but was present.
    if (PIState == 1  & PIState != lastState) {    
      digitalWrite(TTL_DEBUG_PIN, HIGH);    
      startTime = millis();
      Serial.print(F("Time Elapsed Check: "));
      Serial.println(timeElapsed);
      timecounter(timeElapsed);
      pelletCount ++;
      logData();
      Serial.println(F("It did work"));
      lastState = PIState;
    }
    // If pellet is not present.
    else if (calculate_shouldDropPellet(PIState, lastState, RFIDtag, timeElapsed)) {
      if(mode == MOUSE_NEAR || mode == BOTH) {
        logRFIDData(RFIDtag);
      }
      Serial.println(F("Turning motor..."));
      power_twi_enable();
      gPtrToStepper->step(STEPS_TO_INCREMENT/3,BACKWARD,DOUBLE);
      delay (500);
      PIState = digitalRead(PHOTO_INTERRUPTER_PIN);
      delay(500);
      if (PIState == 1) {gPtrToStepper->step(STEPS_TO_INCREMENT, FORWARD, DOUBLE);
        Serial.println("moved forward");} 
        gPtrToStepper->release();
        power_twi_disable();
        lastState = PIState;
      }
      // If pellet is present, but was not present.
      else if (PIState == 0 & PIState != lastState) {
        Serial.print(F("Time Elapsed Since Last Pellet: "));
        timeElapsed = millis() - startTime;
        Serial.println(timeElapsed);
        lastState = PIState;
      }
      // If pellet is simply present.
      else  {
        lastState = PIState;
        Serial.flush();
        enterSleep();
      }
      delay(500);
    }

// utility function for controlling time display on Serial monitor output
    void printDigits(byte digits) {
      if (digits < 10) {
        Serial.print('0');
      }
      Serial.print(digits, DEC);
    }

// function for entering sleep mode to save power
void enterSleep(){
  power_usart0_disable();// Serial (USART) 
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(PHOTO_INTERRUPTER_PIN), pinInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(rfidTIRPin), pinInterrupt, RISING);
  // lastState = 0;
  delay(100);
  
  // set_sleep_mode(SLEEP_MODE_PWR_DOWN);   
  set_sleep_mode(SLEEP_MODE_ADC);   
  cli();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  sleep_disable();
}

// function for allowing the FED to wakeup when pellet is removed
void pinInterrupt(void){
  detachInterrupt(digitalPinToInterrupt(PHOTO_INTERRUPTER_PIN));
  detachInterrupt(digitalPinToInterrupt(rfidTIRPin));
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  /* Re-enable the serial. */
  power_usart0_enable();
}

// function for printing timing information in serial monitor display
void timecounter(long timeNow) {
  Serial.println(timeNow);
  int days = timeNow / day2 ;                                //number of days
  int hours = (timeNow % day2) / hour2;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  int minutes = ((timeNow % day2) % hour2) / minute2 ;         //and so on...
  int seconds = (((timeNow % day2) % hour2) % minute2) / second2;
  int mil = ((((timeNow % day2) % hour2) % minute2) % second2);

  // digital clock display of current time
  printDigits(hours); Serial.print(":");
  printDigits(minutes); Serial.print(":");
  printDigits(seconds); Serial.print(":");
  printDigits(mil);
  Serial.println();
}
