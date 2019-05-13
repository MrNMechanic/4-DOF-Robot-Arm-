#include <AccelStepper.h>
#include <Bounce2.h>

 

const int ledEnable = 13; 
const int pinSwEnable = 7;  
const int pinEnable = 8;  
 
unsigned long debounceDelay = 10; 
 
const int jX = A0;  
const int stepX = 3;  
const int dirX = 4; 
long speedX, valX, mapX;  
 
const int jY = A1;  
const int stepY = 5;  
const int dirY = 6; 
long speedY, valY, mapY;  
 
const int maxSpeed = 1000; 
const int minSpeed = 0; 
const float acceleration = 50.0; 
 
const int treshold = 30;  
long tresholdUp, tresholdDown;  
 
boolean moveX, moveY, enable;  
 
Bounce btnEnable = Bounce();  
 
AccelStepper motorX(AccelStepper::DRIVER, stepX, dirX);
AccelStepper motorY(AccelStepper::DRIVER, stepY, dirY);
 
void setup() {
  speedX = speedY = 0;
  enable = false;
 
  pinMode(ledEnable, OUTPUT);
  pinMode(pinEnable, OUTPUT);
 
  pinMode(pinSwEnable, INPUT_PULLUP); 
 
  digitalWrite(ledEnable, enable);
  digitalWrite(pinEnable, !enable); 
 
  btnEnable.attach(pinSwEnable);
  btnEnable.interval(debounceDelay);
 
  tresholdDown = (maxSpeed / 2) - treshold;
  tresholdUp = (maxSpeed / 2) + treshold;
 
  motorX.setMaxSpeed(maxSpeed);
  motorX.setSpeed(minSpeed);
  motorX.setAcceleration(acceleration);
 
  motorY.setMaxSpeed(maxSpeed);
  motorY.setSpeed(minSpeed);
  motorY.setAcceleration(acceleration);
}
 
void loop() {
 
  checkEnable();
 
  digitalWrite(ledEnable, enable);  
  digitalWrite(pinEnable, !enable); 
 
  valX = analogRead(jX);
  valY = analogRead(jY);
 
  mapX = map(valX, 0, 1023, minSpeed, maxSpeed);
  mapY = map(valY, 0, 1023, minSpeed, maxSpeed);
 
  pilotMotor(mapX, mapY);
 
}
 
void pilotMotor(long mapX, long mapY) {
 
  if (mapX <= tresholdDown) {
    speedX = -map(mapX, tresholdDown, minSpeed,   minSpeed, maxSpeed);
    moveX = true;
  } else if (mapX >= tresholdUp) {
    speedX = map(mapX,  maxSpeed, tresholdUp,  maxSpeed, minSpeed);
    moveX = true;
  } else {
    speedX = 0;
    moveX = false;
  }
 
  if (mapY <= tresholdDown) {
    speedY = -map(mapY, tresholdDown, minSpeed,   minSpeed, maxSpeed);
    moveY = true;
  } else if (mapY >= tresholdUp) {
    speedY = map(mapY,  maxSpeed, tresholdUp,  maxSpeed, minSpeed);
    moveY = true;
  } else {
    speedY = 0;
    moveY = false;
  }
 
  if (moveX) {
    motorX.setSpeed(speedX);
    motorX.run();
  } else {
    motorX.stop();
  }
 
  if (moveY) {
    motorY.setSpeed(speedY);
    motorY.run();
  } else {
    motorY.stop();
  }
}
 
 
void checkEnable() {
 
  btnEnable.update();
 
  if (btnEnable.fell()) {
    enable = !enable;
  }
 
}
