/*
  The following code is intended specifically for the DS18B20 and an Arduino Uno.
  Ensure that the sensor has been wired properly, before running this code.

  This software will gather temperature and time data from a DS18B20 sensor and a DS3231 RTC module, respectively.
  The information automaticlly gets stored into a micro SD card via the SD card module.
  An external LED will blink each time a measurement is taken.
  
  The toggle switch is to safely shut down the SD card, before the Arduino Uno is manually disconnected from the power source.
  When the SD card is safely shut down, the LED remains on, indicating that it is safe to disconnect the power supply.

  When the code is first ran, the LED is constantly on, while the modules are booting up.
  Once all modules are active, then the LED will begin to blink.
  If the LED does not blink within the first minute, that means there is something wrong with at least one of the modules.

  Written by: Adriann Liceralde
  adriann8399@gmail.com
  */
  
//IMPORT LIBRARIES
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>
#include <SdFat.h>
#include <avr/wdt.h>

//SETUP ONEWIRE
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

//PIN SETTINGS
//const int SWITCH_PIN = 3;       //Digital Pin #3
//const int LED_PIN = 4;          //Digital Pin #4
//int ledState = 1;               //State of the led. 0 means that blinking is off and the led will stay on, indicating that the sd file is closed



//PRE_ALLOCATE VARIABLES
RTC_DS3231 rtc;
DateTime dt;
DateTime now_dt;
uint32_t ut;
uint32_t now_ut;
float temp;
const uint8_t sdChipSelect = SS;
SdFat sd;
SdFile file;    


//THIS IS THE SETUP CHUNK THAT THE ARDUINO WILL RUN ONCE
void setup() {
  Serial.begin(9600);       // This is for the Serial Monitor of the Arduino

  //SETUP EXTERNAL LED AND TOGGLE SWITCH
  //pinMode(LED_PIN, OUTPUT);       //Set the pin to output, so that it can control current flow into the LED
  //digitalWrite(LED_PIN, HIGH);    //This ensures that the LED starts as on
  //attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), fileclose, FALLING); //Sets up the toggle switch
  

  //SETUP RTC AND SD MODULES
  RTCBegin();
  //rtc.adjust(1645469240);
  SDBegin();
  file.open("DATA.csv", O_CREAT|O_WRITE|O_APPEND);
  file.print("YEAR");
  file.print("MONTH");
  file.print("DAY");
  file.print("HOUR");
  file.print("MINUTE");
  file.print("SECOND");
  file.println("TEMP");

  Serial.println("Unixtime - Temp");
  wdt_enable(WDTO_8S); //Enable the watchdog timer.
                       //The board will reset after 8 seconds of inactivity, in case the sd card module is experiencing problems
}


//THIS IS THE MAIN LOOP THAT THE ARDUINO WILL RUN FOREVER
void loop() {

  //GET TIME AND CO2
  dt = rtc.now();                         //Execute this function to retrieve the time from the RTC module
  ut = dt.unixtime();                     //Convert the dt variable to a unixtime
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);

  //PRINT DATA TO SERIAL AND TO SD CARD
  Serial.print(dt.year(), DEC);
  Serial.print('/');
  Serial.print(dt.month(), DEC);
  Serial.print('/');
  Serial.print(dt.day(), DEC);
  Serial.print(" ");
  Serial.print(dt.hour(), DEC);
  Serial.print(':');
  Serial.print(dt.minute(), DEC);
  Serial.print(':');
  Serial.print(dt.second(), DEC);
  Serial.print(" ");
  Serial.println(temp);
  
  file.print(dt.year());
  file.print(",");
  file.print(dt.month());
  file.print(",");
  file.print(dt.day());
  file.print(",");
  file.print(dt.hour());
  file.print(",");
  file.print(dt.minute());
  file.print(",");
  file.print(dt.second());
  file.print(",");
  file.println(temp);
  file.sync();

//  //BLINK THE LED
//  if (ledState == 1){
//    digitalWrite(LED_PIN, HIGH);            //Turn led on
//    delay(50);
//    digitalWrite(LED_PIN, LOW);             //Turn led off
//  }


  //WAIT FOR A SPECIFIED AMOUNT OF TIME BEFORE NEXT MEASUREMENT
  do {
    now_dt = rtc.now();
    now_ut = now_dt.unixtime();
  }
  while ( now_ut < ut + 1 );            //This will wait until 3 seconds as elapsed. Change the value as desired.

  wdt_reset(); // Reset the watchdog timer
}




//DEFINING THE FUNCTION TO START-UP THE RTC MODULE
void RTCBegin() {
  bool success = false;
  while (success == false) {
    if(rtc.begin())
      success = true;
    else
      Serial.println("RTC Failed");
    delay(1000);
  }
  Serial.println("RTC Operational");
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
  //ledState = 0;
  //digitalWrite(LED_PIN, HIGH);
}
