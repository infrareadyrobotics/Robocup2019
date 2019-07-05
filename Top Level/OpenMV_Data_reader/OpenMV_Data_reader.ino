void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
}
  char num;
  int thing;
  float number = 0;
  byte* numberPtr = (byte*) &number;
  int rawData[32];
  int blobs[3][4];

void loop() {
//  delay(30);
  for (int i = 0; i < 32; i++) {
    rawData[i] = NULL;
  }
  for (int a = 0; a < 6; a++) {
    for (int b = 0; b < 3; b++) {
      blobs[a][b] = NULL;
    }
  }
  Serial2.write("1");

  if (Serial2.available() > 0) {
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.print("\tcde\tagl\tdst\tsze\tcde\tagl\tdst\tsze\tcde\tagl\tdst\tsze\tcde\tagl\tdst\tsze\tcde\tagl\tdst\tsze\tcde\tagl\tdst\tsze\t");
    Serial.println();
    Serial.print("a: ");
  }
  
  for (int i = 0; Serial2.available(); i++) {
    rawData[i] = Serial2.read();
    Serial.print(rawData[i]);
    Serial.print("\t");

  }
  for (int i = 0; i < 32; i++) {
    byte locked = 0;
    byte lockPoint;
    if (rawData[i] == 255) {
      lockPoint = i;
      locked = 1;
    }
    if (locked == 1) {
      int addr = 0;
            Serial.println();
      Serial.print("b: \t");
      for (int a = 0; a < 3; a++) {
        for (int b = 0; b < 4; b++) {
          addr++;
          blobs[a][b] = rawData[addr + lockPoint];
//          Serial.print(blobs[a][b]);
//          Serial.print("\t");
        }
        blobs[a][1] = blobs[a][1] * 4.5 / PI;
      }
      for (int a = 0; a < 3; a++) {
        for (int b = 0; b < 4; b++) {
          Serial.print(blobs[a][b]);
          Serial.print("\t");
        }
      }
      break;
    }
  }
}
