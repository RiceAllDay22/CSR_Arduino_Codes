/*
  ---------- PLEASE READ THIS BEFORE POWERING THE SENSOR ----------

INTRODUCTION
  This code is for the MH-Z16 sensor (black cylinder shaped)
  The sensor does not need to be connected to an adaptor
  Ensure that the wiring is correct before running the code and powering the Arduino

WIRING
  Black Sensor -> Arduino
  ------------------------
  WHITE  wire     -> 5.0V Port
  1st BLACK  wire -> GND Port
  BLUE wire       -> Digital Port # 2
  YELLOW wire     -> Digital Port # 5
  2nd BLACK  wire -> GND Port

CALIBRATION
  This sensor is connected to a black switch module.
  The switch is currently positioned towards the yellow wire.
  Once the sensor has been in a stable condition, move the switch towards the black wire.
  The sensor will then start giving readings around 400 ppm.
  Keep the switch on this position for at least 10 seconds.
  Then move the switch back towards the yellow wire.
  Using this switch will always force the sensor to give 400 ppm readings. 

IMPORTANT NOTES
  Disregard the first data reading that is given.
  The first data reading is sometimes incorrect and invalid.

SPECIFICATIONS
  Collection Frequency is normally set to 1 second but can be increased or decreased.
  Measurement Range is 0 to 10000 ppm.
  Accuracy is ± (100 ppm + 5% MV)
  Current Consumption is 60 mA.

LINKS
  Product Info:     https://sandboxelectronics.com/?product=mh-z16-ndir-co2-sensor-with-i2cuart-5v3-3v-interface-for-arduinoraspeberry-pi
  Arduino Library:  https://github.com/SandboxElectronics/NDIR (Use the one that is titled NDIR_I2C)

*/



//IMPORT LIBRARIES AND SETUP VARIABLES
const byte interruptPin   = 2;
const byte calibrationPin = 5;
volatile byte state = LOW;
unsigned long rise_time;
bool started = false;
uint32_t conc;

//SETUP PINS & MODULES
void setup() {
  Serial.begin(9600);   // This is the baud rate. Keep at 9600, no need to change
  Serial.println("Begin");
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(calibrationPin, OUTPUT);
  digitalWrite(calibrationPin, HIGH);
  state = digitalRead(interruptPin);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changed, CHANGE);
}

// MAIN CODE
void loop() {
  
}




// FUNCTION FOR MEASURING CONCENTRATION
// Please do not change anything contained in this function.
void changed() {
  state = !state;

  if (state) {                      // rising edge
    rise_time = micros();
    if (!started) {
      started = true;
    }
  }
  else if (started) {               // falling edge
    unsigned long total_time = micros() - rise_time;
    if (total_time < 0) {
      total_time += 4294967296;
    }
    conc = (total_time-2000)/100;   
    Serial.println(conc);
  }
}
