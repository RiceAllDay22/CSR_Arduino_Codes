int VaneValue;      // Raw value given by the wind vane
int Direction;      // Converting raw value into a direction
int CalDirection;   // Adding specified offset to the direction

int WindSpeedPin = 2;       //Digital Pin 2
int WindDirectionPin = A3;  //Analog Pin 3

volatile unsigned long Rotations;             
volatile unsigned long ContactBounceTime;

float WindSpeed;
#define Offset 0;          //Specified degree offset for the wind direction

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
  pinMode(WindSpeedPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(WindSpeedPin), isr_rotation, FALLING);
  interrupts();
   
}

void loop() {
  //GET WIND DIRECTION
  VaneValue = analogRead(WindDirectionPin);

  Direction = map(VaneValue, 0, 1023, 1, 360);    //Use this one if power is 5.0 V
  //Direction = map(VaneValue, 0, 813, 1, 360);   //Use this one if power is 3.3 V

  CalDirection = Direction + Offset;

  if (CalDirection > 360)
    CalDirection = CalDirection - 360;
  if (CalDirection < 1)
    CalDirection = CalDirection + 360;

  
  //PRINT RESULTS
  Serial.print(CalDirection);
  Serial.print(';');
  Serial.println(Rotations);

  //DELAY and Reset the number of rotations
  Rotations = 0;
  delay(1000);
}


// FUNCTION FOR COUNTING WIND CUP ROTATIONS
void isr_rotation () {
  if ((millis() - ContactBounceTime) > 15 ) {
    Rotations ++;
    ContactBounceTime = millis();
  }
}     
