# Multi Color Blob Tracking Example
#
# This example shows off multi color blob tracking using the OpenMV Cam.

import sensor, image, time, math, struct
from pyb import UART

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green things. You may wish to tune them...
thresholds = [(30, 60, 45, 80, 25, 60),(35,50,-5,20,20,45),(21, 31, -14, -7, -14, -3)]
# You may pass up to 16 thresholds above. However, it's not really possible to segment any
# scene with 16 thresholds before color thresholds start to overlap heavily.
width = 240
height = 240
deg = 0
r = 0
x = 0
y = 0
uart = UART(1, 115200)
UARTRecieve = 0
bytesToSend = 0;

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False,gain_db=10) # must be turned off for color tracking
#sensor.set_auto_whitebal(False, rgb_gain_db = (0, 0, 0))
sensor.set_auto_exposure(False, 10000)
#sensor.set_hmirror(True);

#sensor.set_saturation(1)
sensor.set_windowing((60,0,width,height))
clock = time.clock()

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. Don't set "merge=True" becuase that will merge blobs which we don't want here.

def sortStuff(stuff):
    return stuff[4]


while(True):
    print(clock.fps())
    clock.tick()
    img = sensor.snapshot()
    blobs = []
    blobs2 = [[],[],[]]
    for blob in img.find_blobs(thresholds, pixels_threshold=7, area_threshold=8,x_stride=3,y_stride=3):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        blobs.append(blob)
        #blobs.sort(key=sortStuff, reverse=True)
        blobs2[0] = list(filter(lambda x: x[8]==1,blobs))
        blobs2[1] = list(filter(lambda x: x[8]==2,blobs))
        blobs2[2] = list(filter(lambda x: x[8]==4,blobs))
    if (len(blobs) != 0):
        x = blobs[0].cx()
        y = blobs[0].cy()
        x -= width/2.0
        y -= height/2.0
        #print(x)
        #print(y)
        deg = math.atan2(y,x)
        if(deg <= 0):
            deg += 2 * math.pi
        r = math.sqrt(x**2 + y**2)
        bytesToSend = struct.pack('f', deg * 180 / math.pi)

    #print("Degrees:" + str(deg * 180 / math.pi) + ", Radius: " + str(r))

    #if uart.any() > 0:
        #print("DATA INCOMING")
    if (len(blobs2[1]) > 0) and (len(blobs2[2]) > 0):
        yx = blobs2[1][0].cx()
        yy = blobs2[1][0].cy()
        bx = blobs2[2][0].cx()
        by = blobs2[2][0].cy()

        ydeg = math.atan2(yy,yx)
        bdeg = math.atan2(by,bx)
        yr = math.sqrt(yx**2 + yy**2)
        br = math.sqrt(bx**2 + by**2)
        print("num" + str((bdeg + ydeg)/2))

    while uart.any() != 0:
        UARTRecieve = str(uart.read(1), 'utf-8')
        #print (UARTRecieve)
    if(UARTRecieve == "x"):
        UARTRecieve = ''
        #print("Codes Match");
        for b in bytesToSend:
            #uart.write(chr(int(b,2)))
            #print(b)
            uart.writechar(b)
