#include <Wire.h>
int colourSensorData[24];
int colourSensorCorrect[24];
enum colour {white, green, black, error};
colour colourSensorColour[24];
float points[4][2];
int pointNum;
int colourSensorRadii = 65; //Diameter 130MM
float colourSensorCartisanPos[24][2];

//Thresholds are minimum values for a given colour
int whiteThreshold = 200; //Don't set to low or confusion will occur in corrners/intimitant for to high detections.
int greenThreshold = -15;
int blackThreshold = -40;
int errorLowerThreshold = -100; //Lower Threshold
int errorUpperThreshold = 700; //Upper Threshold

void setup() {
  for (int i = 0; i < 24; i++) { //Calculate
    colourSensorCartisanPos[i][0] = colourSensorRadii * sin((2.0 * PI) * (i / 24.0)); //x
    colourSensorCartisanPos[i][1] = colourSensorRadii * cos((2.0 * PI) * (i / 24.0)); //y
  }

  Serial.begin(115200);
  Wire.begin(); // join i2c bus as master
  calibrateColourSensors();
}

void loop() {
  getColourData();
  getColourSensorColour();
  getPoints();

  //Colour Debug
  //  for (int i = 0; i < 24; i++) {
  //    if (colourSensorColour[i] == white) Serial.print("White");
  //    else if (colourSensorColour[i] == green) Serial.print("Green");
  //    else if (colourSensorColour[i] == black) Serial.print("Black");
  //    else if (colourSensorColour[i] == error) Serial.print("Error");
  //    Serial.print(",");
  //  }

  //Raw Data Debug
  //  Serial.print("\tRaw Data: ");
  //  for (int i = 0; i < 24; i++) {
  //    Serial.print(colourSensorData[i]);
  //    Serial.print(",");
  //  }

  //Points Debug
  //      Serial.print("Points: ");
  //  for (int i = 0; i < pointNum; i++) {
  //    //      Serial.print("\tPoint"); Serial.print(i); Serial.print(": ");
  //    //      Serial.print("X: ");
  //    Serial.print(",");
  //    Serial.print(points[i][0]);
  //    Serial.print(",");
  //    //      Serial.print(" Y: ");
  //    Serial.print(points[i][1]);
  //  }

  Serial.print("\tPointNumber: ");
  switch (pointNum) {
    case 0:
      Serial.print("0");
      break;
    case 1:
      Serial.print("1");
      break;
    case 2:
      Serial.print("2");
      break;
    case 3:
      Serial.print("3");
      break;
  }

  Serial.print('\n');
  //  delay(200);
}
