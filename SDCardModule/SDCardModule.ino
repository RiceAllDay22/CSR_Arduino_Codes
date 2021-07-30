/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------

INTRODUCTION
  This code is for the SDCard Module.

  Written by Adriann Liceralde.
  Carbon Science Research at the University of Utah.

WIRING
  Module -> Arduino
  ------------------------
  CS   Pin -> Digital Pin #10
  SCK  Pin -> Digital Pin #13
  MOSI Pin -> Digital Pin #11
  MISO Pin -> Digital Pin #12
  VCC  Pin -> 5.0 V   Pin
  GND  Pin -> GND     Pin

IMPORTANT NOTES


SPECIFICATIONS


LINKS



  Written by Adriann Liceralde.
  Carbon Science Research at the University of Utah.

*/


//IMPORT LIBRARIES AND SETUP VARIABLES
#include <SdFat.h>
const uint8_t sdChipSelect = SS;
SdFat sd;
SdFile file;


//SETUP PINS & MODULES
void setup() {
  Serial.begin(9600);   // This is the baud rate. Keep at 9600, no need to change
  
  if(sd.begin(sdChipSelect) == false) {
    Serial.println("Module not detected. Please check wiring. ");
    while (1);
  }
  
  file.open("DATA.csv", O_CREAT|O_WRITE|O_APPEND);
  file.print("Column #1");
  file.println("Column #2");
} 


// MAIN CODE
void loop() { 
  int value_a = 500;
  int value_b = 1000;

  Serial.print(value_a);  //Print data to Serial Monitor
  Serial.print(',');
  Serial.println(value_b);
 
  file.print(value_a);    //Print data to SD Card
  file.print(',');
  file.println(value_b);

  file.sync();            //Temporarily close the SDCard


  delay(1000);            //Wait for 1000 millseconds. Can change as necessary.
}
