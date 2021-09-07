/**
 * 
 */

#define PIN_MOTOR_BACK_1 6
#define PIN_MOTOR_BACK_2 5
#define PIN_MOTOR_BACK_EN 9

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_MOTOR_BACK_1, OUTPUT);
  pinMode(PIN_MOTOR_BACK_2, OUTPUT);
  pinMode(PIN_MOTOR_BACK_EN, OUTPUT);

  pinMode(A0, INPUT);

  // Turn Motor Off
  analogWrite(PIN_MOTOR_BACK_1, 0);
  analogWrite(PIN_MOTOR_BACK_2, 0);
  analogWrite(PIN_MOTOR_BACK_EN, 0);

  Serial.begin(9600);
}

void loop() {
  int pwdsignal = 40;
  
  testBothDirections(pwdsignal);
  //testPWDM_SIGNAL(pwdsignal);
  //testePWM_EN(pwdsignal);
}

void testPWDM_SIGNAL( int pwdsignal )
{
    digitalWrite(PIN_MOTOR_BACK_1, 0 );
    analogWrite(PIN_MOTOR_BACK_2, pwdsignal );
    digitalWrite(PIN_MOTOR_BACK_EN, HIGH );

    delay(400);

    Serial.print(millis());
    Serial.print(" ");
    Serial.println(pwdsignal);
}

void testePWM_EN( int pwdsignal )
{
    digitalWrite(PIN_MOTOR_BACK_1, 0 );
    digitalWrite(PIN_MOTOR_BACK_2, HIGH );

    analogWrite(PIN_MOTOR_BACK_EN, pwdsignal );
    
    Serial.print(millis());
    Serial.print(" ");
    Serial.println(pwdsignal);

    delay(100);
}

void testBothDirections( int pwdsignal )
{
    digitalWrite(PIN_MOTOR_BACK_1, 0 );
    analogWrite(PIN_MOTOR_BACK_2, pwdsignal );
    digitalWrite(PIN_MOTOR_BACK_EN, HIGH );

    Serial.print(millis());
    Serial.print(" ");
    Serial.println(pwdsignal);
    
    delay(2000);

    digitalWrite(PIN_MOTOR_BACK_2, 0 );
    digitalWrite(PIN_MOTOR_BACK_1, 0 );
    digitalWrite(PIN_MOTOR_BACK_EN, 0 );

    Serial.print(millis());
    Serial.print(" ");
    Serial.println(0);
    
    delay(1000);

    analogWrite(PIN_MOTOR_BACK_1, pwdsignal );
    digitalWrite(PIN_MOTOR_BACK_2, 0 );
    digitalWrite(PIN_MOTOR_BACK_EN, HIGH );

    Serial.print(millis());
    Serial.print(" ");
    Serial.println(-pwdsignal);
    
    delay(2000);

    digitalWrite(PIN_MOTOR_BACK_2, 0 );
    digitalWrite(PIN_MOTOR_BACK_1, 0 );
    digitalWrite(PIN_MOTOR_BACK_EN, 0 );

    Serial.print(millis());
    Serial.print(" ");
    Serial.println(0);
    
    delay(1000);
}
