//Drive Function

void drive(float Degrees, float targetHeading, int Speed, int rotateAmount) {
  int mSpeed[4];
  float rotate;
  float maxRotateSpeed = 150;
  char buf[100];

  //Calculate Correct Heading Target
  float useHeading = heading[0] - targetHeading; //Caluclate Heading amount reltaive to zeroed heading
  if (useHeading < -180) useHeading += 360; //Fix Value Range
  else if (useHeading > 180) useHeading -= 360; //Fix Value Range

  //Limit speed if predicted close to line
  // Serial.print("\tPremodifedSpeed: "); Serial.print(Speed);
  if(Speed > maxSpeedNearLine && (tactics == normal || techChallenge6 == normalMode)){ //Only perform speed limiting if speed is more than speed limit and not superteam
    if(runningOnWhiteCode == false){ //If not running onwhite code so to not interfer with code
      float yCloseness = (fieldLength / 2 - fabs(fieldPositionY));
      float xCloseness = (fieldWidth / 2 - fabs(fieldPositionX));
      // Serial.print("\txCloseness"); Serial.print(xCloseness);
      // Serial.print("\tyCloseness"); Serial.print(yCloseness);
      if (yCloseness < speedLimitNearLineStart || xCloseness < speedLimitNearLineStart) {//So as not to increase speed
        float xComponent;
        float yComponent;
        float fieldStabilisedDegrees = Degrees;
        fieldStabilisedDegrees += heading[0];
        if (fieldStabilisedDegrees < -180) fieldStabilisedDegrees += 360;
        else if (fieldStabilisedDegrees > 180) fieldStabilisedDegrees -= 360;
        if(fabs(fieldStabilisedDegrees) < 90){
          xComponent = map(fabs(fieldStabilisedDegrees), 0, 90, 0, 1000) / 1000.0;
          yComponent = map(fabs(fieldStabilisedDegrees), 0, 90, 1000, 0) / 1000.0;
        }
        else{
          xComponent = map(fabs(fieldStabilisedDegrees), 90, 180, 1000, 0) / 1000.0;
          yComponent = map(fabs(fieldStabilisedDegrees), 90, 180, 0, 1000) / 1000.0;
        }

        //Note use of fabs() instead of abs() as abs() doesn't support floats
        // Serial.print("\tradiansToDegrees: "); Serial.print(radiansToDegrees);
        // Serial.print("\tfieldStabilisedDegrees: "); Serial.print(fieldStabilisedDegrees);
        // Serial.print("\txComponent: "); Serial.print(xComponent, 4);
        // Serial.print("\tyComponent: "); Serial.print(yComponent, 4);
        //Map speed to slow down depending on angle xCloseness to speedlimitstart to end to speed to max speed and proportional to angle of attack on white line
        float newSpeedX = Speed;
        float newSpeedY = Speed;
        float newSpeed;
        if (xCloseness < speedLimitNearLineStart) {//So as not to increase speed
          if (fieldRightLeft == 2) { //Left
            if (fieldStabilisedDegrees < 0) {
              // Serial.print("\tLeft");
              newSpeedX = map(xCloseness, speedLimitNearLineStart, speedLimitNearLineEnd, Speed, maxSpeedNearLine); //Map speed from xCloseness to speedlimitstart to end to speed to max speed
            }
          }
          else { //Right
            if (fieldStabilisedDegrees > 0) {
              // Serial.print("\tRight");
              newSpeedX = map(xCloseness, speedLimitNearLineStart, speedLimitNearLineEnd, Speed, maxSpeedNearLine); //Map speed from xCloseness to speedlimitstart to end to speed to max speed
            }
          }
        }
        if (yCloseness < speedLimitNearLineStart) {//So as not to increase speed
          if(fieldFrontBack == 2) { //Back
            if (fieldStabilisedDegrees < -90 && fieldStabilisedDegrees > 90) {
              // Serial.print("\tBack");
              newSpeedY = map(yCloseness, speedLimitNearLineStart, speedLimitNearLineEnd, Speed, maxSpeedNearLine); //Map speed from yCloseness to speedlimitstart to end to speed to max speed
            }
          }
          else { //Front
            if (fieldStabilisedDegrees > -90 && fieldStabilisedDegrees < 90) {
              // Serial.print("\tFront");
              newSpeedY = map(yCloseness, speedLimitNearLineStart, speedLimitNearLineEnd, Speed, maxSpeedNearLine); //Map speed from yCloseness to speedlimitstart to end to speed to max speed
            }
          }
        }
        // Serial.print("\tnewSpeedY: "); Serial.print(newSpeedY);
        // Serial.print("\tnewSpeedX: "); Serial.print(newSpeedX);

        if (newSpeedX > newSpeedY){
          newSpeed = newSpeedY;
          Speed = float(Speed) * xComponent + yComponent * newSpeed; //Proportional to angle of attack on white line
        }
        else {
          newSpeed = newSpeedX;
          Speed = float(Speed) * yComponent + xComponent * newSpeed; //Proportional to angle of attack on white line
        }
      }
    }
  }
  Serial.print("\tmodifedSpeed: "); Serial.print(Speed);

  if(Speed < 0) { //If some idiot / algorithim is stupid and puts in a negative number flip direction and change speed to positive mainly in algorithim above so 
    Speed = abs(Speed); 
    // if (Speed > 100) Speed = 100;
    Degrees += 180;
    if (Degrees < -180) Degrees += 360; //Fix Value Range
    else if (Degrees > 180) Degrees -= 360; //Fix Value Range
  }

  // if (((useHeading < -15 || useHeading > 15) && rotateAmount == 0) && Speed > 60) Speed = 60; //If heading not corrected limit speed stops fish tailing

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

    // Serial.print("useHeading: "); Serial.print(useHeading);

    if (useHeading < 0) { //Calculate Rotation amount based on heading
      if (haveBall == true) {
        if (abs(useHeading) > 16) rotate = 25; //If haveball rotate at constant speed if not centred as to not lose ball
        else rotate = map(useHeading, -16, 0, 25, 5); //If not haveball rotate normally
      }
      else rotate = map(useHeading, -180, 0, maxRotateSpeed, 5); //If not haveball rotate normally
    }
    else if (useHeading > 0) {
      if (haveBall == true) {
        if (abs(useHeading) > 16) rotate = -25; //If haveball rotate at constant speed if not centred as to not lose ball
        else rotate = map(useHeading, 0, 16, -5, -25); //If not haveball rotate normally
      }
      else rotate = map(useHeading, 0, 180, -5, -maxRotateSpeed);
    }
    else rotate = 0;

    if (rotate > 99) rotate = 99; //Caps rotate speed at 99
    if (rotate < -99) rotate = -99; //Caps rotate speed at 99

       //    Serial.println(rotate);

    //    float a = 0.0110119047619;
    //    float b = 0.122023809524;
    //    float c = 7.5;

    //    rotate = a * rotate * rotate + b * rotate + c; //Make it les vicious at low values but equally as strong at high (quadratic form ax^2+bx+c)

    //    Serial.println(rotate);

    
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

    if (rotateAmount != 0) rotate = rotateAmount; //If wanting to  rotate (not correct to heading) set rotate to rotateAmount.

    // Serial.print("\tRotate: "); Serial.print(rotate);
    //Scale down
    mSpeed[0] *= float(100 - abs(rotate)) / 100.0;
    mSpeed[1] *= float(100 - abs(rotate)) / 100.0;
    mSpeed[2] *= float(100 - abs(rotate)) / 100.0;
    mSpeed[3] *= float(100 - abs(rotate)) / 100.0;

    //Add motor rotation speed
    float rotateSpeed = 80;

    mSpeed[0] += rotate / 100.0 * rotateSpeed;
    mSpeed[1] += rotate / 100.0 * rotateSpeed;
    mSpeed[2] += rotate / 100.0 * rotateSpeed;
    mSpeed[3] += rotate / 100.0 * rotateSpeed;
  }
  else { //If robot speed is < 10 don't do anything.
    mSpeed[0] = 0;
    mSpeed[1] = 0;
    mSpeed[2] = 0;
    mSpeed[3] = 0;

    //Rotate on spot if variable set to true
    if (rotateOnSpot == true) {
      // maxRotateSpeed = 150;
      if (useHeading < 0) { //Calculate Rotation amount based on heading
        if (haveBall == true) {
          if (abs(useHeading) > 16) rotate = 25; //If haveball rotate at constant speed if not centred as to not lose ball
          else rotate = map(useHeading, -16, 0, 25, 5); //If not haveball rotate normally
        }
        else rotate = map(useHeading, -180, 0, maxRotateSpeed, 5); //If not haveball rotate normally
      }
      else if (useHeading > 0) {
        if (haveBall == true) {
          if (abs(useHeading) > 16) rotate = -25; //If haveball rotate at constant speed if not centred as to not lose ball
          else rotate = map(useHeading, 0, 16, -5, -25); //If not haveball rotate normally
        }
        else rotate = map(useHeading, 0, 180, -5, -maxRotateSpeed);
      }
      else rotate = 0;

      if (rotate > 99) rotate = 99; //Caps rotate speed at 99
      if (rotate < -99) rotate = -99; //Caps rotate speed at 99

      float rotateSpeed = 80;
      mSpeed[0] = rotate / 100.0 * rotateSpeed;
      mSpeed[1] = rotate / 100.0 * rotateSpeed;
      mSpeed[2] = rotate / 100.0 * rotateSpeed;
      mSpeed[3] = rotate / 100.0 * rotateSpeed;
    }
  }

  //Write motor speed and debug message
  // sprintf(buf, "\tM1 Speed, %d M2 Speed, %d M3 Speed, %d M4 Speed, %d, Rotate: %d", mSpeed[0], mSpeed[1], mSpeed[2], mSpeed[3], int(rotate));
  // Serial.println(buf);
  turn(1, mSpeed[0]);
  turn(2, mSpeed[1]);
  turn(3, mSpeed[2]);
  turn(4, mSpeed[3]);
}
