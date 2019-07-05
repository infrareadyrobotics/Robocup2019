//Calibrate Colour Sensors Function

void calibrateColourSensors(){
  getColourData(); //Get latest Data
  for (int i = 0; i < 24; i++) colourSensorCorrect[i] = colourSensorData[i]; //Get calibration values
}
