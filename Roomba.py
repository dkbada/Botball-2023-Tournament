#!/usr/bin/python
# Post 1010 BotGal Collector
import os, sys
import ctypes
K=ctypes.CDLL("/usr/lib/libkipr.so")

## CONSTANTS

TRUE=1

SLOW = 125
FAST = 250

CLAW = 3
ARM = 0

CLAW_HOME = 1024
ARM_HOME = 1024

LS = 1

## wrappers

# all off ( also K.create_stop() )
def stop():
    K.create_drive_direct(0,0)

def pause(time,test):
    K.create_drive_direct(0,0)
    if test==1:
        print "Press C to continue"  
        while(K.c_button() == 0):
            K.msleep(50) 
    K.msleep(time)

## top level functions for this program

# function for using servo
def servo_control(servo_name, end_pos, rate=1):
    pos = K.get_servo_position(servo_name)
    print(servo_name, pos, end_pos)
    if pos > end_pos:
        for i in range(pos, end_pos, -rate):
            K.set_servo_position(servo_name, i)
            K.msleep(rate)
    else:
        for i in range(pos, end_pos, rate):
            K.set_servo_position(servo_name, i)
            K.msleep(rate)

# drive for n miliseconds
def drive(left_power, right_power, duration):
    K.create_drive_direct(left_power, right_power)
    K.msleep(duration)
    stop()  

# wait for start light
def wfl():
    START = K.analog(LS)
    while(K.analog(LS) > START/2):
        K.msleep(50)

def main():

# Create Setup ==============================================================

    loop = 0
    test = 0
    print "Press A to continue"
    print "Press B to test"
    while(loop == 0):
        if(K.a_button() == 1):
            test = 0
            print "Continuing"
            loop = 1
        elif(K.b_button() == 1):
            test = 1
            print "Testing"
            loop = 1        
        
    print("Turn on Create")
    K.create_connect()
    print("Create Connected!")

    print("Walaby Battery = {0}".format(str(K.power_level())))
    print("Create Battery = {0}".format(str(K.get_create_battery_capacity())))
        
    K.set_servo_position(ARM,ARM_HOME)
    K.set_servo_position(CLAW,CLAW_HOME)
    K.enable_servos()
    print("Servos Enabled")
    K.set_servo_position(ARM,ARM_HOME)
    K.set_servo_position(CLAW,CLAW_HOME)
    
    # Wait for light start
    print("Wait for Start Light")
    wfl()

    K.shut_down_in(118)

# Create Mission ===============================================

    # Back align with wall in start box
    drive(-SLOW,-SLOW,1000)
    pause(300,test)

    # Drive to middle line
    lw = SLOW
    rw = SLOW
    K.create_drive_direct(lw,rw)
    K.msleep(2000) #skip over first black line and knock over ring toy

    while ((lw > 0) & (rw > 0)):
        if (K.get_create_lcliff_amt() < 2000):
            lw = 0
        if (K.get_create_rcliff_amt() < 2000):
            rw = 0
        K.create_drive_direct(lw,rw)
    pause(300,test)
    
    # Rotate 90d to right (clockwise)
    drive(SLOW,-SLOW,1100)
    pause(300,test)        
    
    # Drive to middle pipe
    drive(-SLOW,-SLOW,5600)
    pause(300,test)
    
    # Grab BotGal
    print("Grab BotGal")
    
    # Drive to middle of board
    drive(SLOW,SLOW,1200)
    pause(300,test)
    
    # Rotate 135d to left (counter clockwise)
    drive(-SLOW,SLOW,2000)
    pause(300,test)

    # Drop BotGal
    print("Drop BotGal")
                
    # Clean up
    K.ao()
    K.create_stop()
    K.disable_servos()
    K.create_disconnect()
    print("Game Over")
    K.msleep(2000)

if __name__== "__main__":
    sys.stdout = os.fdopen(sys.stdout.fileno(),"w",0)
    main();
