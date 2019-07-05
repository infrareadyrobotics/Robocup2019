void chaseBallLogic() {
  dribblerOn = true;
  if(blobs.array[0][3] != 0) { //If reading is actually valid
    if (blobs.array[0][2] > 60){ //If far away
      drive(blobs.array[0][1], 0, 90, 0);
      Serial.print("\tFaraway");
    }
    else { //If close to robot
      Serial.print("\tClose, ");
      if (blobs.array[0][1] < 15 && blobs.array[0][1] > -15) { //Don't rotate drive direction
        drive(blobs.array[0][1], 0, 70, 0);
        Serial.print("Forwards section");
      }
      else { //Rotate drive direction to get behind ball
        Serial.print("backwards Section");
        if (blobs.array[0][1] > 180 - chaseBallAngleBehind || blobs.array[0][1] < -180 + chaseBallAngleBehind){ //If in very back area (stop switching between sides by picking one based on field side)
          if (xSign < 0) { //If on left
            int driveAngle = blobs.array[0][1];
            if (blobs.array[0][2] < 15) driveAngle -= chaseBallAngleBehind * 3; //Drive a bit anti-clockwise to the ball angle. more because close
            else driveAngle -= chaseBallAngleBehind; //Drive a bit anti-clockwise to the ball angle. less because far

            if (driveAngle < -180) driveAngle += 360; //Fix Value Range
            else if (driveAngle > 180) driveAngle -= 360; //Fix Value Range
            drive(driveAngle, 0, 70, 0);
          }
          else { //If on right
            int driveAngle = blobs.array[0][1];
            if (blobs.array[0][2] < 15) driveAngle += chaseBallAngleBehind * 3; //Drive a bit clockwise to the ball angle. more because close
            else driveAngle += chaseBallAngleBehind; //Drive a bit clockwise to the ball angle. less because far

            if (driveAngle < -180) driveAngle += 360; //Fix Value Range
            else if (driveAngle > 180) driveAngle -= 360; //Fix Value Range
            drive(driveAngle, 0, 70, 0);
          }
        }
        else if (blobs.array[0][1] < 0) { //If to left
          int driveAngle = blobs.array[0][1];
          if (blobs.array[0][2] < 15) driveAngle -= chaseBallAngleBehind * 3; //Drive a bit anti-clockwise to the ball angle. more because close
          else driveAngle -= chaseBallAngleBehind; //Drive a bit anti-clockwise to the ball angle. less because far

          if (driveAngle < -180) driveAngle += 360; //Fix Value Range
          else if (driveAngle > 180) driveAngle -= 360; //Fix Value Range
          drive(driveAngle, 0, 70, 0);
        }
        else { //If to right
          int driveAngle = blobs.array[0][1];
          if (blobs.array[0][2] < 15) driveAngle += chaseBallAngleBehind * 3; //Drive a bit clockwise to the ball angle. more because close
          else driveAngle += chaseBallAngleBehind; //Drive a bit clockwise to the ball angle. less because far

          if (driveAngle < -180) driveAngle += 360; //Fix Value Range
          else if (driveAngle > 180) driveAngle -= 360; //Fix Value Range
          drive(driveAngle, 0, 70, 0);
        }
      }
    }
  }
  else { //Invalid reading

  }
}

void haveBallLogic() {
  Serial.print("\nhaveBallLogic!!!!");
  dribblerOn = true;
  float goalAngle;
  float goalDistance;
  if (goalColour == blue) { //Blue goal target
    goalAngle = blobs.array[2][1];
    goalDistance = blobs.array[2][2];
  }
  else if (goalColour == yellow){ //Yellow goal target
    goalAngle = blobs.array[1][1];
    goalDistance = blobs.array[1][2];
  }
  if (attinyData.ultrasonics[0] < 30 && goalDistance > 25) {//Robot/goal infront of robot
    driveForwardTimer = millis();
    if (ySign < 0) { //Robot on back side of field
      if (xSign < 0){ //Robot on left side of field
        drive(-90, 0, 90, 0);
      }
      else { //Robot on right side of field 
        drive(90, 0, 90, 0);
      }
    }
    else { //Robot on front side of field
      if (xSign < 0){ //Robot on left side of field
        drive(90, 0, 90, 0);
      }
      else { //Robot on right side of field 
        drive(-90, 0, 90, 0);
      }
    }
  }
  else { //area infront of robot clear
    drive(0, goalAngle, 90, 0); //Drive forwards facing goal
    if (goalAngle < heading[0] + 5 && goalAngle > heading[0] - 5) {
      driveForwardTimer = millis();
    }
    else {
      dribblerOn = false;
      turnDribbler(0);
      if (millis() - driveForwardTimer > dribblerTurnOffTime){
        Serial.print(" good to kick ");
        kick();
        dribblerOn = true;
      }
    }
  }

}

void onWhiteLogic(byte side, byte corner, int speed) {
    dribblerOn = true;
    runningOnWhiteCode = true;
    // Serial.print("\tSide: "); Serial.print(side);
    // Serial.print("\tCorner: "); Serial.print(corner);
    // Serial.print("\tSpeed: "); Serial.print(speed);
    if (corner == 1) drive(45,0,speed,0);
    else if (corner == 2) drive(135,0,speed,0);
    else if (corner == 3) drive(-135,0,speed,0);
    else if (corner == 4) drive(-45,0,speed,0);
    else if (side == 1) drive(0,0,speed,0);
    else if (side == 2) drive(90,0,speed,0);
    else if (side == 3) drive(180,0,speed,0);
    else if (side == 4) drive(-90,0,speed,0);
    runningOnWhiteCode = false;
}

void logicController(){
  switch (pointNum) {
    case 0: //No white points detected
      Serial.print("\tWhite Points 0");
      //run normal code
      if (haveBall == true) haveBallLogic();
      else chaseBallLogic();
      break;
    case 1: { //1 white points detected
      Serial.print("\tWhite Points 1");
      // char buf[100];
      // sprintf(buf, "\tPoint1: (%d, %d)", int(points[0][0]), int(points[0][1]));
      // Serial.print(buf);
      float r = sqrt(points[0][0] * points[0][0] + points[0][1] * points[0][1]);
      float deg = radiansToDegrees * atan2(points[0][1], points[0][0]);
      deg -= 90; //Rotate so when line infront angle is zero
      deg += heading[0];
      if (deg < -180) deg += 360;
      else if (deg > 180) deg -= 360;

      if (fieldFrontBack == 1 && (deg > -45 && deg < 45)) {//Front rotate 180 and subtract magnitude
          r -= 65;
          deg += 180;
          if (deg < -180) deg += 360;
          else if (deg > 180) deg -= 360;
        }
        else if (fieldFrontBack == 2 && (deg > 135 || deg < -135)) {//Back rotate 180 and subtract magnitude
          r -= 65;
          deg += 180;
          if (deg < -180) deg += 360;
          else if (deg > 180) deg -= 360;
        }
        else if (fieldRightLeft == 1 && (deg > 45 && deg < 135)) {//Right rotate 180 and subtract magnitude
          r -= 65;
          deg += 180;
          if (deg < -180) deg += 360;
          else if (deg > 180) deg -= 360;
        }

        else if (fieldRightLeft == 2 && (deg > -135 && deg < -45)) {//Left rotate 180 and subtract magnitude
          r -= 65;
          deg += 180;
          if (deg < -180) deg += 360;
          else if (deg > 180) deg -= 360;
        }

        // Serial.print("\tDegree: "); Serial.print(deg, 2);

        float speed;
        if (r < 0) speed = map(r , -65, 0, onWhiteMinSpeed, onWhiteMaxSpeed/2);
        else speed = map(r , 0, 65, onWhiteMaxSpeed/2, onWhiteMaxSpeed);
        
        byte cornerRange = 10; //In degrees, Note don't go more than 45 degrees.
        byte corner = 0; //1 frontRight, 2 backRight, 3 backLeft, 4 frontLeft
        if (deg > 45 - cornerRange && deg < 45 + cornerRange) corner = 1; //FrontRight
        else if (deg > 135 - cornerRange && deg < 135 + cornerRange) corner = 2; //BackRight
        else if (deg > -135 - cornerRange && deg < -135 + cornerRange) corner = 3; //BackLeft
        else if (deg > -45 - cornerRange && deg < -45 + cornerRange) corner = 4; //FrontLeft

        //Compute Line side
        byte side = 0; //1 front, 2 right, 3 back, 4 left
        if (corner == 0) { //Check sides after corner if corner not detected
          if (deg > -45 && deg < 45) side = 1; //Front
          else if (deg > 45 && deg < 135) side = 2; //Right
          else if (deg > 135 || deg < -135) side = 3; //back
          else if (deg > -135 && deg < -45) side = 4; //left
        }

        //Debug
        Serial.print("\tSide: "); Serial.print(side);
        Serial.print("\tCorner: "); Serial.print(corner);   

        if (corner != 0) { //Corner
          if (fieldCorner == ((corner + 1) % 4 + 1)) onWhiteLogic(0, corner, speed); //If corner is opposite to on side opposite
          else {//Run normal code
            if (haveBall == true) haveBallLogic();
            else chaseBallLogic();
          }
        }
        else if (side != 0) { //Side
          if(fieldFrontBack == 1 && side == 3) onWhiteLogic(side, 0, speed); //If on front section of field and line detected on back
          else if (fieldFrontBack == 2 && side == 1) onWhiteLogic(side, 0, speed); //If on back section of field and line detected on front
          else if (fieldRightLeft == 1 && side == 4) onWhiteLogic(side, 0, speed); //If on right section of field and line detected on left
          else if (fieldRightLeft == 2 && side == 2) onWhiteLogic(side, 0, speed); //If on left section of field and line detected on right
          else {//Run normal code no valid on white conditions
            if (haveBall == true) haveBallLogic();
            else chaseBallLogic();
          }
        }
        break;
    }
    case 2: {//2 white points detected, Note use of {} due to variables being declared
        Serial.print("\tWhite Points 2");

        //Note might want to scale points out into a constant radius / extend line to edge of circle so midpoint is more representing of chord of the cricle.
        float x1;
        float x2;
        float y1;
        float y2;
        if (!isinf((points[1][1] - points[0][1]) / (points[1][0] - points[0][0]))) { //If gradient not inf/undefined (vertical)
          float m = (points[1][1] - points[0][1]) / (points[1][0] - points[0][0]); //Gradient
          // Serial.print("\tm: "); Serial.print(m);
          float c = -m * points[0][0] + points[0][1]; //y intercept
          // Serial.print("\tc: "); Serial.print(c);
          x1 = (sqrt(4225 * m * m - c * c + 4225) - (m * c)) / (m * m + 1); //Calculate line intersect with circle of radius 65
          x2 = (-sqrt(4225 * m * m - c * c + 4225) - (m * c)) / (m * m + 1);
          y1 = m * x1 + c;
          y2 = m * x2 + c;
        }
        else { //Get positions if vertical
          x1 = points[0][0];
          x2 = points[1][0];
          y1 = sqrt(65 * 65 - x1 * x1);
          y2 = -sqrt(65 * 65 - x2 * x2);
        }

        char buf[100];
        // sprintf(buf, "\tPoint1: (%d, %d)\tPoint2: (%d, %d)", int(x1), int(y1), int(x2), int(y2));
        // Serial.print(buf);

        float midPointX = (x1 + x2) / 2;
        float midPointY = (y1 + y2) / 2;
        // sprintf(buf, "\tMid Point: (%d, %d)", int(midPointX), int(midPointY));
        // Serial.print(buf);

        //Convert to polar
        float r = sqrt(midPointX * midPointX + midPointY * midPointY);
        float deg = radiansToDegrees * atan2(midPointY, midPointX);

        deg -= 90; //Rotate so when line infront angle is zero

        //Stabalise with compass
        deg += heading[0];
        if (deg < -180) deg += 360;
        else if (deg > 180) deg -= 360;
        
        if (fieldFrontBack == 1 && (deg > -45 && deg < 45)) {//Front rotate 180 and subtract magnitude
          r -= 65;
          deg += 180;
          if (deg < -180) deg += 360;
          else if (deg > 180) deg -= 360;
        }
        else if (fieldFrontBack == 2 && (deg > 135 || deg < -135)) {//Back rotate 180 and subtract magnitude
          r -= 65;
          deg += 180;
          if (deg < -180) deg += 360;
          else if (deg > 180) deg -= 360;
        }
        else if (fieldRightLeft == 1 && (deg > 45 && deg < 135)) {//Right rotate 180 and subtract magnitude
          r -= 65;
          deg += 180;
          if (deg < -180) deg += 360;
          else if (deg > 180) deg -= 360;
        }

        else if (fieldRightLeft == 2 && (deg > -135 && deg < -45)) {//Left rotate 180 and subtract magnitude
          r -= 65;
          deg += 180;
          if (deg < -180) deg += 360;
          else if (deg > 180) deg -= 360;
        }

        Serial.print("\tDegree: "); Serial.print(deg, 2);

        float speed;
        if (r < 0) speed = map(r , -65, 0, onWhiteMinSpeed, onWhiteMaxSpeed/2);
        else speed = map(r , 0, 65, onWhiteMaxSpeed/2, onWhiteMaxSpeed);
        
        byte cornerRange = 10; //In degrees, Note don't go more than 45 degrees.
        byte corner = 0; //1 frontRight, 2 backRight, 3 backLeft, 4 frontLeft
        if (deg > 45 - cornerRange && deg < 45 + cornerRange) corner = 1; //FrontRight
        else if (deg > 135 - cornerRange && deg < 135 + cornerRange) corner = 2; //BackRight
        else if (deg > -135 - cornerRange && deg < -135 + cornerRange) corner = 3; //BackLeft
        else if (deg > -45 - cornerRange && deg < -45 + cornerRange) corner = 4; //FrontLeft

        //Compute Line side
        byte side = 0; //1 front, 2 right, 3 back, 4 left
        if (corner == 0) { //Check sides after corner if corner not detected
          if (deg > -45 && deg < 45) side = 1; //Front
          else if (deg > 45 && deg < 135) side = 2; //Right
          else if (deg > 135 || deg < -135) side = 3; //back
          else if (deg > -135 && deg < -45) side = 4; //left
        }

        //Debug
        Serial.print("\tSide: "); Serial.print(side);
        Serial.print("\tCorner: "); Serial.print(corner);   

        if (corner != 0) { //Corner
          if (fieldCorner == ((corner + 1) % 4 + 1)) onWhiteLogic(0, corner, speed); //If corner is opposite to on side opposite
          else {//Run normal code
            if (haveBall == true) haveBallLogic();
            else chaseBallLogic();
          }
        }
        else if (side != 0) { //Side
          if(fieldFrontBack == 1 && side == 3) onWhiteLogic(side, 0, speed); //If on front section of field and line detected on back
          else if (fieldFrontBack == 2 && side == 1) onWhiteLogic(side, 0, speed); //If on back section of field and line detected on front
          else if (fieldRightLeft == 1 && side == 4) onWhiteLogic(side, 0, speed); //If on right section of field and line detected on left
          else if (fieldRightLeft == 2 && side == 2) onWhiteLogic(side, 0, speed); //If on left section of field and line detected on right
          else {//Run normal code no valid on white conditions
            if (haveBall == true) haveBallLogic();
            else chaseBallLogic();
          }
        }
        break;
      }
    case 3: {//3 white points detected, Note use of {} due to variables being declared
        Serial.print("\tWhite Points 3 STOPPING CURRENTLY DUE TO NOT BEING IMPLEMENTED");
        // drive(0,0,0,0);

        float angles[3];
        for (int i = 0; i < 3; i++) {
          float tempA1 = atan2(points[i][1] - points[(i + 1) % 3][1], points[i][0] - points[(i + 1) % 3][0]);
          float tempA2 = atan2(points[(i + 1) % 3][1] - points[(i + 2) % 3][1], points[(i + 1) % 3][0] - points[(i + 2) % 3][0]);
          angles[i] = (tempA2 - tempA1) * radiansToDegrees;
          Serial.print("\tAngle: "); Serial.print(angles[i]);
        }

        //if (centre outside corner) //Run get back on field
        //else //Run normal code
        break;
      }
  }
}

void haveBallCalc (){
  boolean check1 = blobs.array[0][1] < 15 && blobs.array[0][1] > -15; //Ball infront with camera
  boolean check2 = current[4] > 1.00 || dribblerCurrentLimited; //If dribbler is drawing more than 1 amp or is currently being current limited.
  if (check1 && check2){ //Has ball
    haveBall = true; //Set haveball to true
    haveBallCalcTimer = millis(); //Reset Timer
  }
  else if (millis() - haveBallCalcTimer > haveBallCalcTimeOut){ //If lost ball for longer than timeout (avoids intermitent switching between states)
    haveBall = false; //set haveball to false
  }
}