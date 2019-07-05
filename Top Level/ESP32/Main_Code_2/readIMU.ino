//Read IMU Function

void readIMU() {
  //Create objects for all sensors
  sensors_event_t aevent, mevent, gevent;

  //Get data
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
}
