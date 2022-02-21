/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------

INTRODUCTION
  This code compiles the individual codes for the SCD-30 sensor, the SDCard Module, and the RTC module.
  This will collect CO2 data and timestamps, and stores them into an SD Card.


IMPORTANT NOTE
  The SCD-30 device is EXTREMELY sensitive to electrostatic discharge.
  NEVER touch the sensor UNLESS wearing Anti-Static Gloves.
  Even if the sensor is not connected to power, still exercise extreme caution.


SCD30 MODULE WIRING
  Sensor -> Arduino Nano
  ------------------------
  Vin Pin -> 5.0V Port
  Gnd Pin -> GND  Port
  SCL Pin -> SCL Port
  SDA Pin -> SDA Port


RTC MODULE WIRING
  Module -> Arduino
  ------------------------
  GND  Pin -> GND     Port
  VCC  Pin -> 5.0 V   Port
  SDA  Pin -> SDA     Port
  SCL  Pin -> SCL     Port
  SQW  Pin -> NOTHING
  32K  Pin -> NOTHING


SD MODULE WIRING
  Module -> Arduino
  ------------------------
  CS   Pin -> Digital Pin #10
  SCK  Pin -> Digital Pin #13
  MOSI Pin -> Digital Pin #11
  MISO Pin -> Digital Pin #12
  VCC  Pin -> 5.0 V   Pin
  GND  Pin -> GND     Pin

*/


//IMPORT LIBRARIES AND SETUP VARIABLES
#include <Wire.h>
#include <RTClib.h>
#include <SdFat.h>
#include <SparkFun_SCD30_Arduino_Library.h>

SCD30 airSensor;
uint32_t conc;
const byte CALIB_PIN   = 5;

RTC_DS3231 rtc;
DateTime dt;
DateTime now_dt;

const uint8_t sdChipSelect = SS;
SdFat sd;
SdFile file;

const byte LED_PIN      = 6;

//SETUP PINS & MODULES
void setup() {
  //General Setup
  Serial.begin(9600);                   // This is the baud rate. Keep at 9600, no need to change
  pinMode(CALIB_PIN, INPUT_PULLUP);     // This setups the calibration wire pin
  pinMode(LED_PIN, OUTPUT);             // This setups the pin for the LED light.
  digitalWrite(LED_PIN, HIGH);
  Wire.begin();
  


  //CO2 Sensor Setup
  if (airSensor.begin() == false) {               //Stars the module.
    Serial.println("Error. SCD not detected.");   
    while (1);                                    //If there is an error, then an infinite loop will occur.
  }
  airSensor.setMeasurementInterval(2);              //Change number of seconds between measurements: 2 to 1800 (30 minutes)
  airSensor.setAltitudeCompensation(1300);        //Set altitude of the sensor in m, stored in non-volatile memory of SCD30
  //airSensor.setForcedRecalibrationFactor(1000); //Set the calibration level
  delay(1000);

  //RTC clock setup
  if(rtc.begin() == false) {                      //Starts the module.
    Serial.println("Error. RTC not detected.");
    while(1);                                     //If there is an error, then an infinite loop will occur.
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //This will adjust the datetime to the datetime your computer gives.
  delay(1000);

  
  //SD Card Module setup
  if(sd.begin(sdChipSelect) == false) {           //Stars the module.
    Serial.println("Error. SD not detected.");
    while (1);                                    //If there is an error, then an infinite loop will occur.
  }
  file.open("DATA.csv", O_CREAT|O_WRITE|O_APPEND);//Creates csv file 
  file.print("Column #1");                        //Creates a column header
  file.println("Column #2");                      //Creates a column header
  delay(1000);

  digitalWrite(LED_PIN, LOW);

} 


// MAIN CODE
void loop() {
  digitalWrite(LED_PIN, HIGH);
  dt = rtc.now();                           //Collect timestamp from RTC
  conc = airSensor.getCO2();                //Collect CO2 reading from SCD30
  
  Serial.print(dt.unixtime());              //Print data to Serial Monitor
  Serial.print(",");                        //^
  Serial.println(conc);                     //^
  
  file.print(dt.unixtime());                //Print data to SD Card
  file.print(",");                          //^
  file.println(conc);                       //^
  file.sync();                              //Temporarily close the SDCard

  digitalWrite(LED_PIN, LOW);

  
  //The code below makes it so that the Arduino waits 3 seconds before collecting another data sample.
  //With the SCD-30 module, 2.1 seconds is the fastest frequency that it can take measurements.
  //Therefore, the Arduino is set to a collection frequency of 3 seconds.
  do {
    now_dt = rtc.now();
  } while ( now_dt.unixtime() < dt.unixtime() + 3 ); 
}
