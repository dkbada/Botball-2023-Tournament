#!/usr/bin/python
# Post 1010 BotGal Collector
from __future__ import print_function
import os, sys
import ctypes
K=ctypes.CDLL("/usr/lib/libkipr.so")

## CONSTANTS

BLACK = 1500
BACK_BLACK = 3700  
    
SLOW = 125
FAST = 400

CLAW = 1
ARM = 0

CLAW_HOME = 1000
ARM_HOME = 2000

CLAW_CLOSE = 2024    
ARM_UP = 0    

LS = 1

# KEEP THIS
test = False

## wrappers

# all off ( also K.create_stop() )
def stop():
    K.create_drive_direct(0, 0)

def pause(time=0):
    stop()
    if test:
        print("Press C to continue")
        while not K.c_button():
            K.msleep(50) 
    K.msleep(time)

## top level functions for this program
                

# function for using servo
def servo_control(servo, end_pos, tick_delay=1):
    pos = K.get_servo_position(servo)
    print(servo, pos, end_pos)
    if pos > end_pos:
        for i in range(pos, end_pos, -2):
            K.set_servo_position(servo, i)
            K.msleep(tick_delay)
    else:
        for i in range(pos, end_pos, 2):
            K.set_servo_position(servo, i)
            K.msleep(tick_delay)

# drive for n miliseconds
def drive(left_power, right_power, duration):
    K.create_drive_direct(int(left_power), int(right_power))
    K.msleep(duration)
    stop()                 


# wait for start light
def wfl():
    START = K.analog(LS)
    while(K.analog(LS) > START/2):
        K.msleep(50)

def main():

# Create Setup ==============================================================
    global test
    loop = False
    test = False
    print("Press A to continue")
    print("Press B to test")
    while not loop:
        if K.a_button():
            print("Continuing")
            loop = True
        elif K.b_button():
            test = True
            print("Testing")
            loop = True        
        
    print("Turn on Create")
    K.create_connect()
    print("Create Connected!")

    print("Walaby Battery = {0}".format(str(K.power_level())))
    print("Create Battery = {0}".format(str(K.get_create_battery_capacity())))
    K.enable_servos()
    print("Servos Enabled")
    K.set_servo_position(ARM,ARM_HOME)
    K.set_servo_position(CLAW,CLAW_HOME)
    
    # Wait for light start
    print("Wait for Start Light")
    wfl()
    start_time = K.seconds()

# Do stuff ===============================================

    #turn 
    drive(SLOW, -SLOW, 615)
    pause()
                
    # Back align with wall in start box
    #drive(-FAST,-FAST,500)
    #pause()

    # Drive to middle line
    lw = -FAST
    rw = -FAST
    K.create_drive_direct(lw,rw)
    K.msleep(1200) #skip over first black line and knock over ring toy
    K.set_servo_position(ARM, 1000)

                
    while ((lw > 0) & (rw > 0)):
        if ((K.get_create_lcliff_amt() < 2000) and (K.get_create_rcliff_amt() < 2000)):
            lw = 0
            rw = 0
    pause(100)         
                
    drive(-FAST, -FAST, 900)
    #drive(SLOW, -SLOW, 3150)
    drive(FAST, -FAST, 1305)
                
    K.create_drive_direct(-lw,-rw)
    K.msleep(1000) 

                
    while ((lw > 0) & (rw > 0)):
        if ((K.get_create_lcliff_amt() < 2000) and (K.get_create_rcliff_amt() < 2000)):
            lw = 0
            rw = 0
    pause(1000)  
    drive(-SLOW, -SLOW, 200)
                
    drive(FAST, -FAST, 500)
    while (K.get_create_lfcliff_amt() < 2000):
	    K.create_drive_direct(FAST, -FAST)
            
    drive(-FAST, -FAST, 1000)
    pause(1000)
            
    #get botgal
    K.set_servo_position(ARM, ARM_UP)
    pause(400)
    K.set_servo_position(CLAW, CLAW_CLOSE)
               
    

                

    # Rotate 90d to right (counterclockwise)
    #drive(SLOW,-SLOW,1230)
    #pause(3000)
    #while (K.get_create_lfcliff_amt() > 2000):
        #K.create_drive_direct(SLOW, -SLOW)
    #drive(-SLOW, SLOW, 100)
    #pause(300)             
    
    # Go back to botgal            
    #drive(-FAST, -FAST, 3000)
            
    # Get Botgal
    #drive(SLOW, SLOW, 300)
    #while (K.get_create_rfcliff_amt() < 2000):
        #K.create_drive_direct(-SLOW, SLOW)        
    #K.set_servo_position(ARM, ARM_UP)
    #pause(500)                   
    #K.set_servo_position(CLAW, CLAW_CLOSE)
    #pause(500)        
    #K.set_servo_position(ARM, ARM_HOME) 
    #pause(300)        
                
    # Turn and move to analysis lab
    #drive(SLOW, -SLOW, 500)
    #drive(FAST, FAST, 1500)
    #pause(300)
    
    # Release Botgal
    #K.set_servo_position(CLAW, CLAW_HOME)            
                
    # Clean up
    K.ao()
    K.create_stop()
    K.disable_servos()
    K.create_disconnect()
    print("Tada")

if __name__== "__main__":
    sys.stdout = sys.stderr = os.fdopen(sys.stdout.fileno(),"w",0)
    main()
