//Get Colour of Colour Sensor Function

void getColourSensorColour() {
  for (int i = 0; i < 24; i++) { //Loop throught all sensors and check there colour and assign a colour for given threshold.
    if (colourSensorData[i] > errorUpperThreshold) colourSensorColour[i] = error;
    else if (colourSensorData[i] > whiteThreshold) colourSensorColour[i] = white;
    else if (colourSensorData[i] > greenThreshold) colourSensorColour[i] = green;
    else if (colourSensorData[i] > blackThreshold) colourSensorColour[i] = black;
    else if (colourSensorData[i] > errorLowerThreshold) colourSensorColour[i] = error;
  }
}
