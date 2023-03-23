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
FAST = 600

CLAW = 1
ARM = 0

CLAW_HOME = 850
ARM_HOME = 2000

CLAW_CLOSE = 2044    
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

def grabby(st=None):
    # Get Botgal
    K.set_servo_position(CLAW, CLAW_HOME)
    K.set_servo_position(ARM, ARM_UP)
    pause(900)
    K.set_servo_position(CLAW, CLAW_CLOSE)
    pause(700)
    K.set_servo_position(ARM, 1000)
    if st is not None:
        print("Got botgal at {} seconds".format((K.seconds() - st) / 1000.))
    K.msleep(400)
    K.set_servo_position(ARM, ARM_HOME)
    pause(1000)
            

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

    # Drive to middle line
    K.create_drive_direct(-FAST,-FAST)
    K.msleep(2000) #skip over first black line and knock over ring toy
    print("  Left Cliff Sensor = {0}".format(str(K.get_create_lcliff_amt())))
    # Look for Middle Line
    while K.get_create_lcliff_amt() > 2000: pass
    print("  Left Cliff Sensor = {0}".format(str(K.get_create_lcliff_amt())))
    pause(100)
      
    # Rotate 45d Counter-Clockwise     
    drive(-SLOW, SLOW, 900)
    K.set_servo_position(ARM, 1000)
    pause(100)  
           
    # Drive to Center Pipe
    drive(-FAST,-FAST,900)
    pause(100)  
                
    grabby(start_time)

    # Drive Straight to Drop Position
    drive(FAST,FAST,1200)
    pause(500)
    
    # Rotate 45d Clockwise
    drive(SLOW,-SLOW,900)
    pause(500)
    
    # Drop Botgal
    K.set_servo_position(CLAW, CLAW_HOME)            
    pause(500)
    
    drive(-SLOW,SLOW,900)
    pause(100)
    drive(-FAST, -FAST, 2000)
    pause(100)
    drive(SLOW,SLOW,500)
    #K.create_drive_direct(SLOW,-SLOW)
    #while K.get_create_lcliff_amt() < 2000: pass
    #while K.get_create_lcliff_amt() > 2000: pass
    drive(SLOW,-SLOW,1300)
    pause(100)
    drive(-SLOW, -SLOW, 2500)
    # Turn to line up with cube tower
    drive(-SLOW,SLOW,1300)
    grabby()
    
    #come back
    drive(SLOW, SLOW, 500)
    drive(SLOW, -SLOW, 750)
    drive(FAST, FAST, 1500)
    K.set_servo_position(CLAW, CLAW_HOME)
    pause(500)
        
    #go for second cube
    while K.get_create_lcliff_amt() > 2000:
		K.create_drive_direct(-FAST,-FAST)
    pause(300)
    while K.get_create_rfcliff_amt() > 2000:
		K.create_drive_direct(-FAST, FAST)
    drive(-FAST, -FAST, 1000)
    pause(100)
    drive(SLOW,SLOW,500)
    drive(SLOW,-SLOW,1300)
    pause(100)
    drive(SLOW, SLOW, 2500)
    # Turn to line up with cube tower
    drive(-SLOW,SLOW,1300)
    grabby()
    

    # Clean up
    K.ao()
    K.create_stop()
    K.disable_servos()
    K.create_disconnect()
    print("Tada")

if __name__== "__main__":
    sys.stdout = sys.stderr = os.fdopen(sys.stdout.fileno(),"w",0)
    main()
