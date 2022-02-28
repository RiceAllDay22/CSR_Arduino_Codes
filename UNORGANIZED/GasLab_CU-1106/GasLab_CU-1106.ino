/*
  Works for all CU-1106 models that have UART communication
 */

const byte message_Version[4] = {0x11, 0x01, 0x1E, 0xD0};                   //Command to read the software version of device
const byte message_Serial[4]  = {0x11, 0x01, 0x1F, 0xCF};                   //Command to read the serial number of device
const byte message_Cont[5]    = {0x11, 0x02, 0x51, 0x01,0x9B};              //Command to set device to continuous power mode
const byte message_Check[5]   = {0x11, 0x01, 0x51, 0x9D};                   //Command to ask to check for measurement mode  
const byte message_Meas[7]    = {0x11, 0x04, 0x50, 0x00, 0x02, 0x02, 0x97}; //Command to set device to 2 second interval and smooth data of 2 
const byte message_Read[4]    = {0x11, 0x01, 0x01, 0xED};                   //Command to ask for CO2 reading

byte response_Version[17];   //Response from sensor when message_Version is sent to it
byte response_Serial[16];    //Response from sensor when message_Serial is sent to it
byte response_Cont[6];       //Response from sensor when message_Cont is sent to it
byte response_Check[5];     //Response from sensor when message_Check is sent to it
byte response_Meas[6];       //Response from sensor when message_Meas is sent to it
byte response_Read[10];      //Response from sensor when message_Read is sent to it


void setup() {
  
  //SETUP SERIAL PORTS
  Serial.begin(9600);
  Serial1.begin(9600);

  //CHECK SOFTWARE VERSION OF DEVICE
  Serial1.write(message_Version, 4);
  Serial1.readBytes(response_Version, 17);
  Serial.println("Software Version:");
  for (int i=0; i<17; i++) {
    Serial.print(response_Version[i], HEX);
    Serial.print("-");
  }
  Serial.println("");

  //CHECK SERIAL NUMBER OF DEVICE
  Serial1.write(message_Serial, 4);
  Serial1.readBytes(response_Serial, 16);
  Serial.println("Device Serial:");
  for (int i=0; i<16; i++) {
    Serial.print(response_Serial[i], HEX);
    Serial.print("-");
  }
  Serial.println("");


  //SET TO CONTINUOUS MODE
  Serial1.write(message_Cont, 5);
  Serial1.readBytes(response_Cont, 6);
  Serial.println("Continuous Set:");
  for (int i=0; i<6; i++) {
    Serial.print(response_Cont[i], HEX);
    Serial.print("-");
  }
  Serial.println("");


  //SET MEASUREMENT PERIOD
  Serial1.write(message_Meas, 7); 
  Serial1.readBytes(response_Meas, 6);
  Serial.println("Measurement Set:");
  for (int i=0; i<6; i++) {
    Serial.print(response_Meas[i], HEX);
    Serial.print("-");
  }
  Serial.println("");
  
  
  
  //ASK FOR READING ONLY ONCE
  Serial1.write(message_Read, 4);
}

void loop() {
  if (Serial1.available()) {
    Serial1.readBytes(response_Read, 10);
    Serial.print(millis()/1000);
    Serial.print(":");
    Serial.print(response_Read[3]*256 + response_Read[4]);
    Serial.println(" ppm");
    Serial1.write(message_Read, 4);  // ask for next measurement
  }

  else {
    //Serial.println("Waiting");
  }
}



//DEFINING THE FUNCTION TO TAKE MEASUREMENTS
void readFunction() {
  if (Serial1.available()) {
    Serial1.readBytes(response_Read, 8);
    Serial.print(millis()/1000);
    Serial.print(":");
    Serial.print(response_Read[3]*256 + response_Read[4]);
    Serial.println(" ppm");
    Serial1.write(message_Read, 4);  // ask for next measurement  
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
}
