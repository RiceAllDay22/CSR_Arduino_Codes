/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------

IMPORTANT NOTE
  This device is EXTREMELY sensitive to electrostatic discharge
  NEVER touch the sensor UNLESS wearing Anti-Static Gloves
  Even if the sensor is not connected to power, still exercise extreme caution

INTRODUCTION
  This code is for the SCD30 sensor (black and green rectanglular shaped).
  This sensor is currently connected to an Arduino Nano.
  A Nano is essentially a miniture version of an Arduino Uno.
  Both the Nano and Uno are functionally identical.

WIRING
  Sensor -> Arduino Nano
  ------------------------
  Vin Pin -> 5.0V Port
  Gnd Pin -> GND  Port
  SCL Pin -> SCL Port
  SDA Pin -> SDA Port

CALIBRATION
  There is currently no external button to calibrate this module.
  Can be implemented if necessary.

SPECIFICATIONS
  Collection Frequency is normally set to 3 seconds, but can be increased. NOT decreased.
  Measurement Range is 0 to 40000 ppm.
  Accuracy is ± (30 ppm + 3% MV)
  Current Consumption is 19 mA.

LINKS
  Product Info:     https://www.sensirion.com/en/environmental-sensors/carbon-dioxide-sensors/carbon-dioxide-sensors-scd30/
  Arduino Library:  https://github.com/sparkfun/SparkFun_SCD30_Arduino_Library



*/

#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_SCD30
SCD30 airSensor;
uint32_t conc;
const byte BUTTON_PIN   = 5;

void setup() {
  Serial.begin(9600);
  Serial.println("SCD30 Example");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Wire.begin();

  if (airSensor.begin() == false) {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1);
  }
  airSensor.setMeasurementInterval(2); //Change number of seconds between measurements: 2 to 1800 (30 minutes)
  airSensor.setAltitudeCompensation(1300); //Set altitude of the sensor in m, stored in non-volatile memory of SCD30
  //airSensor.setForcedRecalibrationFactor(1000);

  //The SCD30 has data ready every two seconds
}

void loop() {
  if (airSensor.dataAvailable()) {
    conc = airSensor.getCO2();
    Serial.print(digitalRead(BUTTON_PIN));
    Serial.print(",");
    Serial.println(conc);
  }
  else {
    Serial.println("Waiting for new data");
  }

  if (digitalRead(BUTTON_PIN) == 1) {
    airSensor.setForcedRecalibrationFactor(1000);
  }
  delay(3000);    //Wait for 3000 milliseconds in between data measurements
}
