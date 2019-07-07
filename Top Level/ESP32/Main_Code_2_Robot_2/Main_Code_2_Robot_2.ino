#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_FXAS21002C.h>
#include <HardwareSerial.h>
#include "Motor_Variables.h"
#include "IMU_Variables.h"
#include "Mouse_Sensor_Variables.h"
#include "Misc.h"
#include "Colour_Sensor_Variables.h"
#include "EEPROM.h"

//kicker
int kickerTimer;
//PWM Controller
Adafruit_PWMServoDriver pwmController = Adafruit_PWMServoDriver();

//IMU objects
Adafruit_FXOS8700 accelMagObject = Adafruit_FXOS8700(0x8700A, 0x8700B);
Adafruit_FXAS21002C gyroObject = Adafruit_FXAS21002C(0x0021002C);

//Variables

boolean haveBall = false;
int chaseBallAngleBehind = 40;
int haveBallCalcTimer = millis();
int haveBallCalcTimeOut = 300;
unsigned long driveForwardTimer = millis();
unsigned long dribblerTurnOffTime = 300;
boolean dribblerOn = true;
boolean rotateOnSpot = false;
unsigned long driveToSideTimer = millis();
unsigned long driveToSideTime = 5 * 1000;
boolean checkHavedBall = false;

//Field variables
float ySign;
float xSign;
float fieldPositionX = 0;
float fieldPositionY = -50;

byte fieldFrontBack = 1;
byte fieldRightLeft = 0;
byte fieldCorner = 0;

//The playing-field is 122 cm by 183 cm.
float fieldWidth = 122;
float fieldLength = 183;

//Switch states
enum _goalColour {blue, yellow};
_goalColour goalColour = blue;
enum _communications {com, noCom};
_communications communications = com;
enum _tactics {normal, superteam};
_tactics tactics = normal;
enum _robotType {attacker, goalie};
_robotType robotType = attacker;
enum _headingType {gyroMagType, gyroType};
_headingType headingType = gyroMagType;
enum _techChallenge3 {refreeBluetooth, normalBluetooth}; 
_techChallenge3 techChallenge3 = normalBluetooth;
enum _techChallenge6 {normalMode, penaltyKickMode}; 
_techChallenge6 techChallenge6 = normalMode;

boolean motorsOn = true;

//Ultrasonics

//Calibration
float ultrasonicsOffsets[4] = {4,  7,  9,  7};
float angularReliablityDistance = 30; //+- 30 degrees before strange values
float ultrasonicsDistanceVariance = 20; //+- 20 cm noise floor

//Field Sizes //**numbers roughly correspond
float ultrasonicsFieldWidth = 180; //**not actual field width but what ultrasonics get
float ultrasonicsFieldGoalsLength = 185; //**not actual field length but what ultrasonics get
float ultrasonicsFieldLength = 236; //**not actual field length but what ultrasonics get
float ultrasonicsGoalWidth = 60; //**not actual field length but what ultrasonics get until change to second reading
boolean ultrasonicsPositionXValid = false;
boolean ultrasonicsPositionYValid = false;
#define ultrasonicsFilterLength 15
int ultrasonicsFilterArray[4][ultrasonicsFilterLength];

float ultrasonicsPositionX = 0;
float ultrasonicsPositionY = -50;

//Camera
byte cameraFieldSide = 0; //0 == ???, 1 == right, 2 == left
byte const cameraFieldSideThreshold = 3; //Has to be x degrees off axis
float cameraFieldPositionX = 0;
float cameraFieldPositionY = -50;
float cameraFieldPositionValid = true;
float const cameraDistanceRemapVariables[3] = {0.00345310496115, 0.154377477734, -1.65249983625};
#define cameraFilterLength 5
float cameraFieldXFilterData[cameraFilterLength];
float cameraFieldYFilterData[cameraFilterLength];


HardwareSerial bluetoothSerial(1);

Blobs blobs;
ATtinyData attinyData;
Button startButton;
Button calibrateButton;
Button kickerButton;
Options options;
BluetoothData localBluetoothData;
BluetoothData otherBluetoothData;

unsigned long loopTimer;
unsigned long l = 0;

unsigned long profileTimer;

void setup() {
  Serial.begin(250000); //Debug Serial port
  Serial2.begin(115200); //Camera communication serial port
  
  //Check switches
  if (attinyData.switches[4] == 255) techChallenge3 = normalBluetooth; //On techChallenge3 is normalBluetooth
  else techChallenge3 = refreeBluetooth; //Off techChallenge3 is refreeBluetooth

  //Bluetooth
  if (techChallenge3 == normalBluetooth){ //normalBluetooth settings
    bluetoothSerial.begin(38400, SERIAL_8N1, 14, 13);
  }
  else { //refreeBluetooth settings
    bluetoothSerial.begin(9600, SERIAL_8N1, 14, 13);
  }
  pinMode(14, INPUT_PULLUP);
  bluetoothSerial.setTimeout(1);

  pinMode(4,OUTPUT);//kicker pin

  //PWM controller
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

  //IMU
  accelMagObject.begin(ACCEL_RANGE_4G);
  gyroObject.begin(GYRO_RANGE_2000DPS);

  //Mouse sensor
  //Create and start SPI for mouse sensor
  vspi = new SPIClass(VSPI);
  vspi->begin(PIN_SCLK, PIN_MISO, PIN_MOSI, PIN_NCS);
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));

  pinMode(PIN_NCS, OUTPUT);
  delay(10);

  //Initallise mouse sensor
  //Start communication
  digitalWrite(PIN_NCS, HIGH);
  delay(20);
  digitalWrite(PIN_NCS, LOW);

  SPIWriteReg(RESET, 0x5a); //Reset
  delay(1); // wait for it to reboot
  SPIWriteReg(MOUSE_CTRL, 0b00100100);//Setup Mouse Control
  SPIWriteReg(MOTION_CTRL, 0x00);//Clear Motion Control register
  delay(1);

  //Colour Sensor Setup
  for (int i = 0; i < 24; i++) { //Calculate X, Y postions of colour sensors in cartesian coordinates.
    colourSensorCartisanPos[i][0] = colourSensorRadii * sin((2.0 * PI) * (i / 24.0)); //x
    colourSensorCartisanPos[i][1] = colourSensorRadii * cos((2.0 * PI) * (i / 24.0)); //y
  }
  
  Wire.setClock(400000); //Sets I2C frequency to 400khz (fast mode)
  gyroResetTimer = micros();
  loopTimer = micros();

  //EEPROM
  loadEEPROM();
  options.started = EEPROM.readInt(120);

  //Field postion stuff
  readButtonSwitchUltrasonic(attinyData);
  // printButtonSwitchUltrasonic(attinyData);
  if (attinyData.switches[0] == 255) goalColour = blue; //On goal is blue
  else goalColour = yellow; //Off goal is yellow

  if (attinyData.switches[1] == 255) tactics = normal; //On tactics is normal
  else tactics = superteam; //Off tactics is superteam
  if (tactics == superteam){
    fieldPositionX = 0;
    fieldPositionY = -250;
    cameraFieldPositionValid = false;
  }
  else {
    fieldPositionX = 0;
    fieldPositionY = -50;
  }

  if (attinyData.switches[2] == 255) robotType = attacker; //On robotType is attacker
  else robotType = goalie; //Off robotType is goalie

  if (attinyData.switches[3] == 255) headingType = gyroMagType; //On headingType is gyroMagType
  else headingType = gyroType; //Off headingType is gyroType
  
  if (attinyData.switches[4] == 255) techChallenge3 = normalBluetooth; //On techChallenge3 is normalBluetooth
  else techChallenge3 = refreeBluetooth; //Off techChallenge3 is refreeBluetooth

  if (attinyData.switches[5] == 255) techChallenge6 = normalMode; //On techChallenge6 is normal
  else techChallenge6 = penaltyKickMode; //Off techChallenge6 is penaltyKick
}

void loop() {
  l++;
  if (l % 300 == 0) {
    Serial.print("\nHz: ");
    Serial.print(1000000 / ((micros() - loopTimer) / l));
  }
  //Camera
  // profileTimer = micros();
  if (tactics == normal){
    getCameraFieldPosition();
  }
  else {
    blobs = cameraRead();
  }
  // Serial.print("\ngetCameraFieldPosition: "); Serial.print(micros() - profileTimer);

  //Current
  // profileTimer = micros();
  averageCurrent();
  // Serial.print("\nAverageCurrent: "); Serial.print(micros() - profileTimer);

  //Dribbler 
  turnDribbler(100 * dribblerOn);

  //IMU
  // profileTimer = micros();
  readIMU();
  calibrateIMU();
  computeIMUHeading();
  // Serial.print("\nIMU: "); Serial.print(micros() - profileTimer);
  // Serial.print("\nCombined Data");
  if (heading[0] / abs(heading[0]) != -1) Serial.print("Z:  "); else Serial.print("Z:  "); Serial.print(heading[0], 2); Serial.print("\t");
  //  if (heading[1] / abs(heading[1]) != -1) Serial.print("Y:  "); else Serial.print("Y:  "); Serial.print(heading[1], 2); Serial.print("\t");
  //  if (heading[2] / abs(heading[2]) != -1) Serial.print("X:  "); else Serial.print("X:  "); Serial.print(heading[2], 2);

  // Serial.print("Gyroscope: ");
  // if (gyroHeadingDeg[2] / abs(gyroHeadingDeg[2]) != -1) Serial.print("Z:  "); else Serial.print("Z:  "); Serial.print(gyroHeadingDeg[2], 2); Serial.print("\t");

  // Serial.print("Compass: ");
  // if (compassHeading[0] / abs(compassHeading[0]) != -1) Serial.print("Z:  "); else Serial.print("Z:  "); Serial.print(compassHeading[0], 2); Serial.print("\t");
  
  
  //Colour Sensor
  // profileTimer = micros();
  getColourData();
  getColourSensorColour();
  getPoints();
  // Serial.print("\nColour Sensors: "); Serial.print(micros() - profileTimer);


  //ATtinyData and buttons
  // profileTimer = micros();
  readButtonSwitchUltrasonic(attinyData);
  // printButtonSwitchUltrasonic(attinyData);
  if (attinyData.switches[0] == 255) goalColour = blue; //On goal is blue
  else goalColour = yellow; //Off goal is yellow

  if (attinyData.switches[1] == 255) tactics = normal; //On tactics is normal
  else tactics = superteam; //Off tactics is superteam

  if (attinyData.switches[2] == 255) robotType = attacker; //On robotType is attacker
  else robotType = goalie; //Off robotType is goalie

  if (attinyData.switches[3] == 255) headingType = gyroMagType; //On headingType is gyroMagType
  else headingType = gyroType; //Off headingType is gyroType

  if (attinyData.switches[4] == 255) techChallenge3 = normalBluetooth; //On techChallenge3 is normalBluetooth
  else techChallenge3 = refreeBluetooth; //Off techChallenge3 is refreeBluetooth

  if (attinyData.switches[5] == 255) techChallenge6 = normalMode; //On techChallenge6 is normal
  else techChallenge6 = penaltyKickMode; //Off techChallenge6 is penaltyKick

  // Serial.print("Switch, Goal Colour: "); Serial.print(goalColour);
  startButton.buttonData = attinyData.buttons[1];
  calibrateButton.buttonData = attinyData.buttons[0];
  kickerButton.buttonData = attinyData.buttons[2];
  toggle(buttonCode(startButton,40), options.started);
  // Serial.print("Options, Started: "); Serial.print(options.started);

  if (buttonCode(calibrateButton,500)) calibrate();
  if (buttonCode(kickerButton,500)) kick();
  // Serial.print("\nButtons and Ultrasonics: "); Serial.print(micros() - profileTimer);
  
  //Ultrasonics
  // profileTimer = micros();
  getUltrasonicPosition();
  // Serial.print("\ngetUltrasonicPosition: "); Serial.print(micros() - profileTimer);

  //Mouse Sensor
  // profileTimer = micros();
  readMouse();
  // Serial.print("\nreadMouse: "); Serial.print(micros() - profileTimer);

  //Combine Mouse + Ultradata
  // if (ultrasonicsPositionXValid == true) {
  //   fieldPositionX = mouseXpos * 0.7 + ultrasonicsPositionX * 0.3;
  //   mouseXpos = ultrasonicsPositionX;
  // }
  // else fieldPositionX = mouseXpos;
  // if (ultrasonicsPositionYValid == true) {
  //   fieldPositionY = mouseYpos * 0.7 + ultrasonicsPositionY * 0.3;
  //   mouseYpos = ultrasonicsPositionY;
  // }
  // else fieldPositionY = mouseYpos;

  if (cameraFieldPositionValid == true) {
    fieldPositionX = mouseXpos * 0.7 + cameraFieldPositionX * 0.3;
    mouseXpos = cameraFieldPositionX;
    fieldPositionY = mouseYpos * 0.7 + cameraFieldPositionY * 0.3;
    mouseYpos = cameraFieldPositionY;
  }
  else {
    fieldPositionX = mouseXpos;
    fieldPositionY = mouseYpos;
  }

  Serial.print("\tFieldPositionX: "); Serial.print(fieldPositionX);
  Serial.print("\tFieldPositionY: "); Serial.print(fieldPositionY);

  //Get side robot is on.
  ySign = fieldPositionY / fabs(fieldPositionY);
  
  if (cameraFieldSide == 0) {
    xSign = fieldPositionX / fabs(fieldPositionX);
    // Serial.print("\txSign: "); Serial.print(xSign);
  }
  else {
    if(cameraFieldSide == 1) xSign = 1;
    else if(cameraFieldSide == 2) xSign = -1;
    // Serial.print("\txSign: "); Serial.print(xSign);
  }

  //Get which side the robot is in the field and assign to intuitive variables.
  if (ySign > 0) fieldFrontBack = 1; //Front
  else fieldFrontBack = 2; //Back

  if (xSign > 0) fieldRightLeft = 1; //Right
  else fieldRightLeft = 2; //Left

  if (ySign > 0 && xSign > 0) fieldCorner = 1; //Front Right
  else if (ySign < 0 && xSign > 0) fieldCorner = 2; //Back Right
  else if (ySign < 0 && xSign < 0) fieldCorner = 3; //Back Left
  else if (ySign > 0 && xSign < 0) fieldCorner = 4; //Front Left

  //Bluetooth
  // profileTimer = micros();

  //Set bluetooth data

  if (techChallenge3 == normalBluetooth) { //Normal
    localBluetoothData.started = options.started; 
    localBluetoothData.haveBall = haveBall;
    localBluetoothData.role = tactics;
    bluetoothWrite(localBluetoothData, otherBluetoothData); //Write data to bluetooth and recieve data
  }
  else { //Refree
    while (bluetoothSerial.available()){
      byte command = bluetoothSerial.read();
      Serial.print('\n'); Serial.print(command, HEX);
      switch (command) {
        case 115: { //Stop
          options.started = 0;
          break;
        }
        case 83: { //Start
          options.started = 1;
          break;
        }
        case 100:{ //Drive to side of field
          driveToSideTimer = millis();
          break;
        }
      }
    }
  }
  
  //Debug
  // otherBluetoothData.haveBall = true; //Set haveball to true for testing

  // Serial.print("\tOther robot started: "); Serial.print(otherBluetoothData.started);
  // Serial.print("\tOther robot haveball: "); Serial.print(otherBluetoothData.haveBall);
  // Serial.print("\tOther robot tactics: "); Serial.print(otherBluetoothData.role);
  // Serial.print("\nBluetooth: "); Serial.print(micros() - profileTimer);

  //haveBallCalc
  haveBallCalc(); //Runs check for if the robot has the ball

  // profileTimer = micros();
  // if (tactics == normal){
  if (techChallenge6 == normalMode){ //Do normal
    logicController();
  }
  else { //Do penaltyKick
    if (checkHavedBall == false){
      rotateOnSpot = false;
      drive(blobs.array[0][1], 0, 50, 0);
    }
    else { //HavedBall
      float goalAngle;
      float goalDistance;
      float goalSize;
      if (goalColour == blue) { //Blue goal target
        goalAngle = blobs.array[2][1];
        goalDistance = blobs.array[2][2];
        goalSize = blobs.array[2][3];
      }
      else if (goalColour == yellow){ //Yellow goal target
        goalAngle = blobs.array[1][1];
        goalDistance = blobs.array[1][2];
        goalSize = blobs.array[1][3];
      }
      
      if (goalAngle < heading[0] + 5 && goalAngle > heading[0] - 5) { //Lined up with goals
        rotateOnSpot = false;
        Serial.print("Alert2");
        drive(0, goalAngle,0,0);
        dribblerOn = false ;
        if (millis() - driveForwardTimer > dribblerTurnOffTime){
          kick();
        }
      }
      else { //Not lined up
        Serial.print("Alert");
        rotateOnSpot = true;
        drive(0,goalAngle,0,0);
        driveForwardTimer = millis();
        dribblerOn = true;
      }
    }
  }
  // }
  // else {
  //   chaseBallLogic();
  // }
  // Serial.print("\nGameLogic: "); Serial.print(micros() - profileTimer);
  
  cameraPrint(blobs);
  // profileTimer = micros();
  cameraWrite();
  // Serial.print("\nCameraWrite: "); Serial.print(micros() - profileTimer);

  Serial.println();
}

