/*
  The following code is intended specifically for the CU-1106H-NS model and an Arduino Mega.
  Ensure that the sensor has been wired properly, before running this code.
  This code uses UART communication to send and recieve information to and from the CU-1106H-NS sensor.

  This software will gather CO2 and time data from a CU-1106H-NS sensor and a DS3231 RTC module, respectively.
  The information automaticlly gets stored into a micro SD card via the SD card module.
  An external LED will blink each time a measurement is taken.
  
  The external button is for calibration.
  The toggle switch is to safely shut down the SD card, before the Arduino Mega is manually disconnected from the power source.
  When the SD card is safely shut down, the LED remains on, indicating that it is safe to disconnect the power supply.

  When the code is first ran, the LED is constantly on, while the modules are booting up.
  Once all modules are active, then the LED will begin to blink.
  If the LED does not blink within the first minute, that means there is something wrong with at least one of the modules.

  Written by: Adriann Liceralde
  adriann8399@gmail.com
  */
  
//IMPORT LIBRARIES
#include <RTClib.h>           
#include <SdFat.h> 
#include <avr/wdt.h>        


//PIN SETTINGS
const int BUTTON_PIN = 2;       //Digital Pin #2
const int SWITCH_PIN = 3;       //Digital Pin #3
const int LED_PIN = 4;          //Digital Pin #4
int buttonState = 0;            //State of the calibration button. 0 means it is not pushed. 1 means it is pushed.
int ledState = 1;               //State of the led. 0 means that blinking is off and the led will stay on, indicating that the sd file is closed
int calValue = 1200;            //Calibration value that the sensor will be calibrated to when the button is pressed.
                                //calValue Range is 400 to 1500. DO NOT GO OUT OF RANGE

//UART COMMANDS
const byte message_Read[4]  = {0x11, 0x01, 0x01, 0xED}; //Command to ask for the CO2 reading
byte response_Read[8];                                  //Response from sensor when message_Read is sent to it
byte response_Calib[4];                                 //Response from sensor when calibration is performed

//PRE_ALLOCATE VARIABLES
RTC_DS3231 rtc;
DateTime dt;
DateTime now_dt;
uint32_t ut;
uint32_t now_ut;
uint32_t ppm;
const uint8_t sdChipSelect = SS;
SdFat sd;
SdFile file;    


//THIS IS THE SETUP CHUNK THAT THE ARDUINO WILL RUN ONCE
void setup() {
  //SETUP SERIAL PORTS
  Serial.begin(9600);       // This is for the Serial Monitor of the Arduino
  Serial1.begin(9600);      // This is for the sensor UART line of the sensor


  //SETUP EXTERNAL LED AND TOGGLE SWITCH
  pinMode(LED_PIN, OUTPUT);       //Set the pin to output, so that it can control current flow into the LED
  digitalWrite(LED_PIN, HIGH);    //This ensures that the LED starts as on
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), fileclose, FALLING); //Sets up the toggle switch
  
  //SETUP CALIBRATION BUTTON
  pinMode(BUTTON_PIN, INPUT);
  if ( (calValue < 400) or (calValue > 1500)){     //If the calibration value is out of range, an infinite loop will occur.
    Serial.println("CALIBRATION POINT IS OUT OF RANGE");
    while(1);
  }

  //SETUP RTC AND SD MODULES
  RTCBegin();
  SDBegin();
  file.open("DATA.csv", O_CREAT|O_WRITE|O_APPEND);
  file.print("UNIXTIME"); file.println("CO2");


  Serial.println("Unixtime - CO2");
  wdt_enable(WDTO_8S); //Enable the watchdog timer.
                       //The board will reset after 8 seconds of inactivity, in case the sd card module is experiencing problems
}


//THIS IS THE MAIN LOOP THAT THE ARDUINO WILL RUN FOREVER
void loop() {

  //GET TIME AND CO2
  dt = rtc.now();                         //Execute this function to retrieve the time from the RTC module
  ut = dt.unixtime();                     //Convert the dt variable to a unixtime
  ppm = readFunction();                   //Execute this function to read the information from the sensor

  //PRINT DATA TO SERIAL AND TO SD CARD
  Serial.print(ut);
  Serial.print(",");
  Serial.println(ppm);
  file.print(ut);
  file.print(",");
  file.println(ppm);
  file.sync();

  //BLINK THE LED
  if (ledState == 1){
    digitalWrite(LED_PIN, HIGH);            //Turn led on
    delay(50);
    digitalWrite(LED_PIN, LOW);             //Turn led off
  }

  //CHECK CALIBRATION BUTTON
  buttonState = digitalRead(BUTTON_PIN);  //Check if the calibration button is pressed or not
  if (buttonState == HIGH){               //If it is pressed, then perform the calibration
    calibrateFunction();
  }

  //WAIT FOR A SPECIFIED AMOUNT OF TIME BEFORE NEXT MEASUREMENT
  do {
    now_dt = rtc.now();
    now_ut = now_dt.unixtime();
  }
  while ( now_ut < ut + 3 );            //This will wait until 3 seconds as elapsed. Change the value as desired.

  wdt_reset(); // Reset the watchdog timer
}


//DEFINING THE FUNCTION TO TAKE MEASUREMENTS
int readFunction() {
  int conc = 0;
  Serial1.write(message_Read, 4);         //Send the command to ask for a measurement
  delay(10);      
  if (Serial1.available()) {              //Check if the sensor's UART has information to give
    Serial1.readBytes(response_Read, 8);  //If there is information, then read it
    conc = response_Read[3]*256 + response_Read[4];  //Convert the information to a CO2 ppm reading
  }
  return conc;
}


//DEFINING THE FUNCTION TO PERFORM A CALIBRATION
void calibrateFunction() {
  //Convert the calibration value to bytes
  int ppm = calValue; 
  int DF0 = ppm/256;
  int DF1 = ppm - DF0*256;
  int CS = (256 - (0x11 +0x03 + 0x03 + DF0 + DF1)%256);


  //Construct the UART command, and then send it to the sensor
  byte command[6] = {0x11, 0x03, 0x03, DF0, DF1, CS}; 
  for (int i=0; i<6; i++) {
      Serial.print(command[i], HEX);
      Serial.print("-");
  }
  Serial.println("");
  Serial1.write(command, 6);    //Send the command
  delay(10); 

  //Read the response from the sensor after the calibration is performed
  Serial1.readBytes(response_Calib , 4);  //Read the response
  for (int i=0; i<4; i++) {
    Serial.print(response_Calib[i], HEX);
    Serial.print("-");
  }
  Serial.println("");
  Serial.println("Calibration Performed");
  delay(1000);
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
  ledState = 0;
  digitalWrite(LED_PIN, HIGH);
}
