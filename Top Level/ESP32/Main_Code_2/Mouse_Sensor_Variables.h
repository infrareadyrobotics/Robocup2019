//Mouse Sensor Variables

//SPI defines
#define PIN_SCLK   SCK
#define PIN_MISO   MISO
#define PIN_MOSI   MOSI
#define PIN_NCS    5


// Define all the registers
#define PROD_ID                          0x00
#define REV_ID                           0x01
#define MOTION_ST                        0x02
#define DELTA_X                          0x03
#define DELTA_Y                          0x04
#define SQUAL                            0x05
#define SHUT_HI                          0x06
#define SHUT_LO                          0x07
#define PIX_MAX                          0x08
#define PIX_ACCUM                        0x09
#define PIX_MIN                          0x0a
#define PIX_GRAB                         0x0b
#define MOUSE_CTRL                       0x0d
#define RUN_DOWNSHIFT                    0x0e
#define REST1_PERIOD                     0x0f
#define REST1_DOWNSHIFT                  0x10
#define REST2_PERIOD                     0x11
#define REST2_DOWNSHIFT                  0x12
#define REST3_PERIOD                     0x13
#define PREFLASH_RUN                     0x14
#define PREFLASH_RUN_DARK                0x18
#define MOTION_EXT                       0x1b
#define SHUT_THR                         0x1c
#define SQUAL_THRESHOLD                  0x1d
#define NAV_CTRL2                        0x22
#define MISC_SETTINGS                    0x25
#define RESOLUTION                       0x33
#define LED_PRECHARGE                    0x34
#define FRAME_IDLE                       0x35
#define RESET                            0x3a
#define INV_REV_ID                       0x3f
#define LED_CTRL                         0x40
#define MOTION_CTRL                      0x41
#define AUTO_LED_CONTROL                 0x43
#define REST_MODE_CONFIG                 0x45

//SPI speed
static const int spiClk = 500000;

//Mouse predicated position in reads (DPI = 250 around 98.4252 DPC)
float mouseXpos = 0;
float mouseYpos = 0;

//Mouse movement
int mouseX = 0;
int mouseY = 0;

float DPC = 98.4252; //dots per centimeter (250 DPI due to only 8 bit register)

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
SPIClass * hspi = NULL;
