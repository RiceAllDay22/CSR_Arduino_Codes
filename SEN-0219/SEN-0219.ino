/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------

INTRODUCTION
  This code is for the black and gold Gravity Infrared CO2 Sensor (also called the SEN-0219)
  Ensure that the wiring is correct before running the code and powering the Arduino.

WIRING
  Sensor -> Arduino
  ------------------------
  BLACK wire  ->  GND   Port
  RED wire    ->  5.0V  Port
  BLUE wire   ->  A0    Port   


CALIBRATION
  Ask Adriann for a tutorial.

IMPORTANT NOTES


SPECIFICATIONS
  Collection Frequency is normally set to 1 second but can be increased or decreased.
  Measurement Range is 0 to 5000 ppm.
  Accuracy is ± (50 ppm + 3% MV)
  Current Consumption is 150 mA.

LINKS
  Product Info: https://wiki.dfrobot.com/Gravity__Analog_Infrared_CO2_Sensor_For_Arduino_SKU__SEN0219
  

*/


int sensorIn = A0;

void setup(){
  Serial.begin(9600);
  analogReference(DEFAULT);   // Set the default voltage of the reference voltage
}

void loop(){
  int sensorValue = analogRead(sensorIn);     // Read analog signal

  float voltage = sensorValue*(5000/1024.0);  // Convert analog signal to a voltage
  if(voltage == 0) {
    Serial.println("ERROR");
  }
  else if(voltage < 400) {
    Serial.println("Still Preheating");
  }
  else {
    int voltage_diference = voltage-400;                  //Calculate voltage difference
    float conc = voltage_diference*50.0/16.0;    //Convert voltage difference to concentration
    Serial.println(conc);
  }
  delay(1000);
}
