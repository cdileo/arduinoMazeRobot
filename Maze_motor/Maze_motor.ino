/*
 *Group of 4 - Motor Testing
 * Chris Dileo
 * Brendon Hodaly
 * Pankhil Kainth
 * Natalie Ngan
 * Steven Kim
*/

#include <NewPing.h> // For enabling our ultrasonic sensors
/*Define Motor Pins. R/L indicates left/right motor and F/R indicates whether 
                  the robot moves forward or reverse when powered on pin.*/
#define motorRF 10 //A-iB
#define motorRR 9 //A-iA
#define motorLF 8 
#define motorLR 7 
#define touchSensor 6

#define R_TRIGGER_PIN  1
#define R_ECHO_PIN     2
#define L_TRIGGER_PIN  3
#define L_ECHO_PIN     4
#define MAX_DISTANCE 200
#define SENSOR_DISTANCE_THRESHOLD 5
 
NewPing r_sonar(R_TRIGGER_PIN, R_ECHO_PIN, MAX_DISTANCE);
NewPing l_sonar(L_TRIGGER_PIN, L_ECHO_PIN, MAX_DISTANCE);

float distToRight = -1.0;
float distToLeft = -1.0;

void setup() {
  pinMode(motorRF, OUTPUT);
  pinMode(motorRR, OUTPUT);
  pinMode(motorLF, OUTPUT);
  pinMode(motorLR, OUTPUT);
  pinMode(touchSensor, INPUT);
  Serial.begin(9600); // Start reading form our touch sensor
}

void loop(){ //starts random motor movement for testing
  // TODO: make a state machine for robot behaviours
//  testMovement();
  //Navigation code
  //Assuming one ultrasonic sensor on L and R sides, each loop, take a reading of distances
  distToRight = getSensorReading('R');
  distToLeft = getSensorReading('L');

  // If one side is farther +/- a threshold value, move towards it
  if (distToRight > distToLeft + SENSOR_DISTANCE_THRESHOLD) {
    // TODO: Turn Right? Or should we try to gradually turn?
    turnRight();
  } else if (distToRight + SENSOR_DISTANCE_THRESHOLD < distToLeft) {
    turnLeft();
  } else {
    // Keep going straight
    goForward();
  }
  
}

float getSensorReading(char side) {
  float value = -1.0;
  if (side == 'L'){
    value = l_sonar.ping_cm();
  } else if (side == 'R') {
    value = r_sonar.ping_cm();
  }
  return value;
}

void testMovement() {
  goForward();
  delay(1000);
  turnLeft();
  delay(1000);
  goForward();
  delay(1000);
  turnRight();
  delay(1000);
  stopMotors();
  delay(1000);
  goBackward();
  delay(1000);
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
  analogWrite(motorRF, 255); 
  analogWrite(motorLF, 255); 
}

void goBackward() {
  stopMotors();
  analogWrite(motorRR, 255); 
  analogWrite(motorLR, 255); 
}

void turnLeft() {
  stopMotors();
  analogWrite(motorRF, 255); 
  //analogWrite(motorLR, 255); // Enable me for a tighter turn
}

void turnRight() {
  stopMotors();
  //analogWrite(motorRR, 255); // Enable me for a tighter turn
  analogWrite(motorLF, 255); 
}


