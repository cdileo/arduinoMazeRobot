/*
 *Group of 4 - Maze traversal code
 * Chris Dileo
 * Brendon Hodaly
 * Pankhil Kainth
 * Natalie Ngan
 * Steven Kim
*/
#include <NewPing.h>
#include <Servo.h>
/*Define Motor Pins. R/L indicates left/right motor and F/R indicates whether 
                  the robot moves forward or reverse when powered on pin.*/
//Control pins for forward and reverse motor movement
#define motorRF 10 //A-iB
#define motorRR 9 //A-iA
#define motorLF 8 
#define motorLR 7 

// Front touch sensor
#define touchSensor 13

// Left ultrasonic sensor pins
#define TRIGGER_PIN_L 5
#define ECHO_PIN_L 6

// Right ultrasonic pins
#define TRIGGER_PIN_R 3
#define ECHO_PIN_R 4
#define MAX_DISTANCE 200

// Distance threshold - must have a reading larger than this to try turning
#define THRESHOLD 20

// Time for a ~45 degree turn
#define TURN_TIME 450 //in ms

// Time to think between changing actions
#define THINKING_TIME 1000 //in ms 

// Time to force backward movement when encountering an obstacle
#define BACK_TIME 300 //ms

// We were playing with feeding different motor speeds.
#define MOTOR_SPEED 255 // 0-255

#define LIGHT_SENSOR_PIN A1 // Light sensor pin
#define LIGHT_THRESHOLD 1.2 // What we consider to be looking at black 

// Using the servo
#define SERVO_PIN 12
Servo servo;
int servoPosition = 80;

NewPing sonarL(TRIGGER_PIN_L, ECHO_PIN_L, MAX_DISTANCE);
NewPing sonarR(TRIGGER_PIN_R, ECHO_PIN_R, MAX_DISTANCE);

void setup() {
  // Set pins to write / read
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
  delay(1000);
}

void loop(){
  // Our base state is to move forward until we hit something.
  goForward();
  int foundBlack = testForBlack();
  while(!foundBlack) {
    delay(200);
    stopAndThink();
    foundBlack = testForBlack();
    goForward();
  }
  
  stopAndThink();
  if (lightReading() < LIGHT_THRESHOLD) {
    stopAndThink();
    turnRight(255);
    delay(100);
    stopAndThink();
    search();
  }
  
}

void search() {
  // Do a max of 5 sweeps before resetting
  int i;
  for (i = 0; i < 5; i++) {
    goForward();
    delay(100);
    stopAndThink();
    sweep();  
  }
  reset();
}

void sweep() {
  // need to incorporate light sensor readings here too
  float lr = lightReading();
  if (testForBlack()) turnAround();
  turnLeft(255);
  delay(1000);
  stopAndThink();
  lr = lightReading();
  if (testForBlack()) turnAround();
  turnRight(255);
  delay(1000);
  stopAndThink();
  lr = lightReading();
  if (testForBlack()) turnAround();
}

int testForBlack() {
  float lr = lightReading();
  Serial.println(lr);
  if (lr < LIGHT_THRESHOLD){
    return 1;
  } else {
    return 0;
  }
}

void turnAround(){
  // Every now and then I fall apart
  turnRight(255);
  delay(2000);
  stopAndThink();
}

void reset(){
  goBackward();
  delay(300);
  turnLeft(255);
  stopMotors();
}

int checkTouch() {
  return (int)digitalRead(touchSensor);
}

void backUpAndLookForHole() {
  //We have bumped into something...
  int distLeft;
  int distRight;
  // Stop and reflect on life
  stopAndThink();

  // Backup a set amount of time
  goBackward();
  delay(BACK_TIME);
  
  while (true) {
    // Take our sensor readings to right and left
    distLeft = readLeftSensor();
    distRight = readRightSensor();

    // Turn towards the side that is more 'open' (farther away) 
    // if the distance is greater than our threshold
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
    }
  }  
  stopAndThink();
}

void stopAndThink() {
  stopMotors();
  delay(THINKING_TIME);
}

// The below sensor code is roughly equivalent to calling
// the sonar.pingcm() function
// In the end, it just gets a reading from the ultrasonic.
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
  analogWrite(motorLR, MOTOR_SPEED); // Enable me for a tighter turn
}

void turnRight(int speed) {
  stopMotors();
  analogWrite(motorRR, MOTOR_SPEED); // Enable me for a tighter turn
  analogWrite(motorLF, speed); 
}

void closeClaw () {
  for (servoPosition = 80; servoPosition <= 160; servoPosition += 1) { 
    servo.write(servoPosition);      // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void openClaw () {
    for (servoPosition = 160; servoPosition <= 80; servoPosition -= 1) { 
    servo.write(servoPosition);      // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

float lightReading () {
  int i = 0;
  int numSamples = 500; //Average out the sensor's readings to get something reliable.
  int sumReadings = 0;
  
  for (i = 0; i < numSamples; i++){
    sumReadings += analogRead(LIGHT_SENSOR_PIN);
  }
  return (sumReadings * 1.0) / (numSamples * 1.0);
}

