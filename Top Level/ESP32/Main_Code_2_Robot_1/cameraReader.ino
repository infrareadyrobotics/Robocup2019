
void cameraWrite() {
  Serial2.write("1");
}
void cameraPrint(Blobs blobs) {
  for (int a = 0; a < 3; a++) { //catagorise data
    Serial.print("Colour"); Serial.print(a);
    for (int b = 0; b < 4; b++) {
      if (b == 0) Serial.print(", Colour:");
      else if (b == 1) Serial.print(", Angle:");
      else if (b == 2) Serial.print(", Distance:");
      else if (b == 3) Serial.print(", Size:");
      Serial.print(blobs.array[a][b]);
    }
    Serial.print("\t");
  }
}
Blobs cameraRead() {
  // // Blobs blobs;
  // for (int a = 0; a < 3; a++) { //Allocate 0s and object number to data so it is allocated
  //   for (int b = 0; b < 4; b++) {
  //     if (b == 0) blobs.array[a][0] = a;
  //     else blobs.array[a][b] = 0;
  //   }
  // }        
  int rawData[32];
  for (int i = 0; Serial2.available(); i++) { //read all available data
    rawData[i] = Serial2.read();
  }
  for (int i = 0; i < 32; i++) { //????
    byte locked = 0;
    byte lockPoint;
    if (rawData[i] == 255) { //lock on start of data packet to prevent misalignment
      lockPoint = i;
      locked = 1;
    }
    if (locked == 1) {
      int addr = 0;
      for (int a = 0; a < 3; a++) { //catagorise data
        for (int b = 0; b < 4; b++) {
          addr++;
          blobs.array[a][b] = rawData[addr + lockPoint];
        }
        blobs.array[a][1] = blobs.array[a][1] * 4.5 / PI - 90 + heading[0]; //convert from scaled radiaans to degrees

        if (a != 0){ //Remap goal data into cm
          if (blobs.array[a][2] < 90){ //Use one equation
            blobs.array[a][2] = cameraDistanceRemapVariables[0] * pow(blobs.array[a][2], 2) + cameraDistanceRemapVariables[1] * blobs.array[a][2] + cameraDistanceRemapVariables[2];
          }
          else { //Use other one 
            blobs.array[a][2] = -1.59399966004 * blobs.array[a][3] + 147.963624001;
          }
        }

        if (blobs.array[a][1] < -180) blobs.array[a][1] += 360;
        else if (blobs.array[a][1] > 180) blobs.array[a][1] -= 360;
      }
      break;
    }
  }
  return blobs;
}
