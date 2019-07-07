void readButtonSwitchUltrasonic(ATtinyData& attinyData) {
  Wire.requestFrom(0x02, 14);
  for (int i = 0; i < 4; i++) {
    attinyData.buttons[i] = Wire.read();
  }
  for (int i = 0; i < 6; i++) {
    attinyData.switches[i] = Wire.read();
  }
  for (int i = 0; i < 4; i++) { // 3 is front, 2 is right 1 is left and 0 is back
    //Remapping ultrasonics so they are in clockwise order starting from front and calibration so distance from centre of robot is taken into acount.
    if (i == 0) attinyData.ultrasonics[2] = Wire.read() + ultrasonicsOffsets[2];
    else if (i == 1) attinyData.ultrasonics[3] = Wire.read() + ultrasonicsOffsets[3];
    else if (i == 2) attinyData.ultrasonics[1] = Wire.read() + ultrasonicsOffsets[1];
    else if (i == 3) attinyData.ultrasonics[0] = Wire.read() + ultrasonicsOffsets[0];
  }

  for(int j = 0; j < 4; j++){ //Shift data for each ultrasonic through array
    for (int k = 0; k < ultrasonicsFilterLength - 1; k++){
      ultrasonicsFilterArray[j][k] = ultrasonicsFilterArray[j][k + 1];
      // Serial.print(", "); Serial.print(ultrasonicsFilterArray[j][k]);
    }
    ultrasonicsFilterArray[j][ultrasonicsFilterLength - 1] = attinyData.ultrasonics[j];
    // Serial.print(", "); Serial.print(ultrasonicsFilterArray[j][ultrasonicsFilterLength - 1]);
  }
}
void printButtonSwitchUltrasonic(ATtinyData attinyData){
  for (int i = 0; i < 4; i++) {
    Serial.print(attinyData.buttons[i]);
    Serial.print("\t");
  }
  for (int i = 0; i < 6; i++) {
    Serial.print(attinyData.switches[i]);
    Serial.print("\t");
  }
  for (int i = 0; i < 4; i++) {
    Serial.print(attinyData.ultrasonics[i]);
    Serial.print("\t");
  }
}
