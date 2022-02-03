/*
  The following code is intended specifically for the CU-1106H-NS model and an Arduino Mega.
  Ensure that the sensor has been wired properly, before running this code.
  This code uses UART communication to send and recieve information to and from the CU-1106H-NS sensor.
  */

//CHANGEABLE PARAMETERS//
const int BUTTON_PIN = 2;       //Digital Pin number that the calibration button is attached to.
int buttonState = 0;            //State of the calibration button. 0 means it is not pushed. 1 means it is pushed.
int calValue = 1000;            //Calibration value that the sensor will be calibrated to when the button is pressed.
                                //calValue Range is 400 to 1500. DO NOT GO OUT OF RANGE
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//NOT CHANGEABLE UART COMMANDS//
const byte message_Read[4]  = {0x11, 0x01, 0x01, 0xED}; //Command to ask for the CO2 reading
byte response_Read[8];                                  //Response from sensor when message_Read is sent to it
byte response_Calib[4];                                 //Response from sensor when calibration is performed


//THIS IS THE SETUP CHUNK THAT THE ARDUINO WILL RUN ONCE
void setup() {
  //SETUP SERIAL PORTS
  Serial.begin(9600);       // This is for the Serial Monitor of the Arduino
  Serial1.begin(9600);      // This is for the sensor UART line of the sensor
  Serial.println("Seconds  -  CO2");

  
  //SETUP CALIBRATION BUTTON
  pinMode(BUTTON_PIN, INPUT);
  if ( (calValue < 400) or (calValue > 1500)){     //If the calibration value is out of range, an infinite loop will occur.
    Serial.println("CALIBRATION POINT IS OUT OF RANGE");
    while(1);
  }
  
}


//THIS IS THE MAIN LOOP THAT THE ARDUINO WILL RUN FOREVER
void loop() {
  readFunction();                         //Execute this function to read the information from the sensor
  
  buttonState = digitalRead(BUTTON_PIN);  //Check if the calibration button is pressed or not
  if (buttonState == HIGH){               //If it is pressed, then perform the calibration
    calibrateFunction();
  }

  delay(1000);                            //Wait for 1 second before repeating
}


//DEFINING THE FUNCTION TO TAKE MEASUREMENTS
void readFunction() {
  Serial1.write(message_Read, 4);         //Send the command to ask for a measurement
  delay(10);      
  if (Serial1.available()) {              //Check if the sensor's UART has information to give
    Serial1.readBytes(response_Read, 8);  //If there is information, then read it
    Serial.print(millis()/1000);
    Serial.print(" - ");
    Serial.print(response_Read[3]*256 + response_Read[4]);  //Convert the information to a CO2 ppm reading
    Serial.println(" ppm"); 
  }
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
