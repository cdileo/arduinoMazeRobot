/*
 *Group of 4 - Motor Testing
 * Chris Dileo
 * Brendon Hodaly
 * Pankhil Kainth
 * Natalie Ngan
 * Steven Kim
*/
#include <NewPing.h>
/*Define Motor Pins. R/L indicates left/right motor and F/R indicates whether 
                  the robot moves forward or reverse when powered on pin.*/
#define motorRF 10 //A-iB
#define motorRR 9 //A-iA
#define motorLF 8 
#define motorLR 7 
#define touchSensor 13
#define TRIGGER_PIN_L 5
#define ECHO_PIN_L 6
#define TRIGGER_PIN_R 3
#define ECHO_PIN_R 4
#define MAX_DISTANCE 200
#define THRESHOLD 20
#define TURN_TIME 450 //in ms
#define THINKING_TIME 1000 //in ms 
#define BACK_TIME 300 //ms
#define MOTOR_SPEED 255 // 0-255

NewPing sonarL(TRIGGER_PIN_L, ECHO_PIN_L, MAX_DISTANCE);
NewPing sonarR(TRIGGER_PIN_R, ECHO_PIN_R, MAX_DISTANCE);

void setup() {
  pinMode(motorRF, OUTPUT);
  pinMode(motorRR, OUTPUT);
  pinMode(motorLF, OUTPUT);
  pinMode(motorLR, OUTPUT);
  pinMode(touchSensor, INPUT);
  Serial.begin(9600); // Start reading form our touch sensor
  pinMode(TRIGGER_PIN_L, OUTPUT);
  pinMode(ECHO_PIN_L, INPUT);
  pinMode(TRIGGER_PIN_R, OUTPUT);
  pinMode(ECHO_PIN_R, INPUT);
}

void loop(){ //starts random motor movement for testing
  // TODO: make a state machine for robot behaviours
//  if (readLeftSensor() > 20) {
//    turnLeft();
//  } else if (readRightSensor() > 20) {
//    turnRight();
//  } else {
//    goForward();
//  }

  goForward();
  delay(50); //TODO: remove me, just testing a force forward
  //check for a bump
  Serial.println(digitalRead(touchSensor));
  if (checkTouch() == 0) {
    backUpAndLookForHole();
  }
}

int checkTouch() {
  return (int)digitalRead(touchSensor);
}

void backUpAndLookForHole() {
  //read left and right sensors
  int distLeft;
  int distRight;
  stopMotors();
  delay(THINKING_TIME);
  goBackward();
  delay(BACK_TIME);
  
  while (true) {
    distLeft = readLeftSensor();
    distRight = readRightSensor();
    if (distLeft > distRight && distLeft > THRESHOLD){
      goBackward();
      delay(BACK_TIME);
      turnLeft(255);
      delay(TURN_TIME);
      break;
    } else if (distRight > distLeft && distRight > THRESHOLD){
      goBackward();
      delay(BACK_TIME);
      turnRight(255);
      delay(TURN_TIME);
      break;
    } else {
      goBackward();
      delay(BACK_TIME);
      //runaway possibililty
    }
  }  
  stopAndThink();
}

void stopAndThink() {
  stopMotors();
  delay(THINKING_TIME);
}

int readLeftSensor() {
  digitalWrite(TRIGGER_PIN_L, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER_PIN_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_L, LOW); 

  int duration = pulseIn(ECHO_PIN_L, HIGH);
  return duration*0.034/2;
}
int readRightSensor() {
  digitalWrite(TRIGGER_PIN_R, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER_PIN_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_R, LOW); 

  int duration = pulseIn(ECHO_PIN_R, HIGH);
  return duration*0.034/2;
}


void stopMotors(){
  // To Avoid doubling up signals on a motor, stop everything between motor behaviours
  analogWrite(motorRF, 0); 
  analogWrite(motorLF, 0); 
  analogWrite(motorRR, 0); 
  analogWrite(motorLR, 0); 
}

void goForward() {
  stopMotors();
  analogWrite(motorRF, MOTOR_SPEED); 
  analogWrite(motorLF, MOTOR_SPEED); 
}

void goBackward() {
  stopMotors();
  analogWrite(motorRR, MOTOR_SPEED); 
  analogWrite(motorLR, MOTOR_SPEED); 
}

void turnLeft(int speed) {
  stopMotors();
  analogWrite(motorRF, speed); 
//  analogWrite(motorLR, MOTOR_SPEED); // Enable me for a tighter turn
}

void turnRight(int speed) {
  stopMotors();
//  analogWrite(motorRR, MOTOR_SPEED); // Enable me for a tighter turn
  analogWrite(motorLF, speed); 
}


