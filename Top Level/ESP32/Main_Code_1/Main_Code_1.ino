#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Motor_Variables.h"

Adafruit_PWMServoDriver pwmController = Adafruit_PWMServoDriver();

//Variables
float headingCal;//, headingDeg, headingRad;

//Test Setting to zere
//headingCal = 0;

boolean haveBall = false;

//Switch states
enum _goalColour {blue, yellow};
_goalColour goalColour = blue;
enum _communications {com, noCom};
_communications communications = com;
enum _tactics {defult, tactic2}; //REPLACE WITH PROPER NAMES
_tactics tactics = defult;
enum _robotType {attack, defend};
_robotType robotType;
boolean motorsOn = true;

void setup() {
  Serial.begin(115200);
  pwmController.begin();
  pwmController.setPWMFreq(freq);  //Set the pwmController's Frequency in Hz

  //Makes omni drive work
  for (int i = 0; i < 4; i++) wDegree[i] = mDegree[i] + 90;

  //Set current sensing pins to input.
  pinMode(currentPin[0], INPUT);
  pinMode(currentPin[1], INPUT);
  pinMode(currentPin[2], INPUT);
  pinMode(currentPin[3], INPUT);
  pinMode(currentPin[4], INPUT);

}

void loop() {
  averageCurrent();
  //  turnDribbler(100);
  //drive(0, 0, 80, 0);

  Serial.println();
}
