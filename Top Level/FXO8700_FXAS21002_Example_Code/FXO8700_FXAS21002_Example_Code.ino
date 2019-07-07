#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_FXAS21002C.h>

Adafruit_FXOS8700 accelMagObject = Adafruit_FXOS8700(0x8700A, 0x8700B);
Adafruit_FXAS21002C gyroObject = Adafruit_FXAS21002C(0x0021002C);

//General
float radiansToDegrees = 180 / PI;
float heading[3] = {0, 0, 0};
float complementaryFactor = 0.3; //Percent Compass
bool firstReading = true;
unsigned long gyroResetTimer = micros();
unsigned long gyroResetTime = 5 * 1000000; //5 Seconds in micro seconds


//Compass
float compassHeadingZeroVal[3] = {0, 0, 0};
float compass[3] = {0, 0, 0};
float compassHeading[3] = {0, 0, 0};
float compassHeadingSign[3] = {0, 0, 0};
float compassHeadingRaw[3] = {0, 0, 0};
//Compass Calibartion Values
float compassHardCal[3] = { -116.25, 90.9, 181.5}; //X, Y, Z
float compassSoftCal[3] = {1.022494888, 0.943396226, 1}; //X, Y, Z

//Accelerometer
float accelerometer[3] = {0, 0, 0};
float accelerometerPos[3] = {0, 0, 0};
float accelerometerVel[3] = {0, 0, 0};
unsigned long accelTimer = micros();

//Calibartion Values
float accelerometerOffset[3] = {0.433869362,  -0.394362042, 0.576632952}; //0.433869362,  -0.394362042, 0.576632952


float gravity = 9.807;

//Gyroscope
float gyroScale = 1;
unsigned long gyroTimer;
float gyro[3] = {0, 0, 0};
float gyroHeadingRad[3] = {0, 0, 0};
float gyroHeadingDeg[3] = {0, 0, 0};
float gyroHeadingSign[3] = {0, 0, 0};
//Gyro Calibration Values
float gyroDirftCal[3] = {0.001703743, -0.010605376,  -0.008906191}; //0.001703743, -0.010605376,  -0.008906191


void setup() {
  Serial.begin(9600);
  accelMagObject.begin(ACCEL_RANGE_4G);
  gyroObject.begin(GYRO_RANGE_2000DPS);
  //  sensor_t accel, mag;
  //  accelmag.getSensor(&accel, &mag);
  gyroResetTimer = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t aevent, mevent, gevent;

  /* Get a new sensor event */
  accelMagObject.getEvent(&aevent, &mevent);
  gyroObject.getEvent(&gevent);

  //Get compass data
  compass[0] = mevent.magnetic.x;
  compass[1] = mevent.magnetic.y;
  compass[2] = mevent.magnetic.z;

  //Get gyro data
  gyro[0] = gevent.gyro.x;
  gyro[1] = gevent.gyro.y;
  gyro[2] = gevent.gyro.z;

  //Get accelerometer data
  accelerometer[0] = aevent.acceleration.x;
  accelerometer[1] = aevent.acceleration.y;
  accelerometer[2] = aevent.acceleration.z;

  //Calibrate
  for (int i = 0; i < 3; i++) {
    //Calibrate Compass
    compass[i] -= compassHardCal[i];
    compass[i] *= compassSoftCal[i];
    //Calibrate Gyro
    gyro[i] -= gyroDirftCal[i];

    //Calibrate accelerometer
    accelerometer[i] -= accelerometerOffset[i];
  }

  //Get compass headings
  compassHeadingRaw[0] = atan2(compass[0], compass[1]) * radiansToDegrees; //XY
  compassHeadingRaw[1] = atan2(compass[0], compass[2]) * radiansToDegrees; //XZ
  compassHeadingRaw[2] = atan2(compass[1], compass[2]) * radiansToDegrees; //YZ

  if (firstReading) { //If first loop get zeroed values for all axis
    compassHeadingZeroVal[0] = compassHeadingRaw[0];
    compassHeadingZeroVal[1] = compassHeadingRaw[1];
    compassHeadingZeroVal[2] = compassHeadingRaw[2];
    firstReading = false;
  }

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

  if (micros() - gyroResetTimer > gyroResetTime) {
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

  if (gyroHeadingSign[2] != compassHeadingSign[0] && fabs(compassHeading[0]) > 90) {
    gyroHeadingDeg[2] *= -1;
  }
  if (gyroHeadingSign[1] != compassHeadingSign[1] && fabs(compassHeading[1]) > 90) {
    gyroHeadingDeg[1] *= -1;
  }
  if (gyroHeadingSign[0] != compassHeadingSign[2] && fabs(compassHeading[2]) > 90) {
    gyroHeadingDeg[0] *= -1;
  }
  //Acceleromter Postion USELESS drifts 0.1 m/s
  //  accelerometerVel[0] += accelerometer[0] * ((micros() - accelTimer) / 1000000.0);
  //  accelerometerVel[1] += accelerometer[1] * ((micros() - accelTimer) / 1000000.0);
  //  accelerometerVel[2] += (accelerometer[2] - 9.81) * ((micros() - accelTimer) / 1000000.0);
  //  //Velocity
  //  accelerometerPos[0] += accelerometerVel[0] * ((micros() - accelTimer) / 1000000.0);
  //  accelerometerPos[1] += accelerometerVel[1] * ((micros() - accelTimer) / 1000000.0);
  //  accelerometerPos[2] += (accelerometerVel[2]) * ((micros() - accelTimer) / 1000000.0);
  //  accelTimer = micros();




  //Apply Simple complementry Filter
  heading[0] = -((1 - complementaryFactor) * gyroHeadingDeg[2] + (complementaryFactor) * (compassHeading[0])); //XY plane Z axis
  heading[1] = gyroHeadingDeg[1];//(1 - complementaryFactor) * gyroHeadingDeg[1] + (complementaryFactor) * (compassHeading[1]); //XZ plane Y axis
  heading[2] = gyroHeadingDeg[0];//(1 - complementaryFactor) * gyroHeadingDeg[0] + (complementaryFactor) * (compassHeading[2]); //YZ plane X axis

  //  Serial.print("Gyroscope: ");
  //  if (gyroHeadingDeg[2] / abs(gyroHeadingDeg[2]) != -1) Serial.print("Z:  "); else Serial.print("Z:  "); Serial.print(gyroHeadingDeg[2], 2); Serial.print("\t");
  //  if (gyroHeadingDeg[1] / abs(gyroHeadingDeg[1]) != -1) Serial.print("Y:  "); else Serial.print("Y:  "); Serial.print(gyroHeadingDeg[1], 2); Serial.print("\t");
  //  if (gyroHeadingDeg[0] / abs(gyroHeadingDeg[0]) != -1) Serial.print("X:  "); else Serial.print("X:  "); Serial.print(gyroHeadingDeg[0], 2);
  //
  //  Serial.print("\t");
  //  Serial.print("Compass: ");
  //  if (compassHeading[0] / abs(compassHeading[0]) != -1) Serial.print("Z:  "); else Serial.print("Z:  "); Serial.print(compassHeading[0], 2); Serial.print("\t");
  //  if (compassHeading[1] / abs(compassHeading[1]) != -1) Serial.print("Y:  "); else Serial.print("Y:  "); Serial.print(compassHeading[1], 2); Serial.print("\t");
  //  if (compassHeading[2] / abs(compassHeading[2]) != -1) Serial.print("X:  "); else Serial.print("X:  "); Serial.print(compassHeading[2], 2);
  //
  //  Serial.print("\t");
  //  Serial.print("Combined Data");
  //  if (heading[0] / abs(heading[0]) != -1) Serial.print("Z:  "); else Serial.print("Z:  "); Serial.print(heading[0], 2); Serial.print("\t");
  //  if (heading[1] / abs(heading[1]) != -1) Serial.print("Y:  "); else Serial.print("Y:  "); Serial.print(heading[1], 2); Serial.print("\t");
  //  if (heading[2] / abs(heading[2]) != -1) Serial.print("X:  "); else Serial.print("X:  "); Serial.print(heading[2], 2);

  //  /* Display the accel results (acceleration is measured in m/s^2) */
  //  Serial.print("Accelerometer ");
  //  Serial.print("X: "); Serial.print(accelerometerPos[0], 4); Serial.print("  ");
  //  Serial.print("Y: "); Serial.print(accelerometerPos[1], 4); Serial.print("  ");
  //  Serial.print("Z: "); Serial.print(accelerometerPos[2], 4); Serial.print("  ");
  //  Serial.println("m/s^2");
  //
  //  /* Display the mag results (mag data is in uTesla) */
  //    Serial.print("Magnetometer ");
  //    Serial.print("X: "); Serial.print(compass[0], 1); Serial.print("\t");
  //    Serial.print("Y: "); Serial.print(compass[1], 1); Serial.print("\t");
  //    Serial.print("Z: "); Serial.print(compass[2], 1);
  //
  //  /* Display the results (speed is measured in rad/s) */
  //  Serial.print("Gyroscope ");
  //  Serial.print("X: "); Serial.print(gevent.gyro.x, 4); Serial.print("  ");
  //  Serial.print("Y: "); Serial.print(gevent.gyro.y, 4); Serial.print("  ");
  //  Serial.print("Z: "); Serial.print(gevent.gyro.z, 4); Serial.print("  ");
  //  Serial.println("rad/s ");

  //  Serial.print(gevent.gyro.x, 12); Serial.print("\t");
  //  Serial.print(gevent.gyro.y, 12); Serial.print("\t");
  //  Serial.print(gevent.gyro.z, 12);

  Serial.print(mevent.magnetic.x, 4); Serial.print("\t");
  Serial.print(mevent.magnetic.y, 4); Serial.print("\t");
  Serial.print(mevent.magnetic.z, 4);

  //  Serial.print(aevent.acceleration.x, 8); Serial.print("\t");
  //  Serial.print(aevent.acceleration.y, 8); Serial.print("\t");
  //  Serial.print(aevent.acceleration.z, 8);

  //  Serial.print(compass[0], 8); Serial.print("\t");
  //  Serial.print(compass[1], 8); Serial.print("\t");
  //  Serial.print(compass[2], 8);


  Serial.println();
  //  delay(1000);
}
