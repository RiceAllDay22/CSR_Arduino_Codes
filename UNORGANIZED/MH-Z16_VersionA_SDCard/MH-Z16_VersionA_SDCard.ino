/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------

INTRODUCTION
  This code is a combination of two codes: the MH-Z16_VersionA code and the SDCardModule code
  Please read the respective guides for each one first, before proceeding.
*/



//IMPORT LIBRARIES AND SETUP VARIABLES
#include <Wire.h>
#include <NDIR_I2C.h>
#include <SdFat.h>

NDIR_I2C mySensor(0x4D);
uint32_t conc;
const uint8_t sdChipSelect = SS;
SdFat sd;
SdFile file;



//SETUP PINS & MODULES
void setup() {
  Serial.begin(9600);   // This is the baud rate. Keep at 9600, no need to change
  
  if (mySensor.begin()) {
    Serial.println("Wait 10 seconds for sensor initialization...");
    delay(10000);       // Make the Arduino wait for 10,000 milliseconds to give the sensor time to warm-up
  }
  else {
    Serial.println("ERROR. Check Wiring and Restart Arduino");
    while(1);           // If there is an error, this while loop will run indefinitely
  }

  if(sd.begin(sdChipSelect) == false) {
    Serial.println("Module detected. Please check wiring. ");
    while (1);
  }

  file.open("DATA.csv", O_CREAT|O_WRITE|O_APPEND);
  file.print("CO2 Data");
  
}


// MAIN CODE
void loop() {
  if (mySensor.measure()) {
      conc = mySensor.ppm;
      Serial.println(conc);
      file.println(conc);
      file.sync();
  }
  else {
    Serial.println("No Data");
  }
  
  delay(1000);        // Wait for 1000 milliseconds, before attempting to take more data readings
                      // This value can be decreased or increased, as desired
}
