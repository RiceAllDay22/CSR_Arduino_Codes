/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------

IMPORTANT NOTE
  This device is EXTREMELY sensitive to electrostatic discharge
  NEVER touch the sensor UNLESS wearing Anti-Static Gloves
  Even if the sensor is not connected to power, still exercise extreme caution

INTRODUCTION
  This code is for the SCD30 sensor (black and green rectanglular shaped).

SPECIFICATIONS
  Measurement Range is 0 to 40000 ppm.
  Accuracy is ± (30 ppm + 3% MV)
  Current Consumption is 19 mA.

LINKS
  Product Info:     https://sensirion.com/products/catalog/SCD30/
  Arduino Library:  https://github.com/sparkfun/SparkFun_SCD30_Arduino_Library

*/

#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_SCD30
SCD30 airSensor;
uint32_t conc;

void setup() {
  Serial.begin(9600);
  Serial.println("SCD30 Example");
  Wire.begin();

  if (airSensor.begin() == false) {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1);
  }
  airSensor.setMeasurementInterval(2);            //Change number of seconds between measurements: 2 to 1800 (30 minutes)
  airSensor.setAltitudeCompensation(1300);        //Set altitude of the sensor in meters, stored in non-volatile memory of SCD30
  //airSensor.setForcedRecalibrationFactor(1000); //Force Calibration to a ppm value.

}

void loop() {
  if (airSensor.dataAvailable()) {
    conc = airSensor.getCO2();
    Serial.println(conc);
  }
  else {
    Serial.println("Waiting for new data");
  }
  delay(1000);
}
