import sensor
import image
import time
import math
import struct
from pyb import UART

ballThresholds = [(20, 80, 15, 97, 36, 77)]
goalThresholds = [(33, 53, -22, 0, 30, 60),(17, 22, -20, -9, -1, 14)]
#Blue goal (21, 30, -19, -13, -13, 1)
cropSize = 240
cropX = 14  #max 80
deg = 0
r = 0
x = 0
y = 0
uart = UART(1, 115200)
UARTRecieve = 0
bytesToSend = 0

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False,gain_db=14)
sensor.set_auto_whitebal(False, rgb_gain_db = (-8, -6, -3.4))
sensor.set_auto_exposure(False, 25000)
sensor.set_windowing((cropX,0,cropSize,cropSize))
sensor.set_hmirror(True);
clock = time.clock()

def sortStuff(stuff):
    return stuff[4]


while(True):
    #print(sensor.get_rgb_gain_db())
    #print(clock.fps())
    clock.tick()
    img = sensor.snapshot()
    blobs = []
    blobs2 = [[], [], []]
    for blob in img.find_blobs(ballThresholds, pixels_threshold=2, area_threshold=2, x_stride=2, y_stride=2):
        blobs2[0].append(blob)
    for blob in img.find_blobs(goalThresholds, pixels_threshold=20, area_threshold=20, x_stride=8, y_stride=8):
        if blob.code() == 1:
            blobs2[1].append(blob)
        else:
            blobs2[2].append(blob)

    blobs2[0].sort(key=sortStuff, reverse=True)
    blobs2[1].sort(key=sortStuff, reverse=True)
    blobs2[2].sort(key=sortStuff, reverse=True)
    for x in blobs2:
        count = 0
        for y in x:
            if count < 0:
                img.draw_rectangle(y.rect())
                img.draw_cross(y.cx(), y.cy())
            count += 1
    if (len(blobs2[0]) != 0):
        x = blobs2[0][0].cx()
        y = blobs2[0][0].cy()
        x -= cropSize/2.0
        y -= cropSize/2.0
        #print(x)
        #print(y)
        img.draw_cross(blobs2[0][0].cx(), blobs2[0][0].cy())
        img.draw_rectangle(blobs2[0][0].rect())
        deg = math.atan2(y, x)
        if(deg <= 0):
            deg += 2 * math.pi
        r = math.sqrt(x**2 + y**2)
        bytesToSend = struct.pack('f', deg * 180 / math.pi)

    #print("Degrees:" + str(deg * 180 / math.pi) + ", Radius: " + str(r))
    if (len(blobs2[1]) > 0) and (len(blobs2[2]) > 0):
        yx = blobs2[1][0].cx()
        yy = blobs2[1][0].cy()
        bx = blobs2[2][0].cx()
        by = blobs2[2][0].cy()
        img.draw_cross(blobs2[1][0].cx(), blobs2[1][0].cy())
        img.draw_rectangle(blobs2[1][0].rect())
        img.draw_cross(blobs2[2][0].cx(), blobs2[2][0].cy())
        img.draw_rectangle(blobs2[2][0].rect())
        ydeg = math.atan2(yy, yx)
        bdeg = math.atan2(by, bx)
        yr = math.sqrt(yx**2 + yy**2)
        br = math.sqrt(bx**2 + by**2)
        #print("num" + str((bdeg + ydeg)*180))
        #print("b" + str(bdeg*180))
        #print("y" + str(ydeg*180))

    #while uart.any() != 0:
        #UARTRecieve = str(uart.read(1), 'utf-8')
    #for a in range(3):
    if uart.any() != 0:
        UARTRecieve = str(uart.read(1), 'utf-8')
        uart.writechar(255)
        for a in range(3):
            if blobs2[a]:
                c=blobs2[a][0]
                img.draw_rectangle(c.rect())
                img.draw_cross(c.cx(), c.cy())
                x = c.cx()
                y = c.cy()
                x -= cropSize/2.0
                y -= cropSize/2.0
                # print(x)
                # print(y)
                deg = math.atan2(y, x)
                #if(deg <= 0):
                deg += math.pi
                deg *= 40
                r = math.sqrt(x**2 + y**2)
                uart.writechar(int(a))
                uart.writechar(int(deg))
                uart.writechar(int(r))
                uart.writechar(int(math.sqrt(c.pixels())))
                #print(uart.any())
                #print(int(a))
                #print(int(deg))
                #print(int(r))
                #print(int(math.sqrt(c.pixels())))
            else:
                uart.writechar(int(a))
                uart.writechar(int(0))
                uart.writechar(int(0))
                uart.writechar(int(0))
    while uart.any() != 0:
        uart.read(1)
