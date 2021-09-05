/* 
  Autor: Mateus Lima e Rafael Lima
  Descrição: Este é um código que possibilita o controle de um 
             carro bluetooth montado em arduino com uma shield 
             ponte H, através do protocolo serial
*/

#include <elapsedMillis.h>

#define PINLED 13

#define PIN_MOTOR_BACK_1 9
#define PIN_MOTOR_BACK_2 6

#define PIN_MOTOR_FRONT_1 5
#define PIN_MOTOR_FRONT_2 3

#define PWD_SIGNAL_OFF 255

elapsedMillis timerLoop;
const int timeStep = 100; // ms

elapsedMillis timerGlobal;

int speedMotor;
int speedStep;

unsigned char speedTurn = 100;

enum car_direction : char
{
  DIRECTION_RIGHT = 'R',
  DIRECTION_LEFT = 'L',
  DIRECTION_FRONT = 'F'
};

void setup() {
  
  Serial.begin(9600);

  pinMode(PINLED, OUTPUT);
  pinMode(PIN_MOTOR_BACK_1, OUTPUT);
  pinMode(PIN_MOTOR_BACK_2, OUTPUT);

  analogWrite(PIN_MOTOR_BACK_1, PWD_SIGNAL_OFF);
  analogWrite(PIN_MOTOR_BACK_2, PWD_SIGNAL_OFF);

  analogWrite(PIN_MOTOR_FRONT_1, PWD_SIGNAL_OFF);
  analogWrite(PIN_MOTOR_FRONT_2, PWD_SIGNAL_OFF);

  // Speed
  speedStep = -1;

  // Reset Timers
  timerGlobal = 0;
}

void loop() {
  if( timerLoop >= timeStep ) // Control Frequency time
  {
    // Reset Timer
    timerLoop = 0; 

    speedStep = (speedMotor >= 255)?-1:(speedMotor < -255) ?1:speedStep;
    speedMotor += speedStep;
    
    setCarSpeed( speedMotor );

    // 
    Serial.print( timerGlobal );
    Serial.print(",");
    Serial.println( speedMotor );
  }
}

/**
 * Define direção
 */
void setCarDirection( const char carDirection )
{
  if( carDirection == DIRECTION_RIGHT )
  {
    analogWrite(PIN_MOTOR_FRONT_1, speedTurn);
    analogWrite(PIN_MOTOR_FRONT_2, PWD_SIGNAL_OFF);
  }
  else if( carDirection == DIRECTION_LEFT )
  {
    analogWrite(PIN_MOTOR_FRONT_1, PWD_SIGNAL_OFF);
    analogWrite(PIN_MOTOR_FRONT_2, speedTurn);
  }
  else
  {
    analogWrite(PIN_MOTOR_FRONT_1, PWD_SIGNAL_OFF);
    analogWrite(PIN_MOTOR_FRONT_2, PWD_SIGNAL_OFF);
  }
}

/**
 * Regula Velocidade do Motor de Trás
 */
void setCarSpeed( const int speed )
{
  int pwdSignal = speed;

  if( speed > 0 )
  {
    analogWrite(PIN_MOTOR_BACK_1, -pwdSignal);
    analogWrite(PIN_MOTOR_BACK_2, PWD_SIGNAL_OFF);
  }
  else if( speed < 0 )
  {
    analogWrite(PIN_MOTOR_BACK_1, PWD_SIGNAL_OFF);
    analogWrite(PIN_MOTOR_BACK_2, pwdSignal);
  }
  else
  {
    analogWrite(PIN_MOTOR_BACK_1, PWD_SIGNAL_OFF);
    analogWrite(PIN_MOTOR_BACK_2, PWD_SIGNAL_OFF);
  }
}
