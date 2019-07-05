#include <Wire.h>


int colorSensor[48];
uint8_t colorSensorI2C[2];
byte boardSegment;



void setup() {
  Wire.begin(0x01);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
}

void requestEvent() {
  if (boardSegment) {
    for (int i = 0; i <= 11; i ++) {
      colorSensor[i] = analogRead("A" + i);
      colorSensorI2C[0] = (colorSensor[i] >> 8) & 0xFF;
      colorSensorI2C[1] = colorSensor[i] & 0xFF;
      Wire.write(colorSensorI2C, 2);
    }
  }else{
    for (int i = 12; i <= 23; i ++) {
      colorSensor[i] = analogRead("A" + i);
      colorSensorI2C[0] = (colorSensor[i] >> 8) & 0xFF;
      colorSensorI2C[1] = colorSensor[i] & 0xFF;
      Wire.write(colorSensorI2C, 2);
    }
  }
}

void receiveEvent() {
  boardSegment = Wire.read();
}
