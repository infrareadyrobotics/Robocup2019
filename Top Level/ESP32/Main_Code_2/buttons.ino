int buttonCode(Button& button) {
  int returnData = 0;
  if (button.buttonData == 0) {
    button.timer1  = millis();
  }
  else {
    if (button.timer1  > button.timer2) {
      if ((button.timer1  - button.timer2) > 40) {
        buttonPressed();
        returnData = 1;
        Serial.print("button");
      }
    }
    button.timer2  = millis();
    return returnData;
  }
}
void buttonPressed() {
  //  buttonTime  = buttonTimer  - buttonTimer2 ;
  //  Serial.println(buttonTime);
  //  if (buttonTime  > 50 && buttonTime  <= 1000) onOff();
  //  else if (buttonTime  > 1000 && buttonTime  <= 3000) calibrate();
  //  else if (buttonTime  > 3000) kick();
}
void toggle(int x, int& y) {
  if (x == 1) {
    if (y == 1) {
      y = 0;
    } else {
      y = 1;
    }
  }
}
