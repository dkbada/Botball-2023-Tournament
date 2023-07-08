#include <kipr/wombat.h>

typedef int bool;
#define true 1
#define false 0
#define until(x) while (!(x))
#define puts(x) printf(x "\n")
#define clamp(x, lo, hi) (x < lo ? lo : x > hi ? hi : x)

#define move(l_power, r_power) create_drive_direct(l_power, r_power)
#define stop() move(0, 0)

void drive(int l_power, int r_power, int sleep_time) {
    move(l_power, r_power);
    msleep(sleep_time);
    move(0, 0);
}

void slow_servo(int which, int to, int delay) {
    int from = get_servo_position(which), i;
    if (from < to) {
        for (i = from; i < to; i++) {
            set_servo_position(which, i);
            msleep(delay);
        }
    } else {
        for (i = from; i > to; i--) {
            set_servo_position(which, i);
            msleep(delay);
        }
    }
}

#define GRABBER_TURN_SERVO 0
typedef enum { G_TOWER = 0, G_TUBE = 2047, G_START = 1552} grabber_pos_t;
void grabber_turn(grabber_pos_t pos) { set_servo_position(GRABBER_TURN_SERVO, pos); msleep(400); }

#define ARM_SERVO 3
typedef enum { DOWN = 652, UP = 1691, BOTGAL_LOW_CUBES = 2047, DROP = 746, STOW = 1320 } arm_pos_t;
void arm(arm_pos_t pos) { slow_servo(ARM_SERVO, pos, 1); }

#define CLAW_SERVO 2
typedef enum { OPEN = 1350, CLOSED = 0 } claw_pos_t;
void claw(claw_pos_t pos) { set_servo_position(CLAW_SERVO, pos); msleep(500); }

//grab cube from towers
void grabby () {
    arm(UP);
    claw(CLOSED);
    arm(DROP);
    grabber_turn(G_TUBE);
    claw(OPEN);
}  

#define TUBE_SENSOR 0
typedef enum { IN = 1600 } tube_pos; 
void tube_in () { 
    while (analog(1) < IN) {
        motor(0, 70);
        msleep(50);
    }
    ao();
}

void tube_out () {
    clear_motor_position_counter(0);
    while (get_motor_position_counter(0) > -500) {
        motor(0, -50);
        msleep(100);
    }
    ao();
}    

void wfl() {
    puts("waiting for light");
    int initial = analog(0);
    until (analog(0) < (initial >> 1)) msleep(100);
    puts("start!!");
}

void setup() {
    enable_servos();
    puts("turn on Create");
    create_connect();
    arm(UP);
    grabber_turn(G_START);
    claw(CLOSED);
    msleep(400);
    arm(DOWN);
}

#define FAST 600
#define SLOW 125
int main() {
    setup();
    //wfl();
    msleep(5000);
    shut_down_in(118);
    
    //drive to center line
    drive(SLOW, -SLOW, 280);
    set_servo_position(3, 1691); msleep(300);
    move(-FAST, -FAST);
    tube_in();
    grabber_turn(G_TOWER);
    claw(OPEN);
    msleep(900);
    while (get_create_lcliff_amt() > 2000) {
        move(-FAST, -FAST);
    }
    move(0, 0);
    msleep(100);
    drive(SLOW, SLOW, 200);
    
    //rotate 45 deg
    drive(-SLOW, SLOW, 950);
    msleep(100);
    drive(-SLOW, -SLOW, 800);
    msleep(100);
    
    //grab botgal and go to drop
    claw(CLOSED);
    msleep(200);
    arm(DROP);
    //might have to wait for other bot
    drive(FAST, FAST, 950);
    msleep(500);
    drive(-SLOW, SLOW, 1800);
    msleep(500);
    arm(UP);
    claw(OPEN);
    msleep(300);
    
    //come back and go to next tower
    drive(SLOW, -SLOW, 1900);
    msleep(100);
    arm(STOW);
    drive(-FAST, -FAST, 1960);
    msleep(100);
    drive(SLOW, SLOW, 500);
    drive(SLOW, -SLOW, 1250);
    msleep(100);
    
    //get cube 1
    while (get_create_lfcliff_amt() > 2000) {
    	move(-SLOW, -SLOW);
    }
    //drive(-SLOW, -SLOW, 2000);
    drive(-SLOW, -SLOW, 1500);
    msleep(300);
    drive(-SLOW, SLOW, 1250);
    drive(-SLOW, -SLOW, 1000);
    msleep(1500);
    drive(SLOW, SLOW, 300);
    grabby();
    
    /*//go to center pipe
    drive(SLOW, SLOW, 500);
    drive(SLOW, -SLOW, 1250);
    while (get_create_lfcliff_amt() > 2000) {
        move(SLOW, SLOW);
    }
    
    //grab cube 2
    drive(SLOW, SLOW, 2000);
    msleep(300);
    drive(-SLOW, SLOW, 1250);
    drive(-SLOW, -SLOW, 1000);
    msleep(300);
    drive(SLOW, SLOW, 300);
    grabby();
    
    //grab cube 3 (lower cube)
    
    
    // back to analysis lab
    
    
    //noodles
    
    
    //push out cubes*/
    
    return 0;
}

