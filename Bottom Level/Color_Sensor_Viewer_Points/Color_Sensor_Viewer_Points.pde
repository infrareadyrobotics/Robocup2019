import processing.serial.*;

Serial myPort;
String data;
String[] decodeData;
float scale = 2;
void setup() {
  try {
    //printArray(Serial.list());
    String portName = Serial.list()[1]; //change the 0 to a 1 or 2 etc. to match your port
    myPort = new Serial(this, portName, 115200);
  }
  catch(ArrayIndexOutOfBoundsException e) {
    println("Serial port disconnected");
  }

  myPort.clear();
  size(600, 600);
  background(255);
}
void draw() {
  translate(width/2, height/2);
  background(255);
  while (myPort.available() > 0) {
    data = myPort.readStringUntil(10);
    if (data == null) {
      return;
    } 
    else {
      decodeData = splitTokens(data, ",");
      for (int i = 0; i < decodeData.length/2; i++) {
        println(decodeData);
        fill(255);
        strokeWeight(8);
        ellipse(int(decodeData[i*2]) * scale, int(decodeData[i*2 + 1]) * scale, 25.0, 25.0);
      }
    }
  }
}

//void draw() {
//  if ( myPort.available() > 0) {  // If data is available,
//    val = myPort.readStringUntil('\n');         // read it and store it in val
//  } 
//  println(val); //print it out in the console
//}
