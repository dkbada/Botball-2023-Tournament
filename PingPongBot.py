#!/usr/bin/python
from __future__ import print_function
import os, sys
import ctypes
KIPR = ctypes.CDLL("/usr/lib/libkipr.so")

# CONSTANTS -----------------------
WHEEL_L = 0
WHEEL_R = 1
    
LS_L = 1
LS_R = 2
BLACK = 1000

BOTTOM_SWITCH = 0
TOP_SWITCH = 1

# FUNCTIONS ------------------------

def move(l_power, r_power, sleep_time=False):
    KIPR.motor(WHEEL_L, l_power)
    KIPR.motor(WHEEL_R, r_power)
    KIPR.msleep(sleep_time or 5) 
    if sleep_time:
        KIPR.motor(WHEEL_L, 0)
        KIPR.motor(WHEEL_R, 0)
        
def stop(s_time=0):
    move(0, 0, s_time)

def ball_collector_arm(pos, wait=True):
    KIPR.set_servo_position(0, pos)
    if wait: KIPR.msleep(400)

def ball_pusher(pos, wait=True):
    KIPR.set_servo_position(1, 2047 * pos)
    if wait: KIPR.msleep(500)

def long_arm(pos, wait=True):
    KIPR.set_servo_position(2, pos)
    if wait: KIPR.msleep(400)
        
def switcher_finger(pos, wait=True):
    KIPR.set_servo_position(3, pos)
    if wait: KIPR.msleep(400)

def switch(which):
    return KIPR.digital(which)
            
def line_follow(aggression=70):
    if KIPR.analog(LS_L) > BLACK:
        move(aggression, 100)
    elif KIPR.analog(LS_R) > BLACK:
        move(100, aggression)
    else:
        move(90, 100)

def setup_servos():
    ball_pusher(0)
    ball_collector_arm(800, False)
    long_arm(1500, False)
    switcher_finger(0, False)
               
def wait_for_light():
    start_light = KIPR.analog(0) # Get ambient light
    while KIPR.analog(0) > start_light/2:
       stop(200)

def main():
    print("Setting up...")
    KIPR.enable_servos()
    setup_servos()
    move(-100, -100, 300)
    print("Done setting up.")
    print("Waiting for start light...")
    wait_for_light()
    start_time = KIPR.seconds()
    print("START!")
    while KIPR.analog(LS_R) < BLACK:
        move(100, 100)
    print("At line! Turning!")
    move(100, 100, 600)
    while KIPR.analog(LS_R) > BLACK:
        move(100, -100)
    while KIPR.analog(LS_R) < BLACK:
        move(100, -100)
    print("Following line to Wireshark...")
    while not switch(BOTTOM_SWITCH):
        line_follow()
    print("At Wireshark. Backing up...")
    move(-100, -90, 750)
    print("Dropping arm.")
    switcher_finger(800)
    long_arm(100)
    KIPR.msleep(200)
    switcher_finger(400)
    print("Pulling Wireshark out of way.")
    move(100, -100, 600)
    while KIPR.analog(LS_R) > BLACK:
        move(100, -100)
    while KIPR.analog(LS_R) < BLACK:
        move(50, -50)
    while KIPR.analog(LS_R) > BLACK:
        move(35, -35)
    stop(400)
    long_arm(1500)
    while KIPR.analog(LS_L) > BLACK:
        move(-100, 100)
    while KIPR.analog(LS_L) < BLACK:
        move(-100, 100)
    while KIPR.analog(LS_L) > BLACK:
        move(-100, 100)
    stop()
    print("Going up to switch light.")
    switcher_finger(1280)
    while not switch(BOTTOM_SWITCH):
        line_follow(40)
    print("Backing up.")
    move(-100, -75, 1000)
    print("Lightswitch!!")
    long_arm(400)
    switcher_finger(2047)
    long_arm(1500)
    print("Turning to re-grab Wireshark.")
    move(100, -100, 900)
    while KIPR.analog(LS_R) < BLACK:
        move(100, -100)
    while KIPR.analog(LS_R) > BLACK:
        move(100, -100)
    print("Got the Wireshark again!")
    print("Going down to the ping pong balls...")
    while not switch(TOP_SWITCH):
        line_follow()
    print("Back up...")
    move(-60, -100, 1000)
    print("Pulling out wireshark.")
    long_arm(0)
    KIPR.msleep(200)
    switcher_finger(1400)
    while KIPR.analog(LS_L) > BLACK:
    	move(-100, -10)
    while KIPR.analog(LS_L) < BLACK:
    	move(-100, -10)
    stop()
    long_arm(1600)
    print("Lining up under thingy pipe")
    move(-100, -100, 100)
    ball_collector_arm(2047)
    while KIPR.analog(LS_R) < BLACK:
    	move(0, -100)
    while KIPR.analog(LS_R) > BLACK:
    	move(0, -100)
    move(50, 50, 2700)
    print("Time at ball thingy:", (KIPR.seconds() - start_time) / 1000, "seconds")
    KIPR.msleep(3000)
    
    #while (KIPR.seconds() - start_time) < 65000:
    #    print("Waiting for 65 seconds...", (KIPR.seconds() - start_time))
    #    KIPR.msleep(100)
    
    for i in range(3):
        print("Drop %ith ball!" % (i + 1))
        ball_pusher(2047)
        ball_pusher(0)
    print("Turning to drop pink one in the wireshark.")
    switcher_finger(512)
    long_arm(0)
    move(-100, 0, 1000)
    ball_collector_arm(1800)
    ball_pusher(2047)
    ball_pusher(0)
    print("Pushing wireshark into analysis lab")
    move(-100, 100, 800)
    long_arm(1500)
    print("Time at end", (KIPR.seconds() - start_time) / 1000, "seconds")
    print("oogityboogityboo")

# --------------------------------------------------------------
if __name__ == "__main__":
    sys.stdout = sys.stderr = os.fdopen(sys.stdout.fileno(), "w", 0)
    main()
