//Calibrate IMU Function

void calibrateIMU() {
  for (int i = 0; i < 3; i++) {
    //Calibrate Compass
    compass[i] -= compassHardCal[i]; //Apply hard iron calibration
    compass[i] *= compassSoftCal[i]; //Apply soft iron calibration

    //Calibrate Gyro
    gyro[i] -= gyroDirftCal[i]; //Apply offset for gyro drift.

    //Calibrate accelerometer
    accelerometer[i] -= accelerometerOffset[i]; //Apply offset to make readings acturate.
  }
}
