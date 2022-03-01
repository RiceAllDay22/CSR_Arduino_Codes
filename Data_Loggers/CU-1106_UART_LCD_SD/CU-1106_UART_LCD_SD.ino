/*
  Works for SL-NS.
  Works for SL-N.
 */
 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SdFat.h>
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

const uint8_t sdChipSelect = SS;
SdFat sd;
SdFile file;
 
const int  RDY = 5;

const byte message_Version[4] = {0x11, 0x01, 0x1E, 0xD0};                   //Read the software version of device.
const byte message_Serial[4]  = {0x11, 0x01, 0x1F, 0xCF};                   //Read the serial number of device.
const byte message_Cont[5]    = {0x11, 0x02, 0x51, 0x01,0x9B};              //Set to continuous power mode.
const byte message_Meas[7]    = {0x11, 0x04, 0x50, 0x00, 0x02, 0x02, 0x97}; //Set to 2 second interval and smooth data of 2 
const byte message_Read[4]    = {0x11, 0x01, 0x01, 0xED};                   //Ask for CO2 reading.

byte buf_Version[17];
byte buf_Serial[16];
byte buf_Cont[6];
byte buf_Meas[6];
byte buf_Read[10];

uint16_t timeData;
uint16_t concData;
const byte LED_PIN      = 6;
const byte DETACH_PIN   = 7;


void setup() {
  
  //SETUP SERIAL AND LCD
  Serial.begin(9600);
  pinMode(LED_PIN,      OUTPUT);
  pinMode(DETACH_PIN,   INPUT_PULLUP);
  digitalWrite(LED_PIN, HIGH);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Ready");
  delay(1000);
  lcd.clear();

  //SETUP SDCARD MODULE
  if(sd.begin(sdChipSelect) == false) {
    Serial.println("Module not detected. Please check wiring. ");
    while (1);
  }
  file.open("SensorData.csv", O_CREAT|O_WRITE|O_APPEND);
  

  //CHECK SOFTWARE VERSION OF DEVICE
//  Serial.write(message_Version, 4);
//  Serial.readBytes(buf_Version, 17);
//  lcd.print("Software Version");
//  lcd.setCursor(0,1);
//  for (int i=0; i<17; i++) {
//    lcd.print(buf_Version[i], HEX);
//    lcd.print("-");
//  }
//  delay(1000);
//  lcd.clear();

  //CHECK SERIAL NUMBER OF DEVICE
//  Serial.write(message_Serial, 4);
//  Serial.readBytes(buf_Serial, 16);
//  lcd.print("Device Serial:");
//  lcd.setCursor(0,1);
//  for (int i=0; i<16; i++) {
//    lcd.print(buf_Serial[i], HEX);
//    lcd.print("-");
//  }
//  delay(1000);
//  lcd.clear();


  //SET TO CONTINUOUS MODE
  Serial.write(message_Cont, 5);
  Serial.readBytes(buf_Cont, 6);
  lcd.print("Continuous Set:");
  lcd.setCursor(0,1);
  for (int i=0; i<6; i++) {
    lcd.print(buf_Cont[i], HEX);
    lcd.print("-");
  }
  delay(1000);
  lcd.clear();


  //SET MEASUREMENT PERIOD
  Serial.write(message_Meas, 7); 
  Serial.readBytes(buf_Meas, 6);
  lcd.print("Measurement Set:");
  lcd.setCursor(0,1);
  for (int i=0; i<6; i++) {
    lcd.print(buf_Meas[i], HEX);
    lcd.print("-");
  }
  delay(1000);
  lcd.clear();
  
  
  digitalWrite(LED_PIN, LOW);
  
  //ASK FOR READING ONLY ONCE
  Serial.write(message_Read, 4);
}

void loop() {
  if (Serial.available()) {
    lcd.setCursor(0,0);
    Serial.readBytes(buf_Read, 10);
    timeData = millis()/1000;
    concData = buf_Read[3]*256 + buf_Read[4];

    if (!digitalRead(DETACH_PIN)){
      digitalWrite(LED_PIN, HIGH);
      file.print(timeData);
      file.print(',');
      file.println(concData);
      file.sync();
    }
    else{
      file.close();
    }
    
    lcd.print(timeData);
    lcd.print(":");
    lcd.print(concData);
    lcd.print(" ppm");
    digitalWrite(LED_PIN, LOW);
    delay(500);
    Serial.write(message_Read, 4);  // ask for next measurement
    
  }

  else {
    //Serial.println("Waiting");
  }
}
