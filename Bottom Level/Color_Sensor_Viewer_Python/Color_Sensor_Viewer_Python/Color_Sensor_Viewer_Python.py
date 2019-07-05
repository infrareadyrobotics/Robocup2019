import pygame
import serial
import math
import sys
import time
ser = serial.Serial('COM8',115200, timeout=4)
pygame.init()
size = (900,900)
screen = pygame.display.set_mode(size)
clock = pygame.time.Clock()
decodedData = [None] * 24
averageX = [None] * 3
averageY = [None] * 3
countAngle = [None] * 3
weight = [None] * 3
colorThreshold = [0,300,350]
comparison = [None] * 24


time.sleep(0.5)
while True:

    #Reset Averaging Variables:
    averageX[0] = 0
    averageX[1] = 0
    averageX[2] = 0
    averageY[0] = 0
    averageY[1] = 0
    averageY[2] = 0
    countAngle[0] = 0
    countAngle[1] = 0
    countAngle[2] = 0


    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            sys.exit() #exit program


    screen.fill((255, 255, 255)) # clear screen


    #code:
    rawData = ser.readline()#read serial data
    rawData = rawData.decode().strip().split(',')#split serial data into array
    print(rawData)
    rawData = list(map(int, rawData[0:24]))#convert serial data to int
    for sensorNum in range(24):
        #rawData[sensorNum] = int(rawData[sensorNum])
        pygame.draw.ellipse(screen, (rawData[sensorNum] / 4, rawData[sensorNum] / 4, rawData[sensorNum] / 4), [(250 * math.cos(math.radians(sensorNum * 15 - 90)) + 450 - 20), 250 * math.sin(math.radians(sensorNum * 15 - 90)) + 450 - 20, 40, 40])#draw circle with detected intensity as color
        if rawData[sensorNum] >= colorThreshold[2]: #if white
            decodedData[sensorNum] = 2 #white
            pygame.draw.ellipse(screen, (255, 255, 255), [(250 * math.cos(math.radians(sensorNum * 15 - 90)) + 450 - 10), 250 * math.sin(math.radians(sensorNum * 15 - 90)) + 450 - 10, 20, 20])
            weight[2] = ((rawData[sensorNum] - (colorThreshold[2] - 1)) / (max(rawData) - (colorThreshold[2] - 1)))
            averageX[2] += (250 * math.cos(math.radians(sensorNum * 15 - 90))) * weight[2]
            averageY[2] += (250 * math.sin(math.radians(sensorNum * 15 - 90))) * weight[2]
            countAngle[2] += 1;
        elif rawData[sensorNum] >= colorThreshold[1]: #if green
            decodedData[sensorNum] = 1 #green
            pygame.draw.ellipse(screen, (0, 255, 0), [(250 * math.cos(math.radians(sensorNum * 15 - 90)) + 450 - 10), 250 * math.sin(math.radians(sensorNum * 15 - 90)) + 450 - 10, 20, 20])
            #weight[1] = ((rawData[sensorNum] - 300) / (max(rawData) - 300))
            #averageX[1] += (250 * math.cos(math.radians(sensorNum * 15 - 90))) * weight[1]
            #averageY[1] += (250 * math.sin(math.radians(sensorNum * 15 - 90))) * weight[1]
            #countAngle[1] += 1;
        else:#if black
            comparison = rawData.copy()
            for sensorNum2 in range(24):
                if comparison[sensorNum2] >= colorThreshold[1]:
                    comparison[sensorNum2] = 0
            decodedData[sensorNum] = 0 #black
            pygame.draw.ellipse(screen, (0, 0, 0), [(250 * math.cos(math.radians(sensorNum * 15 - 90)) + 450 - 10), 250 * math.sin(math.radians(sensorNum * 15 - 90)) + 450 - 10, 20, 20])
            weight[0] = (((rawData[sensorNum] * -1) + (max(comparison))*2) / max(comparison))
            averageX[0] += (250 * math.cos(math.radians(sensorNum * 15 - 90))) * weight[0]
            averageY[0] += (250 * math.sin(math.radians(sensorNum * 15 - 90))) * weight[0]
            countAngle[0] += 1;

    if countAngle[2] > 0:
        averageX[2] = averageX[2] / countAngle[2]
        averageY[2] = averageY[2] / countAngle[2]
        pygame.draw.ellipse(screen, (200, 200, 200), [averageX[2] + 450 - 10, averageY[2] + 450 - 10, 20, 20])
    if countAngle[1] > 0:
        averageX[1] = averageX[1] / countAngle[1]
        averageY[1] = averageY[1] / countAngle[1]
        #pygame.draw.ellipse(screen, (0,200,0), [averageX[1]+450-10, averageY[1]+450-10,20,20])
    if countAngle[0] > 0:
        averageX[0] = averageX[0] / countAngle[0]
        averageY[0] = averageY[0] / countAngle[0]
        pygame.draw.ellipse(screen, (56, 56, 56), [averageX[0] + 450 - 10, averageY[0] + 450 - 10, 20, 20])
    pygame.display.flip()
    clock.tick(60)