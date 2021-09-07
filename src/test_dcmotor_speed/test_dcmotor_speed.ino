#include <elapsedMillis.h>

#define PINLED 13

#define PINMOTOR_EN 9
#define PINMOTOR1 6
#define PINMOTOR2 5

#define MAX_SPEED 100
#define SPEED_ACC 500

elapsedMillis timerLoop;
const int timeStep = 100; // ms

elapsedMillis timerGlobal;

int speedMotor,speedStep;

void setup() {
  
  Serial.begin(9600);

  pinMode(PINLED, OUTPUT);
  pinMode(PINMOTOR1, OUTPUT);
  pinMode(PINMOTOR2, OUTPUT);
  pinMode(PINMOTOR_EN, OUTPUT);

  digitalWrite(PINMOTOR_EN,HIGH);
  analogWrite(PINMOTOR1,0);
  analogWrite(PINMOTOR2,0);

  speedStep = 1;

  timerGlobal = 0;
}

void loop() {
  if( timerLoop >= timeStep ) // Control Frequency time
  {
    // Reset Timer
    timerLoop = 0; 

    speedStep = (speedMotor >=MAX_SPEED)?-1:(speedMotor < -MAX_SPEED) ?1:speedStep;
    speedMotor +=speedStep;

    if( speedMotor > 0 )
    {
      analogWrite(PINMOTOR1,speedMotor);
      analogWrite(PINMOTOR2,0);
    }
    else
    {
      analogWrite(PINMOTOR1,0);
      analogWrite(PINMOTOR2,10-speedMotor );
    }

    // 
    Serial.print(timerGlobal);
    Serial.print(";");
    Serial.println(speedMotor);
  }
}
