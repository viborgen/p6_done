import serial

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random

serialPort = serial.Serial(port = "COM7", baudrate=9600,bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

 
while(1):

    # Wait until there is data waiting in the serial buffer
    if(serialPort.in_waiting > 0):

        # Read data out of the buffer until a carraige return / new line is found
        serialString = serialPort.readline()

        # Print the contents of the serial data
        print(serialString.decode('Ascii'))