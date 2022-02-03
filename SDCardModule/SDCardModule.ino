/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------

INTRODUCTION
  This code is a simple data logger using the SDCard Module and a Toggle Switch.
  In this example, the SD Card will store integers as data.
  It is important that before disconnecting the power supply, the toggle switch needs to be flipped towards the GND state.
  Flipping the switch will safely close the SD file to minimize the chances of a file corruption.
  Once the switch is flipped, then it is safe to disconnect the power.
  Flip the switch back to the 5V state, before connecting the power back.
  
  Written by Adriann Liceralde.
  Carbon Science Research at the University of Utah.

*/


//IMPORT LIBRARIES AND SETUP VARIABLES
#include <SdFat.h>
const int SWITCH_PIN = 3;         //Digital Pin number that the toggle switch is attached to.
const uint8_t sdChipSelect = SS;  //SS is Digital Pin #10 for Arduino Uno and is #53 for Arduino Mega
SdFat sd;
SdFile file;



//SETUP PINS & MODULES
void setup() {
  Serial.begin(9600);   // This is the baud rate. Keep at 9600, no need to change
  SDBegin();            // This is the function that boots up the SD Card module
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), fileclose, FALLING); //Setup the toggle switch
  
  file.open("DATA.csv", O_CREAT|O_WRITE|O_APPEND); // Creates/opens a csv file
  file.print("Column #1");      // Header for first column
  file.println("Column #2");    // Header for second column
} 


// MAIN CODE
void loop() { 
  //Collect the data
  int value_a = 500;   
  int value_b = 1000;

  //Print the data to Serial
  Serial.print(value_a); 
  Serial.print(',');
  Serial.println(value_b);

  //Save the data to the SD card
  file.print(value_a);   
  file.print(',');
  file.println(value_b);
  file.sync();            

  //Wait until next data collection. Can value change as necessary (in milliseconds)
  delay(1000);            
}



//DEFINING THE FUNCTION TO START-UP THE SD CARD MODULE
void SDBegin() {
  bool success = false;
  while (success == false) {
    if(sd.begin(sdChipSelect))
      success = true;
    else
      Serial.println("SD Module Failed");
    delay(1000);
  }
  Serial.println("SD Module Operational");
}


//DEFINING THE FUNCTION THAT WILL CLOSE THE SD FILE
void fileclose() {
  Serial.println("Close");
  file.close();
}
