struct Blobs { //object is: 1 ball, 2 yellow goal, 3 blue goal
  int array[3][4]; //[object][object, degrees, distance, averageSideLengthOfSquare];
};
struct ATtinyData {
  int buttons[8];
  int switches[8];
  int ultrasonics[8];
  int colors[32];
};
struct Button {
  int timer1;
  int timer2;
  int timePressed;
  int buttonData;
};
struct Options {
  int started;
};
struct BluetoothData{
  int role;
  int started;
  int haveBall;
};