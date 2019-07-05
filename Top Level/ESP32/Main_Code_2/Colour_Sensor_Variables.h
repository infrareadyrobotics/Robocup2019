//Colour Sensor Variables
int colourSensorData[24];
int colourSensorCorrect[24];
enum colour {white, green, black, error};
colour colourSensorColour[24];
float points[4][2];
int pointNum;
int const colourSensorRadii = 65; //Diameter 130MM
float colourSensorCartisanPos[24][2];

//Thresholds are minimum values for a given colour
int const whiteThreshold = 200; //Don't set to low or confusion will occur in corrners/intimitant for to high detections.
int const greenThreshold = -15;
int const blackThreshold = -40;
int const errorLowerThreshold = -100; //Lower Threshold
int const errorUpperThreshold = 700; //Upper Threshold

int const onWhiteMinSpeed = 40;
int const onWhiteMaxSpeed = 100;

int const maxSpeedNearLine = 20;
//Distances from edge of line
int const speedLimitNearLineStart = 55;
int const speedLimitNearLineEnd = 5; //Max speed towards line at 60cm

boolean runningOnWhiteCode = false;
