import processing.serial.*;

Serial myPort;
String data;
String[] decodeData;

void setup() {
  try {
    myPort = new Serial(this, Serial.list()[0], 115200);
  }
  catch(ArrayIndexOutOfBoundsException e) {
    println("Serial port disconnected");
  }

  myPort.clear();
  size(600, 600);
}
void draw() {
  while (myPort.available() > 0) {
    data = myPort.readStringUntil(10);
    if (data == null) {
      return;
    } else {
      decodeData = splitTokens(data, ",");
      for (int i =  0; i < 24; i ++) {
        println(decodeData);
        try {
          fill(int(decodeData[i])/4);
        }
        catch(ArrayIndexOutOfBoundsException e) {
          println("Serial Data is invalid / Can not connect to attiny");
        }
        ellipse( 250 * cos(radians(( i * 15 + 90 ) * -1 )) + 300, 250 * sin(radians(( i * 15 + 90) * -1 )) + 300, 40, 40);
      }
    }
  }
}
