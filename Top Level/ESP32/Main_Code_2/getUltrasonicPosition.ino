    // Get Ultrasonic Position Function

void getUltrasonicPosition(){
char buf[100];
  float angle = int(heading[0] + 180) % 90; //Gets angle mod 90 of 0-360 so in range from 0-90
  if (angle > 90 - angularReliablityDistance || angle < angularReliablityDistance) { //If with in +-angularReliablityDistance of a 90 degree angle
    float ultraData[4];
    for (int i = 0; i < 4; i++) ultraData[i] = medianFilterUltrasonics(ultrasonicsFilterArray[i]); //Apply median filter to filter out high and low noise
    
    attinyData.ultrasonics[0] = ultraData[0];

    //Debug for calibration and debuging of sensor data exspecially after filter
    // sprintf(buf, "\tFront: %d, Right: %d, Back: %d, Left: %d", int(ultraData[0]), int(ultraData[1]), int(ultraData[2]), int(ultraData[3]));
    // sprintf(buf, "%d\t%d\t%d\t%d", int(ultraData[0]), int(ultraData[1]), int(ultraData[2]), int(ultraData[3]));
    // Serial.print(buf);

    //create seperate var so confusion/data overwrite doesn't occur
    float old[4];
    for (int i = 0; i < 4; i++) old[i] = ultraData[i]; 

    //Remap ultrasonics depending on heading
    if (heading[0] > -45 && heading[0] < 45) { //Front
      ultraData[0] = old[0];
      ultraData[1] = old[1];
      ultraData[2] = old[2];
      ultraData[3] = old[3];
    }
    else if (heading[0] > 45 && heading[0] < 135) { //Right
      ultraData[0] = old[3];
      ultraData[1] = old[0];
      ultraData[2] = old[1];
      ultraData[3] = old[2];
    }
    else if (heading[0] < -135 || heading[0] > 135) { //Back
      ultraData[0] = old[2];
      ultraData[1] = old[3];
      ultraData[2] = old[0];
      ultraData[3] = old[1];
    }
    else if (heading[0] > -135 && heading[0] < -45) { //Left
      ultraData[0] = old[1];
      ultraData[1] = old[2];
      ultraData[2] = old[3];
      ultraData[3] = old[0];
    }

    //Debug sums for left-right and front-back after rotation to correct ultrasonics to right side.
    // Serial.print("\tLateral Sum: "); Serial.print(ultraData[1] + ultraData[3]);
    // Serial.print("\tFrontback Sum: "); Serial.print(ultraData[0] + ultraData[2]);

    //Check if total lateral distance is valid
    if (ultraData[1] + ultraData[3] > ultrasonicsFieldWidth - ultrasonicsDistanceVariance && ultraData[1] + ultraData[3] < ultrasonicsFieldWidth + ultrasonicsDistanceVariance) {
      ultrasonicsPositionX = ((ultraData[3] - ultrasonicsFieldWidth / 2) + (ultrasonicsFieldWidth / 2 - ultraData[1])) / 2; //If valid compute x position
      ultrasonicsPositionXValid = true;
    }
    else ultrasonicsPositionXValid = false;  //If invalid set var to false

    
    boolean check1 = false;
    boolean check2 = false;
    
    
    if (abs(fieldPositionX) < ultrasonicsGoalWidth / 2) { //If within goal area
        check1 = ultrasonicsFieldGoalsLength - ultrasonicsDistanceVariance < ultraData[0] + ultraData[2] && ultrasonicsFieldGoalsLength + ultrasonicsDistanceVariance > ultraData[0] + ultraData[2]; //Goal to goal reading is valid
    }
    else {
        check2 = ultrasonicsFieldLength - ultrasonicsDistanceVariance < ultraData[0] + ultraData[2] && ultrasonicsFieldLength + ultrasonicsDistanceVariance > ultraData[0] + ultraData[2]; //field end to field end reading is valid
    }
     
    if (check1 || check2) { //If up/down reading is valid
      ultrasonicsPositionY = ((ultraData[2] - ultrasonicsFieldWidth / 2) + (ultrasonicsFieldWidth / 2 - ultraData[0])) / 2; //Compute vertical reading
      ultrasonicsPositionYValid = true; //set to valid
    }
    else ultrasonicsPositionYValid = false; //set to unvalid
  }
  else { //If not valid angle set position valid checks to false
    ultrasonicsPositionXValid = false;
    ultrasonicsPositionYValid = false;
  }

  //Debug Ultrasonics Estimated Position and If valid reading
  // sprintf(buf, "\tXPos: %d, YPos: %d, XPosValid: %d, YPosValid: %d", int(ultrasonicsPositionX), int(ultrasonicsPositionY), int(ultrasonicsPositionXValid), int(ultrasonicsPositionYValid));
  // Serial.print(buf);
}

//Calculate Mean of Given Array of pre defined size
float medianFilterUltrasonics(int _values[ultrasonicsFilterLength]){
  int temp;
  int values[ultrasonicsFilterLength];
  for (int i = 0; i < ultrasonicsFilterLength; i++) { //changes pointer so arrays are isolated
    values[i] = _values[i]; 
  }
  //Sort values in accending order
  for (int i = 0; i < ultrasonicsFilterLength; i++){
    for (int j = i + 1; j < ultrasonicsFilterLength; j++){
      if(values[j] < values[i]){
        temp = values[i];
        values[i] = values[j];
        values[j] = temp;
      }
    }
  }

  // for (int i = 0; i < ultrasonicsFilterLength; i++) {
  //   Serial.print("\tReading"); Serial.print(i); Serial.print(": "); Serial.print(values[i]);
  // }

  //Get median
  float median;
  if (ultrasonicsFilterLength % 2 == 1) median = values[ultrasonicsFilterLength/2];
  else median = (values[ultrasonicsFilterLength/2]+ values[ultrasonicsFilterLength/2 + 1]) / 2;

  // Serial.print("\tMedian: "); Serial.print(median, 1);
  return median;
}