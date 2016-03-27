#!/usr/bin/env python

#Copyright (c) 2015, Josh Elsdon

#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#* Redistributions of source code must retain the above copyright notice, this
#  list of conditions and the following disclaimer.
#* Redistributions in binary form must reproduce the above copyright notice,
#  this list of conditions and the following disclaimer in the documentation
#  and/or other materials provided with the distribution.
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# this code will read from the mbed the Lepton thermal image. It will then display
# the image using a colour mapping, use right and left click on the image to change
# the min and the max values of the mapping.


import serial
import cv2
import sys
import numpy
from collections import namedtuple
import struct
import time


def unsigned(n):
    return n & 0xFFFFFFFF

min = 8050
max = 8650
xloc = 0
yloc = 0
newRclick = 0
newLclick = 0


def onMouse(event,x,y,flags,param):
    global newLclick
    global xloc
    global yloc
    global newRclick
    if event == 1:
        newLclick = 1
        xloc = x
        yloc = y
    if event == 2:
        newRclick = 1
        xloc = x
        yloc = y


def thermal():

    serdev = '/dev/ttyACM0' #change this if needed to your mbed serial dev.
    mbed = serial.Serial(serdev, 921600)


    size = 60, 80
    m = numpy.zeros(size, dtype=numpy.float32)
    count = 0;
    start = time.time()
    isSyncronised = 's'
    while (True):
        count = count+1
        if count%30 == 0:
            print(30/(time.time() - start),'fps')
            start = time.time()
        
        while (ord(isSyncronised) != 0xde):
            isSyncronised = mbed.read(1)

        while (ord(isSyncronised) != 0xad):
            isSyncronised = mbed.read(1)

        while (ord(isSyncronised) != 0xbe):
            isSyncronised = mbed.read(1)

        while (ord(isSyncronised) != 0xef):
            isSyncronised = mbed.read(1)

        packingStruct = struct.Struct('BBBB')
        unpackingStruct = struct.Struct('>i')

        for x in range(60):
            for y in range(80):
                a = mbed.read(1)
                b = mbed.read(1)
                m[x][y] = (ord(a)*255 + ord(b))
        global newLclick
        global xloc
        global yloc
        global newRclick
        global min
        global max
        mult = 8        #just a scaling to make things easier to see. set to 1 for native
        if( newLclick == 1):
            print( yloc),
            print( xloc)
            min = m[ yloc/mult][ xloc/mult]
            newLclick = 0
        if( newRclick == 1):
            max = m[ yloc/mult][ xloc/mult]
            newRclick = 0
        
        m = (m - min)/(max-min + 0.0)
        bigm = cv2.resize(m, (80*mult,60*mult))
        bigm = bigm * 255
        cvuint8 = cv2.convertScaleAbs(bigm)
        
        im_color = cv2.applyColorMap(cvuint8, cv2.COLORMAP_RAINBOW)
        cv2.imshow('Thermal Image', im_color)
        cv2.setMouseCallback('Thermal Image', onMouse, param=None)
        cv2.waitKey(1)
    

if __name__ == '__main__':
    thermal()
