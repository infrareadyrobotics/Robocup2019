//Read Mouse Sensor Function

void readMouse() {
  mouseX = convTwosComp(SPIReadReg(0x03));
  mouseY = convTwosComp(SPIReadReg(0x04));

  mouseXpos += cos(heading[0] / radiansToDegrees) * (mouseX / DPC) + sin(heading[0] / radiansToDegrees) * (mouseY / DPC);
  mouseYpos += -sin(heading[0] / radiansToDegrees) * (mouseX / DPC) + cos(heading[0] / radiansToDegrees) * (mouseY / DPC);

  // Serial.print("Cos: "); Serial.print(cos(heading[0] / radiansToDegrees));
  // Serial.print("\tSin: "); Serial.print(sin(heading[0] / radiansToDegrees));
  // Serial.print("\tX: "); Serial.print(mouseXpos);
  // Serial.print("\tY: "); Serial.print(mouseYpos);

  // Serial.print("\tmouseXRaw: "); Serial.print(mouseX);
  // Serial.print("\tmouseYRaw: "); Serial.print(mouseY);
}
