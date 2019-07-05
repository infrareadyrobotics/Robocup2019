//Drive Function

void drive(float Degrees, float targetHeading, int Speed, int rotateAmount) {
  int mSpeed[4];
  float rotate;
  float maxRotateSpeed = 99;
  boolean rotateOnSpot = false;
  char buf[100];

  //Calculate Correct Heading Target
  float useHeading = headingCal + targetHeading; //Caluclate Heading amount reltaive to zeroed heading
  if (useHeading < -180) useHeading += 360; //Fix Value Range
  if (useHeading > 180) useHeading -= 360; //Fix Value Range

  if (((useHeading < -15 || useHeading > 15) && rotateAmount == 0) && Speed > 60) Speed = 60;

  if (rotateAmount == 0 && Speed == 0) { //Rotate at 15 speed if Speed is 0 and rotate amount.
    rotateOnSpot = true;
    Speed = 15;
  }


  if (Speed > 10) { //If motors are running at reasonable speed run code.

    //Rotate Drive direction by 90 degrees
    Degrees -= 90;
    if (Degrees < 0) Degrees += 360;

    //    robotDirection = Degrees;


    //Calculate motor speed and direction
    mSpeed[0] = sin(radians(wDegree[0] - Degrees)) * Speed;
    mSpeed[1] = sin(radians(wDegree[1] - Degrees)) * Speed;
    mSpeed[2] = sin(radians(wDegree[2] - Degrees)) * Speed;
    mSpeed[3] = sin(radians(wDegree[3] - Degrees)) * Speed;


    int Max = mSpeed[0];
    int Min = mSpeed[0];

    for (int i = 0; i < 4; i++) { //Caluclate max and min of motors.
      if (mSpeed[i] < Min) Min = mSpeed[i];
      if (mSpeed[i] > Max) Max = mSpeed[i];
    }

    //Scale motor speed to match set speed
    mSpeed[0] = map(mSpeed[0], Min, Max, -Speed, Speed);
    mSpeed[1] = map(mSpeed[1], Min, Max, -Speed, Speed);
    mSpeed[2] = map(mSpeed[2], Min, Max, -Speed, Speed);
    mSpeed[3] = map(mSpeed[3], Min, Max, -Speed, Speed);

    if (useHeading < 0) { //??????
      if (haveBall == true && (Degrees == 45 || Degrees == 315)) rotate = map(useHeading, -180, 0, maxRotateSpeed, 30);
      else rotate = map(useHeading, -180, 0, maxRotateSpeed, 0);
    }
    else if (useHeading > 0) {
      if (haveBall == true && (Degrees == 45 || Degrees == 315)) rotate = map(useHeading, 0, 180, 30, maxRotateSpeed);
      else rotate = map(useHeading, 0, 180, 0, maxRotateSpeed);
    }
    else rotate = 0;

    //    Serial.println(rotate);

    //    float a = 0.0110119047619;
    //    float b = 0.122023809524;
    //    float c = 7.5;

    //    rotate = a * rotate * rotate + b * rotate + c; //Make it les vicious at low values but equally as strong at high (quadratic form ax^2+bx+c)

    //    Serial.println(rotate);

    //if (useHeading > 0) rotate *= -1;  //apply negative after quadratic to make course correction to left work
    /*
        Serial.println(rotate);

        rotate *= 0.6;

        float correctionSpeedOverTime;
        if (millis() - correctionTimer < 20) correctionSpeedOverTime = 0.3;
        else if (millis() - correctionTimer > 300) correctionSpeedOverTime = 1;
        else correctionSpeedOverTime = ((float)(millis() - correctionTimer) / 300) / 3.33333333 + 0.3;

        rotate += correctionSpeedOverTime * 0.4;

        Serial.println(rotate);
    */
    if (rotateOnSpot == true) { //If rotating on spot
      if (useHeading > 8) rotate = -100; //If to left
      else if (useHeading < -8) rotate = 100; //If to right
      else { //If it front
        mSpeed[0] = 0;
        mSpeed[1] = 0;
        mSpeed[2] = 0;
        mSpeed[3] = 0;
        Speed = 0;
        rotate = 0;
      }
    }

    if (rotateAmount != 0) rotate = rotateAmount; //If wanting to  rotate (not correct to heading) set rotate to rotateAmount.

    //Scale down
    mSpeed[0] *= float(100 - abs(rotate)) / 100.0;
    mSpeed[1] *= float(100 - abs(rotate)) / 100.0;
    mSpeed[2] *= float(100 - abs(rotate)) / 100.0;
    mSpeed[3] *= float(100 - abs(rotate)) / 100.0;

    //Add motor rotation speed
    mSpeed[0] += rotate / 100 * Speed;
    mSpeed[1] += rotate / 100 * Speed;
    mSpeed[2] += rotate / 100 * Speed;
    mSpeed[3] += rotate / 100 * Speed;
  }
  else { //If robot speed is < 10 don't do anything.
    mSpeed[0] = 0;
    mSpeed[1] = 0;
    mSpeed[2] = 0;
    mSpeed[3] = 0;
  }

  //Write motor speed and debug message
  //sprintf(buf, "M1 Speed, %d M2 Speed, %d M3 Speed, %d M4 Speed, %d", mSpeed[0], mSpeed[1], mSpeed[2], mSpeed[3]);
  //Serial.println(buf);
  turn(1, mSpeed[0]);
  turn(2, mSpeed[1]);
  turn(3, mSpeed[2]);
  turn(4, mSpeed[3]);
}
