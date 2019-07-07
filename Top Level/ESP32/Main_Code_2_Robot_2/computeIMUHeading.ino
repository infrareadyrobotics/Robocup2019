// Compute IMU Heading Function

void computeIMUHeading () {
  //Calculate compass headings
  compassHeadingRaw[0] = atan2(compass[0], compass[1]) * radiansToDegrees; //XY
  compassHeadingRaw[1] = atan2(compass[0], compass[2]) * radiansToDegrees; //XZ
  compassHeadingRaw[2] = atan2(compass[1], compass[2]) * radiansToDegrees; //YZ

  //Apply compass zeroing
  compassHeading[0] = compassHeadingRaw[0] - compassHeadingZeroVal[0];
  compassHeading[1] = compassHeadingRaw[1] - compassHeadingZeroVal[1];
  compassHeading[2] = compassHeadingRaw[2] - compassHeadingZeroVal[2];
  for (int i = 0; i < 3; i++) { //Make sure in range between -180 and 180
    if (compassHeading[i] < -180) compassHeading[i] += 360;
    else if (compassHeading[i] > 180) compassHeading[i] -= 360;
  }

  //get sign of compass heading
  compassHeadingSign[0] = compassHeading[0] / fabs(compassHeading[0]);
  compassHeadingSign[1] = compassHeading[1] / fabs(compassHeading[1]);
  compassHeadingSign[2] = compassHeading[2] / fabs(compassHeading[2]);

  //Get gyro headings and intergrate them with current time between loops
  gyroHeadingRad[0] += gyro[0] * ((micros() - gyroTimer) / 1000000.0) * gyroScale;
  gyroHeadingRad[1] += gyro[1] * ((micros() - gyroTimer) / 1000000.0) * gyroScale;
  gyroHeadingRad[2] += gyro[2] * ((micros() - gyroTimer) / 1000000.0) * gyroScale;
  gyroTimer = micros();

  if (micros() - gyroResetTimer > gyroResetTime && headingType == gyroMagType) { //Zero gyro to mag if timer finished and switch set to gyroMag
    gyroResetTimer = micros();
    gyroHeadingRad[0] = compassHeading[2] / radiansToDegrees;
    gyroHeadingRad[1] = compassHeading[1] / radiansToDegrees;
    gyroHeadingRad[2] = compassHeading[0] / radiansToDegrees;
  }

  //Convert gyro headings to degrees
  gyroHeadingDeg[0] = gyroHeadingRad[0] * radiansToDegrees;
  gyroHeadingDeg[1] = gyroHeadingRad[1] * radiansToDegrees;
  gyroHeadingDeg[2] = gyroHeadingRad[2] * radiansToDegrees;

  for (int i = 0; i < 3; i++) { //Make sure in range between -180 and 180
    if (gyroHeadingDeg[i] < -180) gyroHeadingDeg[i] += 360;
    else if (gyroHeadingDeg[i] > 180) gyroHeadingDeg[i] -= 360;
  }

  //Get sign of gyro heading
  gyroHeadingSign[0] = gyroHeadingDeg[0] / fabs(gyroHeadingDeg[0]);
  gyroHeadingSign[1] = gyroHeadingDeg[1] / fabs(gyroHeadingDeg[1]);
  gyroHeadingSign[2] = gyroHeadingDeg[2] / fabs(gyroHeadingDeg[2]);

  if (gyroHeadingSign[2] != compassHeadingSign[0] && fabs(compassHeading[0]) > 90  && headingType == gyroMagType) {
    gyroHeadingDeg[2] *= -1;
  }
  if (gyroHeadingSign[1] != compassHeadingSign[1] && fabs(compassHeading[1]) > 90  && headingType == gyroMagType) {
    gyroHeadingDeg[1] *= -1;
  }
  if (gyroHeadingSign[0] != compassHeadingSign[2] && fabs(compassHeading[2]) > 90  && headingType == gyroMagType) {
    gyroHeadingDeg[0] *= -1;
  }

  if (headingType == gyroMagType){ //If switch set to using gyro and magnetometer
    //Apply Simple complementry Filter
    heading[0] = -((1 - complementaryFactor) * gyroHeadingDeg[2] + (complementaryFactor) * (compassHeading[0])); //XY plane Z axis //gyroHeadingDeg[2];
    heading[1] = gyroHeadingDeg[1]; //(1 - complementaryFactor) * gyroHeadingDeg[1] + (complementaryFactor) * (compassHeading[1]); //XZ plane Y axis //Compass is funky on Y and X axis due to uncomplete calibration
    heading[2] = gyroHeadingDeg[0]; //(1 - complementaryFactor) * gyroHeadingDeg[0] + (complementaryFactor) * (compassHeading[2]); //YZ plane X axis //Compass is funky on Y and X axis due to uncomplete calibration
  }
  else { //If switch set to using gyro
    //Use gyro  
    heading[0] = -gyroHeadingDeg[2]; //XY plane Z axis
    heading[1] = gyroHeadingDeg[1]; //XZ plane Y axis 
    heading[2] = gyroHeadingDeg[0]; //YZ plane X axis
  }
}
