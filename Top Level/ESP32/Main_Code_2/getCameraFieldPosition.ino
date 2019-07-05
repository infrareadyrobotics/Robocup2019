//Get Camera Field Postion Function

void getCameraFieldPosition(){
  blobs = cameraRead();
  if (blobs.array[1][1] > cameraFieldSideThreshold && blobs.array[1][1] < 180 - cameraFieldSideThreshold) { //If Blue goal on right
    if (blobs.array[2][1] > cameraFieldSideThreshold && blobs.array[2][1] < 180 - cameraFieldSideThreshold) { //If Yellow goal on right
      cameraFieldSide = 2;
    }
  }
  else if (blobs.array[1][1] < -cameraFieldSideThreshold && blobs.array[1][1] > -180 + cameraFieldSideThreshold) {
    if (blobs.array[2][1] < -cameraFieldSideThreshold && blobs.array[2][1] > -180 + cameraFieldSideThreshold) {
      cameraFieldSide = 1;
    }
  }
  else cameraFieldSide = 0;

  if (goalColour == blue) {
    
    //Blue
    float x1 = -(blobs.array[2][2] * sin(blobs.array[2][1] / radiansToDegrees));
    float y1 = fieldLength / 2 - (blobs.array[2][2] * cos(blobs.array[2][1] / radiansToDegrees));

    //Yellow
    float x2 = -(blobs.array[1][2] * sin(blobs.array[1][1] / radiansToDegrees));
    float y2 = -(fieldLength / 2 + (blobs.array[1][2] * cos(blobs.array[1][1] / radiansToDegrees)));

    // Serial.print("\tblueX: "); Serial.print(x1);
    // Serial.print("\tblueY: "); Serial.print(y1);
    // Serial.print("\tyellowX: "); Serial.print(x2);
    // Serial.print("\tyellowY: "); Serial.print(y2);

    if (blobs.array[1][3] < 10 || abs(x2) > 90 || abs(y2) > 120){ //Bad yellow goal data
      if (blobs.array[2][3] < 10 || abs(x1) > 90 || abs(y1) > 120){ //Bad blue goal data
        cameraFieldPositionValid = false;
      }
      else { //If good blue goal data 
        cameraFieldPositionX = x1;
        cameraFieldPositionY = y1;
        cameraFieldPositionValid = true;
      }
    }
    else {//Yellow goal data good
      if (blobs.array[2][3] < 10 || abs(x1) > 90 || abs(y1) > 120){ //Blue goal data bad
        cameraFieldPositionX = x2;
        cameraFieldPositionY = y2;
        cameraFieldPositionValid = true;
      }
      else { //Blue goal data good
        cameraFieldPositionX = (x1 + x2) / 2;
        cameraFieldPositionY = (y1 + y2) / 2;
        cameraFieldPositionValid = true;
      }
    }
  }
  else if (goalColour == yellow) {
        
    //Blue
    float x1 = -(blobs.array[2][2] * sin(blobs.array[2][1] / radiansToDegrees));
    float y1 = -(fieldLength / 2 + (blobs.array[2][2] * cos(blobs.array[2][1] / radiansToDegrees)));

    //Yellow
    float x2 = -(blobs.array[1][2] * sin(blobs.array[1][1] / radiansToDegrees));
    float y2 = fieldLength / 2 - (blobs.array[1][2] * cos(blobs.array[1][1] / radiansToDegrees));

    // Serial.print("\tblueX: "); Serial.print(x1);
    // Serial.print("\tblueY: "); Serial.print(y1);
    // Serial.print("\tyellowX: "); Serial.print(x2);
    // Serial.print("\tyellowY: "); Serial.print(y2);

    if (blobs.array[1][3] < 10 || abs(x2) > 90 || abs(y2) > 120){ //Bad yellow goal data
      if (blobs.array[2][3] < 10  || abs(x1) > 90 || abs(y1) > 120){ //Bad blue goal data
        cameraFieldPositionValid = false;
      }
      else { //If good blue goal data 
        cameraFieldPositionX = x1;
        cameraFieldPositionY = y1;
        cameraFieldPositionValid = true;
      }
    }
    else {//Yellow goal data good
      if (blobs.array[2][3] < 10 || abs(x1) > 90 || abs(y1) > 120){ //Blue goal data bad
        cameraFieldPositionX = x2;
        cameraFieldPositionY = y2;
        cameraFieldPositionValid = true;
      }
      else { //Blue goal data good
        cameraFieldPositionX = (x1 + x2) / 2;
        cameraFieldPositionY = (y1 + y2) / 2;
        cameraFieldPositionValid = true;
      }
    }
  }

  if(cameraFieldPositionValid == true){
    for (int k = 0; k < cameraFilterLength - 1; k++){ //Shift data for each 
      cameraFieldXFilterData[k] = cameraFieldXFilterData[k + 1];
      cameraFieldYFilterData[k] = cameraFieldYFilterData[k + 1];
    }
    //Add new data
    cameraFieldXFilterData[cameraFilterLength - 1] = cameraFieldPositionX;
    cameraFieldYFilterData[cameraFilterLength - 1] = cameraFieldPositionY;
    
    cameraFieldPositionX = medianFilterCamera(cameraFieldXFilterData);
    cameraFieldPositionY = medianFilterCamera(cameraFieldYFilterData);
  }
  // Serial.print("\tcameraFieldPositionX: "); Serial.print(cameraFieldPositionX);
  // Serial.print("\tcameraFieldPositionY: "); Serial.print(cameraFieldPositionY);

  //Debug camera predicted field side (left/right)
  // Serial.print("\tcameraFieldSide: "); Serial.print(cameraFieldSide);  
}


float medianFilterCamera(float _values[cameraFilterLength]){
  int temp;
  float values[cameraFilterLength];
  for (int i = 0; i < cameraFilterLength; i++) { //changes pointer so arrays are isolated
    values[i] = _values[i]; 
  }
  //Sort values in accending order
  for (int i = 0; i < cameraFilterLength; i++){
    for (int j = i + 1; j < cameraFilterLength; j++){
      if(values[j] < values[i]){
        temp = values[i];
        values[i] = values[j];
        values[j] = temp;
      }
    }
  }

  // for (int i = 0; i < cameraFilterLength; i++) {
  //   Serial.print("\tReading"); Serial.print(i); Serial.print(": "); Serial.print(values[i]);
  // }

  //Get median
  float median;
  if (cameraFilterLength % 2 == 1) median = values[cameraFilterLength/2];
  else median = (values[cameraFilterLength/2] + values[cameraFilterLength/2 + 1]) / 2;

  // Serial.print("\tMedian: "); Serial.print(median, 1);
  return median;
}