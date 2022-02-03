/*
  The following code is intended specifically for the CU-1106-C model.
  Ensure that the ssensor has been wired properly, before running this code.
  This code uses UART communication to send and recieve information to and from the CU-1106-C sensor.
 */

//CHANGEABLE PARAMETERS//
const int BUTTON_PIN = 2; //Digital Pin number that the calibration button is attached to.
int buttonState = 0;      //State of the calibration button. 0 means it is not pushed. 1 means it is pushed.
int calValue = 1200;       //Calibration value that the sensor will be calibrated to when the button is pressed. Range is 400 to 1500. DO NOT GO OUT OF RANGE
///////////////////////


//NOT CHANGEABLE UART COMMANDS//
const byte message_Cont[5]  = {0x11, 0x02, 0x51, 0x01,0x9B};              //Command to set device to continuous measurement mode
const byte message_Check[5] = {0x11, 0x01, 0x51, 0x9D};                   //Command to ask to check for measurement mode  
const byte message_Meas[7]  = {0x11, 0x04, 0x50, 0x00, 0x04, 0x02, 0x95}; //Command to set device to 4 second interval and smooth data of 2 
const byte message_Read[4]  = {0x11, 0x01, 0x01, 0xED};                   //Command to ask for CO2 reading

byte response_Cont[4];      //Response from sensor when message_Cont is sent to it
byte response_Check[5];     //Response from sensor when message_Check is sent to it
byte response_Meas[4];      //Response from sensor when message_Meas is sent to it
byte response_Read[8];      //Response from sensor when message_Read is sent to it
byte response_Calib[10];     //Response from sensor when calibration is performed


//THIS IS THE SETUP LOOP THAT THE ARDUINO WILL RUN ONCE
void setup() {
  
  //SETUP CALIBRATION PROCESS
  pinMode(BUTTON_PIN, INPUT);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), calibrateFunction, RISING);
  if ( (calValue < 400) or (calValue > 1500)){     //If the calibration value is out of range, an infinite loop will occur.
    while(1);
    Serial.println("CALIBRATION POINT IS OUT OF RANGE");
  }

  
  //SETUP SERIAL PORTS
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Begin");


//  //SET SENSOR TO CONTINUOUS MEASUREMENT MODE
//  Serial1.write(message_Cont, 5);
//  Serial1.readBytes(response_Cont, 4);
//  Serial.println("Continuous Set:");
//  for (int i=0; i<4; i++) {
//    Serial.print(response_Cont[i], HEX);
//    Serial.print("-");
//  }
//  Serial.println("");


//  //CHECK IF THE SENSOR IS IN CONTINUOUS MODE
//  Serial1.write(message_Check, 5);
//  Serial1.readBytes(response_Check, 5);
//  Serial.println("Continuous Check:");
//  for (int i=0; i<5; i++) {
//    Serial.print(response_Check[i], HEX);
//    Serial.print("-");
//  }
//  Serial.println("");


//  //SET THE MEASUREMENT PERIOD 
//  Serial1.write(message_Meas, 7); 
//  Serial1.readBytes(response_Meas, 4);
//  Serial.println("Measurement Set:");
//  for (int i=0; i<4; i++) {
//    Serial.print(response_Meas[i], HEX);
//    Serial.print("-");
//  }
//  Serial.println("");
  
  
  //BEGIN MEASUREMENTS
}



//THIS IS THE MAIN LOOP THAT THE ARDUINO WILL RUN FOREVER
void loop() {
  readFunction();                        //Execute this function to read the information from the sensor


  buttonState = digitalRead(BUTTON_PIN);  //Read if the button is pressed or not
  if (buttonState == HIGH){                //If it is pressed, then perform the calibration
    calibrateFunction();
  }

  delay(1000);     //Wait for 1 second   
}







//DEFINING THE FUNCTION TO TAKE MEASUREMENTS
void readFunction() {
  Serial1.write(message_Read, 4);   // ask for a measurement  
  if (Serial1.available()) {
    Serial1.readBytes(response_Read, 8);
    Serial.print(millis()/1000);
    Serial.print(":");
    Serial.print(response_Read[3]*256 + response_Read[4]);
    Serial.println(" ppm"); 
  }
}


//DEFINING THE FUNCTION TO PERFORM A CALIBRATION
void calibrateFunction() {
  int ppm = calValue;
  int DF0 = ppm/256;
  int DF1 = ppm - DF0*256;
  int CS = (256 - (0x11 +0x03 + 0x03 + DF0 + DF1)%256);
 
  byte command[6] = {0x11, 0x03, 0x03, DF0, DF1, CS};
  
  for (int i=0; i<6; i++) {
      Serial.print(command[i], HEX);
      Serial.print("-");
  }
  Serial.println("");
  Serial.println("Calibration Performed");

  Serial1.write(command, 6);
  Serial1.readBytes(response_Calib , 10);
  
  delay(1000);
}
