# -*- coding: utf-8 -*-
from subprocess import call
import shlex
import serial
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS,

)
 
##Infinity loop that waits for MCU order to record

while True:
    if ser.read(1) == 'CAMREC':
            print 'Exit code: ', call('mencoder tv:// -tv driver=v4l:width=320:height=24)0:device=/dev/video1:forceaudio:adevice=/dev/dsp2 -ovc lavc -oac mp3lame -lameopts cbr:br=64:mode=3 -o webcam.avi')
    elif ser.read(1) == 'CAMSTOP':
            print 'Exit code: ', call('killall mplayer')