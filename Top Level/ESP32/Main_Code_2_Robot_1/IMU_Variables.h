//IMU Variables

//General
float const radiansToDegrees = 180 / PI;
float heading[3] = {0, 0, 0}; //Combined heading
float const complementaryFactor = 0.3; //Percent Compass
bool firstReading = true;
unsigned long gyroResetTimer = micros();
unsigned long gyroResetTime = 2.5 * 1000000; //5 Seconds in micro seconds

//Compass
float compassHeadingZeroVal[3] = {0, 0, 0}; //Heading to zero compass off.
float compass[3] = {0, 0, 0}; //Raw data
float compassHeading[3] = {0, 0, 0}; //Calibrated heading
float compassHeadingSign[3] = {0, 0, 0};
float compassHeadingRaw[3] = {0, 0, 0}; //Raw heading

//Calibartion Values
float const compassHardCal[3] = {-78.55, 73.65, 181.5}; //X, Y, Z
float const compassSoftCal[3] = {1.002734731, 1.008249313, 1}; //X, Y, Z


//Accelerometer
float accelerometer[3] = {0, 0, 0}; //Acceleration
float accelerometerPos[3] = {0, 0, 0}; //Position
float accelerometerVel[3] = {0, 0, 0}; //Velocity
unsigned long accelTimer = micros(); //Time for intergration

//Calibartion Values
float const accelerometerOffset[3] = {0.433869362,  -0.394362042, 0.576632952}; //0.433869362,  -0.394362042, 0.576632952


//Gyroscope
float gyroScale = 1;
unsigned long gyroTimer;
float gyro[3] = {0, 0, 0}; //Gyro raw value (rad/s)
float gyroHeadingRad[3] = {0, 0, 0}; //Gyro heading rad
float gyroHeadingDeg[3] = {0, 0, 0}; //Gyro heading deg
float gyroHeadingSign[3] = {0, 0, 0};
//Gyro Calibration Values
float gyroDirftCal[3] = {0, 0, 0}; 
