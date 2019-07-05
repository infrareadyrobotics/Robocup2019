//Motor Variables
/*
  Dribbler PWM to 8

  Motor 1 A to 0
  Motor 1 B to 4

  Motor 2 A to 1
  Motor 2 B to 5

  Motor 3 A to 2
  Motor 3 B to 6

  Motor 4 A to 3
  Motor 4 B to 7

  CS1 to GPIO 36
  CS2 to GPIO 39
  CS3 to GPIO 34
  CS4 to GPIO 32
  CS5 to GPIO 35
*/

float mDegree[4] = {50, 130, 230, 310};
float wDegree[4];
float current[5];

//Note Motor 2's Channels are swapped.
int inAPwm[4] = {0, 5, 2, 3}; //Assign motor driver input A to a PWM channel
int inBPwm[4] = {4, 1, 6, 7}; //Assign motor driver input B to a PWM channel
int dribblerPwmChannel = 8;
int freq = 1000; //Frequency of PWM in hurts
byte currentPin[5] = {36, 39, 34, 27, 35}; //Pins current sense comes in on.
unsigned long cTimer1[5];
unsigned long cTimer2[5];
float avgCurrent[32][5];
int pwmLimit[5];

unsigned long timer[40];
boolean pwmState[40];

boolean dribblerCurrentLimited = false;
