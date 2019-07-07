//Get Points Function

void getPoints() {
  int whitePoints[24];
  int whitePointsNum = 0;
  for (int i = 0; i < 24; i++) {//Gets all white points and put in array with there number
    if (colourSensorColour[i] == white) {
      whitePoints[whitePointsNum] = i;
      whitePointsNum++;
    }
  }

  if (whitePointsNum == 0) {
    points[0][0] = 0;
    points[0][1] = 0;
    points[1][0] = 0;
    points[1][1] = 0;
    points[2][0] = 0;
    points[2][1] = 0;
    points[3][0] = 0;
    points[3][1] = 0;
    pointNum = 0;
    return;
  }

  //White Points Debug
  //  Serial.print("WhitePoints: ");
  //  for (int i = 0; i < whitePointsNum; i++) {
  //    Serial.print(whitePoints[i]); Serial.print(", ");
  //  }

  int blobs[4][whitePointsNum]; //Blob, Point Number: Holds Point Number
  int blobsPointsNum[4] = {0, 0, 0, 0}; //Blob: Holds Total Points for given blob
  int blobsNum = 0; //Blob Number
  if (whitePointsNum != 0) blobsNum++;

  //first white point is start of first blob
  blobs[blobsNum - 1][blobsPointsNum[blobsNum - 1]] = whitePoints[0]; //Adds point to list of current points in blob
  blobsPointsNum[blobsNum - 1] += 1; //Add one to current number of points in blob

  for (int i = 1; i < whitePointsNum; i++) { //Loop through all white points and check if can join blob else make new one
    //Check detected white point before in array for being next to each other.
    if (whitePoints[i] == whitePoints[i - 1] + 1) {
      blobs[blobsNum - 1][blobsPointsNum[blobsNum - 1]] = whitePoints[i]; //Adds point to list of current points in blob
      blobsPointsNum[blobsNum - 1] += 1; //Add one to current number of points in blob
    }

    else {//If new blob
      blobsNum++; //Increment Blob Number
      blobs[blobsNum - 1][blobsPointsNum[blobsNum - 1]] = whitePoints[i]; //Adds point to list of current points in blob
      blobsPointsNum[blobsNum - 1] += 1; //Add one to current number of points in blob
    }
  }

  //Merge blobs if correspondingly on 23 and 0
  if (whitePoints[0] == 0 && whitePoints[whitePointsNum - 1] == 23) {
    for (int i = 0; i < blobsPointsNum[blobsNum - 1]; i++) { //Add last blob to end of first blob for each blob in last blob
      blobs[0][blobsPointsNum[0] + i] = blobs[blobsNum - 1][i];
    }
    //Fix up values of first blob and remove last blob
    blobsPointsNum[0] += blobsPointsNum[blobsNum - 1];
    blobsNum--;
  }

  //Blobs Debug
  //  Serial.print("\tBlobs: ");
  //  for (int i = 0; i < blobsNum; i++) {
  //    Serial.print("Blob"); Serial.print(i + 1); Serial.print(": ");
  //    for (int j = 0; j < blobsPointsNum[i]; j++) {
  //      Serial.print(blobs[i][j]); Serial.print(", ");
  //    }
  //  }

  //Average Blobs to points and convert to cartesian
  for (int i = 0; i < blobsNum; i++) {
    float sumX = 0;
    float sumY = 0;
    for (int j = 0; j < blobsPointsNum[i]; j++) {
      sumX += colourSensorCartisanPos[blobs[i][j]][0]; //Add x component of point
      sumY += colourSensorCartisanPos[blobs[i][j]][1]; //Add y component of point
    }
    points[i][0] = sumX / blobsPointsNum[i]; //Get average of X
    points[i][1] = sumY / blobsPointsNum[i]; //Get average of Y
    pointNum = blobsNum;
  }
}
