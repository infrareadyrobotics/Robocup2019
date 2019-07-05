//Calibrate Function

void calibrate(){
    Serial.print("\nCalibrating: ");
    EEPROM.begin(128); //Start EEPROM with 128 bytes avalible

    //Gyro
    Serial.print("Gyro, ");
    delay(500);
    //Stop to ensure still
    unsigned long Timer = micros();
    double i = 0;
    double gyroData[3];
    sensors_event_t gevent;
    gyroObject.getEvent(&gevent);
    while (micros() - Timer < 15 * 1000000){ //Run for 15 seconds 
        i++; //increment count
        //add data to total
        gyroData[0] += gevent.gyro.x;
        gyroData[1] += gevent.gyro.y;
        gyroData[2] += gevent.gyro.z;
        delay(10);  //Loop at 100 readings per second
    }

    //Compute average
    gyroDirftCal[0] = gyroData[0] / i;
    gyroDirftCal[1] = gyroData[1] / i;
    gyroDirftCal[2] = gyroData[2] / i;

    //EEPROM
    EEPROM.writeFloat(0,float(gyroDirftCal[0]));
    EEPROM.writeFloat(4,float(gyroDirftCal[1]));
    EEPROM.writeFloat(8,float(gyroDirftCal[2]));

    //Colour sensors
    Serial.print("ColourSensors, ");
    calibrateColourSensors();
    
    //EEPROM
    for (int j = 0; j < 24; j++){
        EEPROM.writeInt(12 + j * 4, colourSensorCorrect[j]);
    }

    //Compass Zero
    Serial.print("CompassZero, ");
   
    unsigned long Timer1 = micros();
    double k = 0;
    double compassData[3];
    while (micros() - Timer1 < 5 * 1000000){ //Run for 5 seconds 
        k++; //increment count
        //add data to total
        readIMU();
        calibrateIMU();
        computeIMUHeading();
        
        compassData[0] += compassHeadingRaw[0];
        compassData[1] += compassHeadingRaw[1];
        compassData[2] += compassHeadingRaw[2];

        delay(10);  //Loop at 100 readings per second
    }

    //Compute Average
    compassHeadingZeroVal[0] = compassData[0] / k;
    compassHeadingZeroVal[1] = compassData[1] / k;
    compassHeadingZeroVal[2] = compassData[2] / k;

    //Reset Gyro
    gyroHeadingRad[0] = compassHeadingZeroVal[2] / radiansToDegrees;
    gyroHeadingRad[1] = compassHeadingZeroVal[1] / radiansToDegrees;
    gyroHeadingRad[2] = compassHeadingZeroVal[0] / radiansToDegrees;
    //EEPROM
    EEPROM.writeFloat(108, compassHeadingZeroVal[0]);
    EEPROM.writeFloat(112, compassHeadingZeroVal[1]);
    EEPROM.writeFloat(116, compassHeadingZeroVal[2]);

    //EEPROM commit
    Serial.print(", Commiting To EEPROM");
    EEPROM.commit();

    Serial.print("\n");
}

void loadEEPROM(){
    Serial.print("\nLoadingCalibration; ");
    EEPROM.begin(128); //Start EEPROM with 128 bytes avalible
    
    //Gyro
    Serial.print("Gyro:");
    gyroDirftCal[0] = EEPROM.readFloat(0);
    gyroDirftCal[1] = EEPROM.readFloat(4);
    gyroDirftCal[2] = EEPROM.readFloat(8);

    Serial.print(" "); Serial.print(gyroDirftCal[0], 6);
    Serial.print(" "); Serial.print(gyroDirftCal[1], 6);
    Serial.print(" "); Serial.print(gyroDirftCal[2], 6);
    Serial.print(", ");
    
    //Colour sensors
    Serial.print("Colour Sensors:");
    for (int j = 0; j < 24; j++){
        colourSensorCorrect[j] = EEPROM.readInt(12 + j * 4);
        Serial.print(" "); Serial.print(colourSensorCorrect[j]);
    }
    Serial.print(", ");

    //Compass Zero
    Serial.print("Compass Zero:");
    compassHeadingZeroVal[0] = EEPROM.readFloat(108);
    compassHeadingZeroVal[1] = EEPROM.readFloat(112);
    compassHeadingZeroVal[2] = EEPROM.readFloat(116);

    //Zero the gyro
    readIMU();
    calibrateIMU();
    computeIMUHeading();
    gyroHeadingRad[0] = compassHeading[2] / radiansToDegrees;
    gyroHeadingRad[1] = compassHeading[1] / radiansToDegrees;
    gyroHeadingRad[2] = compassHeading[0] / radiansToDegrees;
    
    Serial.print(" "); Serial.print(compassHeadingZeroVal[0], 6);
    Serial.print(" "); Serial.print(compassHeadingZeroVal[1], 6);
    Serial.print(" "); Serial.print(compassHeadingZeroVal[2], 6);
}