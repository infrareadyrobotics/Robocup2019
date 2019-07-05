#include <Wire.h>

//Ultrasonics Variables
byte ultrasonicPins[8] = {11, 13, 15, 16, 10, 12, 14, 17};
long ultrasonicTimer[4];
int ultrasonicData[4];
int ultrasonicDataI2C[2];

//Buttons + switches Varibles
byte buttonSwitch[10];
float timeOut;

void setup()
{

  //Buttons
  pinMode(0, INPUT); //button 1
  pinMode(1, INPUT); //button 2
  pinMode(2, INPUT); //button 3
  pinMode(3, INPUT); //button 4

  //Switches
  pinMode(4, INPUT); //switch 1
  pinMode(5, INPUT); //switch 2
  pinMode(6, INPUT); //switch 3
  pinMode(7, INPUT); //switch 4
  pinMode(8, INPUT); //switch 5
  pinMode(9, INPUT); //switch 6

  //Ultrasonics
  pinMode(10, INPUT);  //echo   1
  pinMode(11, OUTPUT); //trig   1
  pinMode(12, INPUT);  //echo   2
  pinMode(13, OUTPUT); //trig   2
  pinMode(14, INPUT);  //echo   3
  pinMode(15, OUTPUT); //trig   3
  pinMode(16, OUTPUT); //trig   4
  pinMode(17, INPUT);  //echo   4

  //Wire setup
  Wire.begin(0x02); //Device slave on address 0x02 (2 in decemial)
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop()
{
  for (byte i = 0; i < 4; i++)
  {
    for (byte i = 0; i < 10; i++)
    { //Read pins and 0 - 9 and assign 0 or 255 for on/off
      buttonSwitch[i] = digitalRead(i) * 255;
    }
    int temp;
    //    timeOut = micros();
    digitalWrite(ultrasonicPins[i], 0);
    delayMicroseconds(10);
    digitalWrite(ultrasonicPins[i], 1);
    delayMicroseconds(10);
    digitalWrite(ultrasonicPins[i], 0);
    temp = pulseIn(ultrasonicPins[i + 4], HIGH, 8000);
    if (temp > 0)
    {
      ultrasonicData[i] = temp / 58.2;
    }
    //    while(true){
    //      if(digitalRead(ultrasonicPins[i + 4]) == 1 || micros() - timeOut > 10000){
    //        break;
    //      }
    //    }
    //    ultrasonicTimer[i] = micros();
    //    while(true){
    //      if(digitalRead(ultrasonicPins[i + 4]) == 0 || micros() - timeOut > 10000){
    //        break;
    //      }
    //    }
    //    ultrasonicData[i] = micros() - ultrasonicTimer[i];
  }
}

void requestEvent()
{ //If data requested from MCU

  for (byte i = 0; i < 10; i++)
  {
    Wire.write(buttonSwitch[i]); //Wire write all 10 switches/buttons to I2C. (pins 0 - 9)
  }
  for (byte i = 0; i < 4; i++)
  {
    if (ultrasonicData[i] >= 255)
    {
      ultrasonicData[i] = 255;
    }
    Wire.write(ultrasonicData[i]);
  }
}

void receiveEvent()
{ //Speeds up reaction time to request events, not sure why.
  Wire.read();
}
