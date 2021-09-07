/* 
  Autor: Mateus Lima e Rafael Lima
  Descrição: Este é um código que possibilita o controle de um 
             carro bluetooth montado em arduino com uma shield 
             ponte H, através do protocolo serial
*/

// Libraries ------------------------------------------------------------------

#include <Servo.h>
#include <elapsedMillis.h>

// Constants ------------------------------------------------------------------

#define PIN_LED 13
#define PIN_BUZZER 13

#define PIN_MOTOR_BACK_1 5
#define PIN_MOTOR_BACK_2 6
#define PIN_MOTOR_BACK_EN 9

#define PIN_MOTOR_FRONT 11

#define ANGLE_TURN_CENTER 90
#define ANGLE_TURN_LEFT 50
#define ANGLE_TURN_RIGHT 110

#define MAX_SPEED 100

// Global Vars ----------------------------------------------------------------

// Timers
elapsedMillis timerGlobal;
elapsedMillis timerLoop;
const int timeStep = 100; // ms

Servo servoFront;

// Car Commands ---------------------------------------------------------------

enum CarCommand : char
{
  // Change Speed
  SET_SPEED1 = '1',
  SET_SPEED2 = '2',
  SET_SPEED3 = '3',
  SET_SPEED4 = '4',
  SET_SPEED5 = '5',
  SET_SPEED6 = '6',
  SET_SPEED7 = '7',
  SET_SPEED8 = '8',
  SET_SPEED9 = '9',
  SET_SPEED0 = '0',

  // Move
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
  char State;
  int Speed;
  int Acceleration;
  char Direction;
};

Car CarState;
int Speed;
char state;

// Core Functions --------------------------------------------------------------

/**
 * Setup - Run once at startup
 */
void setup() {
  // Start PIN
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_MOTOR_BACK_1, OUTPUT);
  pinMode(PIN_MOTOR_BACK_2, OUTPUT);
  pinMode(PIN_MOTOR_BACK_EN, OUTPUT);

  // Turn Motor Off
  analogWrite(PIN_MOTOR_BACK_1, 0);
  analogWrite(PIN_MOTOR_BACK_2, 0);
  analogWrite(PIN_MOTOR_BACK_EN, 0);

  // Start Servo
  servoFront.attach(PIN_MOTOR_FRONT);
  servoFront.write(ANGLE_TURN_CENTER);

  // Set Car Start Info
  CarState.Speed = 0;
  CarState.Direction = DIRECTION_FRONT;
  CarState.Acceleration = -1;

  Speed = MAX_SPEED;
  state = DO_NOTHING;

  // Start Communication
  Serial.begin(9600);

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
  
    if (Serial.available() > 0) {
      state  = Serial.read();
      Serial.write( state );
    }
  
    CarState = driverCommand( state );
    //CarState = driverCommand( DO_NOTHING );
    //CarState = testCarSpeed( CarState );
    
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
    servoFront.write(ANGLE_TURN_RIGHT);
  }
  else if( direction == DIRECTION_LEFT )
  {
    servoFront.write(ANGLE_TURN_LEFT);
  }
  else
  {
    servoFront.write(ANGLE_TURN_CENTER);
  }
}

/**
 * Regula Velocidade do Motor de Trás
 */
void setCarSpeed( const int speed )
{
  int pwdSignal = (speed <= MAX_SPEED)? speed: MAX_SPEED;

  if( speed > 0 )
  {
    analogWrite(PIN_MOTOR_BACK_1, pwdSignal );
    analogWrite(PIN_MOTOR_BACK_2, 0 );
    analogWrite(PIN_MOTOR_BACK_EN, 255 );
  }
  else if( speed < 0 )
  {
    analogWrite(PIN_MOTOR_BACK_1, 0 );
    analogWrite(PIN_MOTOR_BACK_2, pwdSignal );
    analogWrite(PIN_MOTOR_BACK_EN, 255 );
  }
  else
  {
    analogWrite(PIN_MOTOR_BACK_1, 0 );
    analogWrite(PIN_MOTOR_BACK_2, 0 );
    analogWrite(PIN_MOTOR_BACK_EN, 0 );
  }
}

/**
 * Define Velocidade e Direção a partir do comando recebido
 */
Car driverCommand( const char command )
{
  Car carCommand;
  carCommand.State = command;
  
  switch( command )
  {
    case SET_SPEED1:
    {
      carCommand.Direction = CarState.Direction;
      Speed = MAX_SPEED/10;
    }
    break;

    case SET_SPEED2:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 2*MAX_SPEED/10;
    }
    break;

    case SET_SPEED3:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 3*MAX_SPEED/10;
    }
    break;

    case SET_SPEED4:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 4*MAX_SPEED/10;
    }
    break;

    case SET_SPEED5:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 5*MAX_SPEED/10;
    }
    break;

    case SET_SPEED6:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 6*MAX_SPEED/10;
    }
    break;

    case SET_SPEED7:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 7*MAX_SPEED/10;
    }
    break;

    case SET_SPEED8:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 8*MAX_SPEED/10;
    }
    break;

    case SET_SPEED9:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 9*MAX_SPEED/10;
    }
    break;

    case SET_SPEED0:
    {
      carCommand.Direction = CarState.Direction;
      Speed = 0;
    }
    break;

    case MOVE_FRONT:
    {
      carCommand.Direction = DIRECTION_FRONT;
      carCommand.Speed = Speed;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_FRONT_LEFT:
    {
      carCommand.Direction = DIRECTION_LEFT;
      carCommand.Speed = Speed;
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
      carCommand.Speed = Speed;
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

    case MOVE_BACK:
    {
      carCommand.Direction = DIRECTION_FRONT;
      carCommand.Speed = -Speed;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_BACK_LEFT:
    {
      carCommand.Direction = DIRECTION_LEFT;
      carCommand.Speed = -Speed;
      carCommand.Acceleration = 0;
    }
    break;

    case MOVE_BACK_RIGHT:
    {
      carCommand.Direction = DIRECTION_RIGHT;
      carCommand.Speed = -Speed;
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
  Serial.print( carState.State );
  Serial.print(",");
  Serial.print( carState.Direction );
  Serial.print(",");
  Serial.print( carState.Speed );
  Serial.print(",");
  Serial.println( carState.Acceleration );
}

// Funções para Testes em Geral ----------------------------------------------

/**
 * Speed Sweep from -MAX_SPEED to MAX_SPEED
 */
Car testCarSpeed( Car carState )
{
  Car carCommand;
  
  const int maxSpeed = MAX_SPEED;

  int speedStep;

  if( carState.Speed >= MAX_SPEED)
  {
    speedStep = -1;
  }
  else if ( carState.Speed < -MAX_SPEED)
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
 * Speed Sweep from -255 to 255
 */
 void testServoMotor()
 {
    int pos;
    for( pos=ANGLE_TURN_LEFT;pos<=ANGLE_TURN_RIGHT;pos += 1 )
    {
      servoFront.write(pos);
      delay(10);
    }

    for( pos=ANGLE_TURN_RIGHT;pos>=ANGLE_TURN_LEFT;pos -= 1 )
    {
      servoFront.write(pos);
      delay(10);
    }
 }

/**
 * Change Direction each 4096 ms
 */
Car testCarDirection()
{
  Car carCommand;

  long int currentTime = millis();

  if( currentTime & 0x1000 )
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
