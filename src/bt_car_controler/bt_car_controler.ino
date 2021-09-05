/* 
  Autor: Mateus Lima e Rafael Lima
  Descrição: Este é um código que possibilita o controle de um 
             carro bluetooth montado em arduino com uma shield 
             ponte H, através do protocolo serial
*/

// Libraries ------------------------------------------------------------------

#include <elapsedMillis.h>

// Constants ------------------------------------------------------------------

#define PINLED 13
#define PIN_BUZZER 12

#define PIN_MOTOR_BACK_1 9
#define PIN_MOTOR_BACK_2 6

#define PIN_MOTOR_FRONT_1 5
#define PIN_MOTOR_FRONT_2 3

#define PWD_SIGNAL_OFF 0

#define MAX_SPEED 100

// Global Vars ----------------------------------------------------------------

// Timers
elapsedMillis timerGlobal;
elapsedMillis timerLoop;
const int timeStep = 100; // ms

enum CarCommand : char
{
  MOVE_FRONT = 'F',
  MOVE_FRONT_LEFT = 'G',
  MOVE_FRONT_RIGHT = 'I',
  MOVE_BACK = 'B',
  MOVE_BACK_LEFT = 'H',
  MOVE_BACK_RIGHT = 'J',
  MOVE_RIGHT = 'L',
  MOVE_LEFT = 'R',
  DO_NOTHING = 'S',
};

enum CarDirection : char
{
  DIRECTION_RIGHT = 'R',
  DIRECTION_LEFT = 'L',
  DIRECTION_FRONT = 'F'
};

struct Car
{
  int Speed;
  int Acceleration;
  CarDirection Direction;
};

Car CarState;
const unsigned char SpeedTurn = 100;

char state;

// Core Functions --------------------------------------------------------------

/**
 * Setup - Run once at startup
 */
void setup() {
  
  Serial.begin(9600);

  pinMode(PINLED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_MOTOR_BACK_1, OUTPUT);
  pinMode(PIN_MOTOR_BACK_2, OUTPUT);
  pinMode(PIN_MOTOR_FRONT_1, OUTPUT);
  pinMode(PIN_MOTOR_FRONT_2, OUTPUT);

  analogWrite(PIN_MOTOR_BACK_1, PWD_SIGNAL_OFF);
  analogWrite(PIN_MOTOR_BACK_2, PWD_SIGNAL_OFF);

  analogWrite(PIN_MOTOR_FRONT_1, PWD_SIGNAL_OFF);
  analogWrite(PIN_MOTOR_FRONT_2, PWD_SIGNAL_OFF);

  // Set Car Start Info
  CarState.Speed = 0;
  CarState.Direction = DIRECTION_FRONT;
  CarState.Acceleration = -1;

  state = '*';

  // Reset Timers
  timerGlobal = 0;
}

/**
 * Loop - Keep reapeting foreaver
 */
void loop() {
  if( timerLoop >= timeStep ) // Control Frequency time
  {
    timerLoop = 0; // Reset Timer
    
//    if (Serial.available() > 0) {
//      state  = Serial.read();
//      Serial.write( state );
//    }
//  CarState = driverCommand( state );

    CarState = testCarSpeed( CarState );

    // Send Command to Actuators
    setCarSpeed( CarState.Speed );
    setCarDirection( CarState.Direction );

    printInfo( CarState );
  }
}

// Funções Auxiliares ----------------------------------------------

/**
 * Define direção
 */
void setCarDirection( const char direction)
{ 
  if( direction == DIRECTION_RIGHT )
  {
    analogWrite(PIN_MOTOR_FRONT_1, -SpeedTurn);
    analogWrite(PIN_MOTOR_FRONT_2, 0);
  }
  else if( direction == DIRECTION_LEFT )
  {
    analogWrite(PIN_MOTOR_FRONT_1, 0);
    analogWrite(PIN_MOTOR_FRONT_2, -SpeedTurn);
  }
  else
  {
    analogWrite(PIN_MOTOR_FRONT_1, 0);
    analogWrite(PIN_MOTOR_FRONT_2, 0);
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
    analogWrite(PIN_MOTOR_BACK_1, -pwdSignal );
    analogWrite(PIN_MOTOR_BACK_2, PWD_SIGNAL_OFF );
  }
  else if( speed < 0 )
  {
    analogWrite(PIN_MOTOR_BACK_1, PWD_SIGNAL_OFF );
    analogWrite(PIN_MOTOR_BACK_2, pwdSignal );
  }
  else
  {
    analogWrite(PIN_MOTOR_BACK_1, PWD_SIGNAL_OFF );
    analogWrite(PIN_MOTOR_BACK_2, PWD_SIGNAL_OFF );
  }
}

/**
 * Define Velocidade e Direção a partir do comando recebido
 */
Car driverCommand( const char command )
{
  Car carCommand;
  
  switch( command )
  {
    case MOVE_FRONT:
    {
      carCommand.Direction = DIRECTION_FRONT;
      carCommand.Speed = MAX_SPEED;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_FRONT_LEFT:
    {
      carCommand.Direction = DIRECTION_LEFT;
      carCommand.Speed = MAX_SPEED;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_LEFT:
    {
      carCommand.Direction = DIRECTION_LEFT;
      carCommand.Speed = 0;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_FRONT_RIGHT:
    {
      carCommand.Direction = DIRECTION_RIGHT;
      carCommand.Speed = MAX_SPEED;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_RIGHT:
    {
      carCommand.Direction = DIRECTION_RIGHT;
      carCommand.Speed = 0;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_BACK_LEFT:
    {
      carCommand.Direction = DIRECTION_LEFT;
      carCommand.Speed = -MAX_SPEED;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_BACK_RIGHT:
    {
      carCommand.Direction = DIRECTION_RIGHT;
      carCommand.Speed = -MAX_SPEED;
      carCommand.Acceleration = 0;
    }
    break;

    case DO_NOTHING:
    default:
    {
      carCommand.Direction = DIRECTION_FRONT;
      carCommand.Speed = 0;
      carCommand.Acceleration = 0;
    }
  }

  return carCommand;
}

void printInfo( Car carState )
{
  Serial.print( millis() );
  Serial.print(",");
  Serial.print( carState.Direction );
  Serial.print(",");
  Serial.print( carState.Speed );
  Serial.print(",");
  Serial.println( carState.Acceleration );
}

// Funções para Testes em Geral ----------------------------------------------

/**
 * Speed Sweep from -255 to 255
 */
Car testCarSpeed( Car carState )
{
  Car carCommand;
  
  const int maxSpeed = 255;

  int speedStep;

  if( carState.Speed >= maxSpeed)
  {
    speedStep = -1;
  }
  else if ( carState.Speed < -maxSpeed)
  {
    speedStep = 1;
  }
  else
  {
    speedStep = carState.Acceleration;
  }

  carCommand.Speed = carState.Speed + speedStep;
  carCommand.Acceleration = speedStep;
  carCommand.Direction = DIRECTION_FRONT;

  return carCommand;
}

/**
 * Change Direction each 256 ms
 */
Car testCarDirection()
{
  Car carCommand;

  long int currentTime = millis();

  if( currentTime & 0x100 )
  {
    carCommand.Direction = DIRECTION_LEFT;
  }
  else
  {
    carCommand.Direction = DIRECTION_RIGHT;
  }

  carCommand.Speed = 0;
  carCommand.Acceleration = 0;

  return carCommand;
}
