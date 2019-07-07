//Get Colour Data from I2C Dunction

void getColourData() {
  //board side is colour sensor data from robot side.

  Wire.beginTransmission(0x01); //start board side communication
  Wire.write(1); //set board side for side 1
  Wire.endTransmission(); //end board side communication

  Wire.requestFrom(0x01, 24); //request first data packet

  while (Wire.available()) { //Loop for all data in buffer
    int a = Wire.read(); //read byte one
    int b = Wire.read(); //read byte two
    int dataCombined = a;
    dataCombined = (dataCombined << 8) | b; //recombine data
    colourSensorData[11 - Wire.available() / 2] = dataCombined; //set data to colour sensor array.
  }

  //Set board side to 0
  Wire.beginTransmission(0x01);
  Wire.write(0);
  Wire.endTransmission();

  //Request second data packet.
  Wire.requestFrom(0x01, 24);

  while (Wire.available()) { //Loop for all data in buffer

    //Read segments of int
    int a = Wire.read();
    int b = Wire.read();

    //Recombine Data
    int dataCombined = a;
    dataCombined = (dataCombined << 8) | b;
    colourSensorData[23 - Wire.available() / 2] = dataCombined; //Set data to colour sensor array.
  }
}
