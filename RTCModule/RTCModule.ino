/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------
  
INTRODUCTION
  This code is for the DS3231 Timekeeping Clock Module.

WIRING
  Module -> Arduino
  ------------------------
  GND  Pin -> GND     Port
  VCC  Pin -> 5.0 V   Port
  SDA  Pin -> SDA     Port
  SCL  Pin -> SCL     Port
  SQW  Pin -> NOTHING
  32K  Pin -> NOTHING

LIBRARY INSTALLATION
  1. In the Arduino IDE, go to "Tools".
  2. Go to "Manage Libraries"
  3. A popup window will open
  4. Search for "RTClib"
  5. The library you want is the one that was written by Adafruit
  6. Install the latest version.


IMPORTANT NOTES

SPECIFICATIONS

  Written by Adriann Liceralde.
  Carbon Science Research at the University of Utah.


*/


// ---------- IMPORT LIBRARIES AND SETUP VARIABLES ----------
#include <RTClib.h>               //Imports the library that is for RTC modules
RTC_DS3231 rtc;                   //Sets the RTC module as a DS3231 model          
DateTime dt;                      //Creates a DateTime variable to store the time.
DateTime now_dt;                  //Creates a DateTime variable to store the time.


// ---------- MAIN SETUP ----------
void setup() {
  Serial.begin(9600);             //Activates the Serial port and sets the baud rate of the Serial Monitor. 9600 is the common rate

  if(rtc.begin()) {               //If the device activates successfully, then run the block of code 
    Serial.println("RTC Operational");
  }
  else {                          //If the device does not activate, then run this block instead
    Serial.println("Error. Check wiring and restart Arduino");
    while(1);                     //This while loop will run indefinitely
  }


  //There are two ways to set the datetime. 
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));     //This way will adjust the datetime to the datetime your computer gives.
  //rtc.adjust(DateTime(2020, 1, 14, 20, 8, 40));     //This way will adjust the datetime to whatever you want. This example sets the datetime to January 14, 2020 20:08:40
}


// ---------- MAIN CODE ----------
void loop() {
  dt = rtc.now();                                     //Retrieves the time from the RTC and stores it into variable "dt"
    
  Serial.print(dt.unixtime());  Serial.print(" or "); //This will give you datetime in the format of unixtime
  
  Serial.print(dt.year());      Serial.print("-");    //Gives the current year
  Serial.print(dt.month());     Serial.print("-");    //Gives the current month
  Serial.print(dt.day());       Serial.print(" ");    //Gives the current day
  Serial.print(dt.hour());      Serial.print(":");    //Gives the currrent hour
  Serial.print(dt.minute());    Serial.print(":");    //Gives the current minute
  Serial.println(dt.second());                        //Gives the current second
  

  //The lines below makes it so that we get the datetime every 2 seconds
  //You can change the collecton frequency by simply changing the number.
  //For example, If you want to collect the datetime every 5 seconds, then replace the 2 with 5.
  do {
    now_dt = rtc.now();
  } while ( now_dt.unixtime() < dt.unixtime() + 2 );  
}
