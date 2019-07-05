//Pwm Write Function

void pwmWrite(int pwm, int channel) {
//  char buf[150];
//  sprintf(buf, "\tPwm: %d, Channel: %d\n", (int)pwm, channel);
//  Serial.print(buf);

  int pwmOn; //Time into pulse signal turns on. Normally Zero.
  int pwmOff; // Time into pulse signal turns off. Normal 4096 * pwm percent.

  if (pwm == 100) { //If pwm signal is 100% turn channel on.
    pwmOn = 4096;
    pwmOff = 0;
  }
  else if (pwm == 0) { //If pwm signal is 0% turn channel off.
    pwmOn = 0;
    pwmOff = 4096;
  }
  else {//Calculate pwm off value based on pwm percent of 4096.
    pwmOn = 0;
    pwmOff = 4096 * (pwm / 100.0);
  }

  pwmController.setPWM(channel, pwmOn, pwmOff); //Write pwmOn and pwmOff values to channel
  //  sprintf(buf, "\tpwmOn: %d, pwmOff: %d, sum: %d", pwmOn, pwmOff, pwmOn + pwmOff);
  //  Serial.print(buf);
}
