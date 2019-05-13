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
 
boolean abilitato, muoviX, muoviY, enable;  
 
Bounce btnEnable = Bounce();  
 
AccelStepper motoreX(AccelStepper::DRIVER, stepX, dirX);
AccelStepper motoreY(AccelStepper::DRIVER, stepY, dirY);
 
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
 
  //configura parametri dei motori
  motoreX.setMaxSpeed(maxSpeed);
  motoreX.setSpeed(minSpeed);
  motoreX.setAcceleration(acceleration);
 
  motoreY.setMaxSpeed(maxSpeed);
  motoreY.setSpeed(minSpeed);
  motoreY.setAcceleration(acceleration);
}
 
void loop() {
 
  checkEnable();
 
  digitalWrite(ledEnable, enable);  
  digitalWrite(pinEnable, !enable); 
 
  valX = analogRead(jX);
  valY = analogRead(jY);
 
  mapX = map(valX, 0, 1023, minSpeed, maxSpeed);
  mapY = map(valY, 0, 1023, minSpeed, maxSpeed);
 
  pilotaMotori(mapX, mapY);
 
}
 
void pilotaMotori(long mapX, long mapY) {
 
  if (mapX <= tresholdDown) {
    //x va indietro
    speedX = -map(mapX, tresholdDown, minSpeed,   minSpeed, maxSpeed);
    muoviX = true;
  } else if (mapX >= tresholdUp) {
    //x va avanti
    speedX = map(mapX,  maxSpeed, tresholdUp,  maxSpeed, minSpeed);
    muoviX = true;
  } else {
    //x sta fermo
    speedX = 0;
    muoviX = false;
  }
 
  if (mapY <= tresholdDown) {
    //y va giù
    speedY = -map(mapY, tresholdDown, minSpeed,   minSpeed, maxSpeed);
    muoviY = true;
  } else if (mapY >= tresholdUp) {
    //y va su
    speedY = map(mapY,  maxSpeed, tresholdUp,  maxSpeed, minSpeed);
    muoviY = true;
  } else {
    //y sta fermo
    speedY = 0;
    muoviY = false;
  }
 
  if (muoviX) {
    motoreX.setSpeed(speedX);
    motoreX.run();
  } else {
    motoreX.stop();
  }
 
  if (muoviY) {
    motoreY.setSpeed(speedY);
    motoreY.run();
  } else {
    motoreY.stop();
  }
}
 
 
void checkEnable() {
 
  btnEnable.update();
 
  if (btnEnable.fell()) {
    enable = !enable;
  }
 
}
