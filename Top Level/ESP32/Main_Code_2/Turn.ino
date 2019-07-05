//Turn Function

void turn(int motor, float pwm) { //Turn motor by pwm
  pwm *= options.started; //Turn PWM signal on or off depend on variable state.
    char buf[80];
  //  sprintf(buf, "\tMotor, %d is running at %d PWM on channels, %d, %d ", motor, (int)pwm, inAPwm[motor - 1] , inBPwm[motor - 1] ); //Debug for function
  //  Serial.print(buf);
  if (current[motor - 1] >= 1.5) { // amps before pwm fall off
    if (cTimer1[motor - 1] < (millis() - 1000)) { //Time before pwm fall off
      if (pwmLimit[motor - 1] > 20 && cTimer2[motor - 1] < (millis() - 50)) { // min pwm percent
        cTimer2[motor - 1] = millis();
        pwmLimit[motor - 1] -= 5; // Pwm decrement per time interval
      }
    }
  }
  else if (pwmLimit[motor - 1] != 100) { // if pwm is not at 100%
    if (pwmLimit[motor - 1] < 100 && cTimer2[motor - 1] < (millis() - 50)) { // increment until pwm is at or above 100% each time interval
      cTimer2[motor - 1] = millis();
      pwmLimit[motor - 1] += 10; // Pwm increment per time interval
    }
    if (pwmLimit[motor - 1] > 100)
    {
      pwmLimit[motor - 1] = 100; //set pwm to 100% if is above 100%
    }
  }
  else
  {
    cTimer1[motor - 1] = millis(); //reset timer if pwm is at 100% and the current is less than limited amount.
  }

  //  Serial.print("PWMLIMIT");
  //  Serial.print(motor);
  //  Serial.print(":\t");
  //  Serial.print(pwmLimit[motor - 1]);
  //  Serial.print("\t");

  if (pwmLimit[motor - 1] < abs(pwm) && millis() > 1000) {//Doesn't limit in first 1 second due to current going stupid, reduces pwm only if its less limited pwm
    if (pwm < 0) pwm = -pwmLimit[motor - 1];
    else if (pwm > 0) pwm = pwmLimit[motor - 1];
  }

  if (pwm > 0) { //If pwm is positive
    //    Serial.println("\tPwm > 0");
    pwmWrite(int(pwm), inAPwm[motor - 1]);
    pwmWrite(0, inBPwm[motor - 1]);
  }
  else if (pwm < 0) { //If pwm is negative
    //    Serial.print("\tPwm < 0");
    pwm = abs(pwm);
    pwmWrite(0, inAPwm[motor - 1]);
    pwmWrite(int(pwm), inBPwm[motor - 1]);
  }
  else { //If pwm is zero
    //    Serial.print("\tPwm = 0");
    pwmWrite(0, inAPwm[motor - 1]);
    pwmWrite(0, inBPwm[motor - 1]);
  }
}
