//Average Current Function

void averageCurrent() {
  char output[50];
  for (int j = 0; j < 5; j++) {//Loop through all 5 current pins
    //Serial.print("Current ");
    //Serial.print(j);
    //Serial.print(":\t");
    //sprintf(output, "Size of current: %d", sizeof(avgCurrent));

    for (int i = ((sizeof(avgCurrent) / 4 ) / 5) - 1; i > 0; i--) { //Loop through size of average array
      //sprintf(output, "Before Val %d is %d and %d", i + 1, (int)avgCurrent[i][j], (int)avgCurrent[i - 1][j]);
      //Serial.println(output);
      avgCurrent[i][j] = avgCurrent[i - 1][j]; //Move 1 position down array
      //sprintf(output, "After Val %d is %d and %d", i + 1, (int)avgCurrent[i][j], (int)avgCurrent[i - 1][j]);
      //Serial.println(output);
    }

    avgCurrent[0][j] = ((3.3 / 4095.0) * (float)analogRead(currentPin[j]) + 0.14) / 0.1 / 2; //Add new current value to array
    //    sprintf(output, "\tCurrent Channel: %d, Val %d is %d", j, 1, (int)avgCurrent[0][j]);
    //    Serial.println(output);

    //Average array
    for (int i = 0; i < ((sizeof(avgCurrent) / 4 ) / 5) - 1; i++) {
      current[j] += avgCurrent[i][j];
    }
    current[j] /= ((sizeof(avgCurrent) / 4 ) / 5);
    /*
        Serial.print("\tMotor: ");
        Serial.print(j+1);
        Serial.print(", Amps: ");
        Serial.print(current[j], 2);
    */
  }
}
