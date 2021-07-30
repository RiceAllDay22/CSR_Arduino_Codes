/*
  ---------- PLEASE READ BEFORE STARTING ----------
  
  Guide for Operating MH-Z16 Sensor via I2C Communication.
  By:       Adriann Liceralde
  For:      Carbon Science Research at the University of Utah
  License:  MIT (basically do whatever you want with this code)
  Date:     July 29, 2021

INTRODUCTION
  This code is for the MH-Z16 CO2 sensor (black cylinder shape).
  The sensor must be connected to the blue UART-I2C adaptor for this code to work.
  Ensure that the wiring is correct before running the code and powering the Arduino.
  The blue adaptor has a switch. Make sure it is set to I2C mode. 
  Any line(s) in this code may be copied and pasted into other Arduino codes, as desired.

IMPORTANT NOTES
  Do not expose sensor directly to sunlight.
  Do not expose sensor in water or rainy conditions.

WIRING
  Adaptor  -> Arduino
  ------------------------
  GND Pin  -> GND  Port
  VCC Pin  -> 5.0V Port
  SDA Pin  -> SDA  Port
  SCL Pin  -> SCL  Port
  Refer to external png file for a picture of the wiring.

CALIBRATION
  There is a white button on the blue adaptor.
  Once the sensor has been in a stable condition for a few minutes, hold the button for 10 seconds.
  The sensor will then start giving readings around 400 ppm.
  Performing a calibration will always force the sensor to give 400 ppm readings. 

SPECIFICATIONS
  Collection Frequency is normally set to 1 second but can be increased or decreased.
  Measurement Range is 0 to 5000 ppm.
  Accuracy is ± (50 ppm + 5% of the Measured Value)
  Current Consumption is 60 mA.

LINKS
  Product Info:     https://sandboxelectronics.com/?product=mh-z16-ndir-co2-sensor-with-i2cuart-5v3-3v-interface-for-arduinoraspeberry-pi
  Datasheet:        https://sandboxelectronics.com/wp-content/uploads/2018/08/Z16DS.pdf
  Arduino Library:  https://github.com/SandboxElectronics/NDIR (Use the one that is titled NDIR_I2C)

CONTACT
  Email any questions or concerns to adriann8399@gmail.com
*/


// ---------- IMPORT LIBRARIES AND SETUP VARIABLES ----------
#include <Wire.h>                             //Imports the library that allows for communication with I2C devices
#include <NDIR_I2C.h>                         //Imports the library that is for the MH-Z16 sensor
NDIR_I2C mySensor(0x4D);                      //Sets the I2C address of the blue adaptor
uint32_t conc;                                //Creates a 32 bit (4 byte) variable to store the concentration data 


// ---------- MAIN SETUP ----------
void setup() {
  Serial.begin(9600);                         //Activates the Serial port and sets the baud rate of the Serial Monitor. 9600 is the common rate
  
  if (mySensor.begin()) {                     //If the device activates successfully, then run the block of code 
    Serial.println("Sensor Operational.");
    delay(10000);                             //Make the Arduino wait for 10,000 milliseconds to give the sensor time to warm-up
  }
  else {                                      //If the device does not activate, then run this block instead
    Serial.println("Error. Check wiring and restart Arduino");
    while(1);                                 //This while loop will run indefinitely
  }
}


// ---------- MAIN CODE ----------
void loop() {
  if (mySensor.measure()) {                   //If the device can measure successfully, then run the block of code
      conc = mySensor.ppm;                    //Extract the ppm measurement and store into the variable called conc
      Serial.println(conc);                   //Print the data into Serial
  }
  else {                                      //If the device cannot measure, then run the block below
    Serial.println("No Data");                //Print this unfortunate message into the Serial port
  }  
  delay(1000);                                //Wait for 1000 milliseconds before attempting to take more data readings. This value can be decreased or increased, as desired
}
