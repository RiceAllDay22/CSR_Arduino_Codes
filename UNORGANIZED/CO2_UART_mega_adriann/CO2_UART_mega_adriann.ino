const int RDY = 5;
const byte message[4] = {0x11, 0x01, 0x01, 0xED};
const byte messagethree[5] = {0x11, 0x02, 0x51, 0x1, 0x9B};
const byte messagefour[7] = {0x11, 0x04, 0x50, 0x0, 0x2, 0x2, 0x97}; 

byte buf[10];
byte buftwo[17];
byte bufthree[7];

void setup() {
  Serial1.begin(9600);
  Serial.begin(115200);

  Serial1.write(messagethree, 5);  // set to continuous mode
  Serial1.readBytes(buftwo, 17);

  Serial1.write(messagefour, 7);  // set to 2 second update?
  Serial1.readBytes(buf, 10);
  Serial1.write(message, 4);  // ask for measurement
}

void loop() {
  if (Serial1.available())
  {
    Serial1.readBytes(buf, 10);

    Serial.print(buf[3]*256 + buf[4]);
    Serial.println(" ppm");

    Serial1.write(message, 4);  // ask for next measurement
  }
}
