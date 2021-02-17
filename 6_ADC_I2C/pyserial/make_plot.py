import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial

#initialize serial port

ser = serial.Serial()
# ser.port = '/dev/ttyACM0' #Arduino serial port
ser.port = 'COM8'
ser.baudrate = 115200
# ser.timeout = 10 #specify timeout when using readline()
ser.timeout = 10 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
# xs = [] #store trials here (n)
# ys = [] #store relative frequency here
# rs = [] #for theoretical probability

time = []
red = []
ir = []

# This function is called periodically from FuncAnimation
def animate(i, xs, ys):
    
    #Aquire and parse data from serial port
    line=ser.readline()      #ascii
    line = line.decode('utf-8')
    # print('----------------')    
    line_as_list = line.split(',')     
    # print(line_as_list)       
    # print('spl',line_as_list[0], line_as_list[1])

    # i = int(line_as_list[0])
    # relProb = line_as_list[1]
    # relProb_as_list = relProb.split(b'\n')
    # relProb_float = float(relProb_as_list[0])
    
    ir_line = line_as_list[1].split('\r\n')    
    if not time:
        time.append(0)
    else:
        time.append(time[-1]+1)
    red.append(int(line_as_list[0]))
    ir.append(int(ir_line[0]))    
    # print(red[-3:], ir[-3:])	

    # Limit x and y lists to 20 items
    #xs = xs[-20:]
    #ys = ys[-20:]
    # time = time[-20:]
    # ir = ir[-20:]
    # red = red[-20:]



    # Draw x and y lists
    ax.clear()
    # ax.plot(xs, ys, label="Ir")
    # ax.plot(xs, rs, label="Red")
    ax.plot(time, ir, label="Ir")
    ax.plot(time, red, label="Red")

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('ppg sensor value')
    plt.ylabel('value')
    plt.legend()
    # plt.axis([1, None, 0, 1.1]) #Use for arbitrary number of trials    
    #plt.axis([1, 100, 0, 1.1]) #Use for 100 trial demo

# Set up plot to call animate() function periodically
# ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=1000)
ani = animation.FuncAnimation(fig, animate, fargs=(time, red), interval=100)
plt.show()