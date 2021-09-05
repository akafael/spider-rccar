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

#define PWD_SIGNAL_OFF 255

#define MAX_SPEED 100

elapsedMillis timerGlobal;
elapsedMillis timerLoop;
const int timeStep = 100; // ms

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

int speedMotor;
int speedAcc;
char carDirection;

unsigned char speedTurn = 100;

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

  // Speed
  speedAcc = -1;
  carDirection = DIRECTION_FRONT;

  // Reset Timers
  timerGlobal = 0;
}

void loop() {
  if( timerLoop >= timeStep ) // Control Frequency time
  {
    timerLoop = 0; // Reset Timer
    
    setCarSpeed( 0 );
    setCarDirection( DIRECTION_RIGHT, 0 );

    printCarInfo();
  }
}

// Funções Auxiliares ----------------------------------------------

/**
 * Define direção
 */
void setCarDirection( const char direction, int speedTurn )
{
  if( direction == DIRECTION_RIGHT )
  {
    analogWrite(PIN_MOTOR_FRONT_1, -speedTurn);
    analogWrite(PIN_MOTOR_FRONT_2, 0);
  }
  else if( direction == DIRECTION_LEFT )
  {
    analogWrite(PIN_MOTOR_FRONT_1, 0);
    analogWrite(PIN_MOTOR_FRONT_2, -speedTurn);
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

/**
 * Define Velocidade e Direção a partir do comando recebido
 */
Car driverCommand( const char command )
{
  Car carCommand;
  
  switch( command )
  {
    case 'F': // Move Front
    {
      carCommand.Direction = DIRECTION_FRONT;
      carCommand.Speed = MAX_SPEED;
      carCommand.Acceleration = speedAcc;
    }
    break;

    case 'G': // Move Front Left
    {
      carCommand.Direction = DIRECTION_LEFT;
      carCommand.Speed = MAX_SPEED;
      carCommand.Acceleration = speedAcc;
    }
    break;

    case 'L': // Move Left
    {
      carCommand.Direction = DIRECTION_LEFT;
      carCommand.Speed = 0;
      carCommand.Acceleration = speedAcc;
    }
    break;

    case 'I': // Move Front Right
    {
      carCommand.Direction = DIRECTION_RIGHT;
      carCommand.Speed = MAX_SPEED;
      carCommand.Acceleration = speedAcc;
    }
    break;

    case 'R': // Move Right
    {
      carCommand.Direction = DIRECTION_RIGHT;
      carCommand.Speed = 0;
      carCommand.Acceleration = speedAcc;
    }
    break;

    case 'H': // Move Backwards Left
    {
      carCommand.Direction = DIRECTION_LEFT;
      carCommand.Speed = -MAX_SPEED;
      carCommand.Acceleration = speedAcc;
    }
    break;

    case 'J': // Move Backwards Right
    {
      carCommand.Direction = DIRECTION_RIGHT;
      carCommand.Speed = -MAX_SPEED;
      carCommand.Acceleration = speedAcc;
    }
    break;

    default:
    {
      carCommand.Direction = DIRECTION_FRONT;
      carCommand.Speed = 0;
      carCommand.Acceleration = 0;
    }
  }

  return carCommand;
}

void printCarInfo()
{
  Serial.print( millis() );
  Serial.print(",");
  Serial.print( timerGlobal );
  Serial.print(",");
  Serial.print( carDirection );
  Serial.print(",");
  Serial.println( speedMotor );
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

void testCarSpeed()
{
  const int maxSpeed = 255;

  // Set Acceleration
  speedAcc = (speedMotor >= maxSpeed)?-1:(speedMotor < -maxSpeed) ?1:speedAcc;
  speedMotor += speedAcc;

  setCarSpeed( speedMotor );
}
