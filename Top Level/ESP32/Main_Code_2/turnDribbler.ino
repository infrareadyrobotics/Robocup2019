//Turn Dribbler Function

void turnDribbler(float pwm) {
  pwm *= options.started; //Turn PWM signal on or off depend on variable state.
  if (current[4] >= 2) { // amps before pwm fall off
    if (cTimer1[4] < (millis() - 50)) { //Time before pwm fall off
      if (pwmLimit[4] > 20 && cTimer2[4] < (millis() - 50)) { // min pwm percent
        cTimer2[4] = millis();
        pwmLimit[4] -= 5; // Pwm decrement per time interval
        dribblerCurrentLimited = true;
      }
    }
  }
  else if (pwmLimit[4] != 100) { // if pwm is not at 100%
    if (pwmLimit[4] < 100 && cTimer2[4] < (millis() - 50)) { // increment until pwm is at or above 100% each time interval
      cTimer2[4] = millis();
      pwmLimit[4] += 10; // Pwm increment per time interval
    }
    if (pwmLimit[4] > 100)
    {
      pwmLimit[4] = 100; //set pwm to 100% if is above 100%
    }
  }
  else
  {
    cTimer1[4] = millis(); //reset timer if pwm is at 100% and the current is less than limited amount.
    dribblerCurrentLimited = false;
  }
  if (pwmLimit[4] < pwm && millis() > 1000) {//Doesn't limit in first 1 second due to current going stupid, reduces pwm only if its less limited pwm
    pwm = pwmLimit[4];
  }

  pwmWrite(pwm, dribblerPwmChannel);
}
