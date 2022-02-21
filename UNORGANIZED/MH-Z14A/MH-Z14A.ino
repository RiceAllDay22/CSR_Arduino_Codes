#include <SoftwareSerial.h>

const int analogPin = A0;
const int pwmPin = 9;

void setup() {
  Serial.begin(9600);
  pinMode(pwmPin, INPUT_PULLUP);
  Serial.println("Preheating");
  delay(10000); // preheat the CO2 sensor for 3 minutes

  Serial.println("Analog:,UART:,PWM:");
}

void loop() {
  int ppm_analog = get_analog();
  int ppm_PWM = gas_concentration_PWM();

  Serial.print(ppm_analog);
  Serial.print(",");
  Serial.println(ppm_PWM);
  
  delay(1000);
}


int get_analog() {
  float v = analogRead(analogPin) * 5.0 / 1023.0;
  int gas_concentration = int((v) * (5000/2));

  return gas_concentration;
}


int gas_concentration_PWM() {
  while (digitalRead(pwmPin) == LOW) {};
  long t0 = millis();
  while (digitalRead(pwmPin) == HIGH) {};
  long t1 = millis();
  while (digitalRead(pwmPin) == LOW) {};
  long t2 = millis();
  long tH = t1-t0;
  long tL = t2-t1;
  long ppm = 5000L * (tH - 2) / (tH + tL - 4);
  while (digitalRead(pwmPin) == HIGH) {};
  delay(10);
  
  return int(ppm);
}
