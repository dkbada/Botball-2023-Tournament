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
typedef enum { G_TOWER = 0, G_TUBE = 2047 } grabber_pos_t;
void grabber_turn(grabber_pos_t pos) { set_servo_position(GRABBER_TURN_SERVO, pos); msleep(400); }

#define ARM_SERVO 3
typedef enum { DOWN = 0, UP = 1050, BOTGAL_LOW_CUBES = 2047, DROP = 746 } arm_pos_t;
void arm(arm_pos_t pos) { slow_servo(ARM_SERVO, pos, 1); }

#define CLAW_SERVO 2
typedef enum { OPEN = 1024, CLOSED = 0 } claw_pos_t;
void claw(claw_pos_t pos) { set_servo_position(CLAW_SERVO, pos); msleep(500); }

//grab cube from towers
void grabby () {
    claw(CLOSED);
    arm(DROP);
    grabber_turn(G_TUBE);
    claw(OPEN);
        

void wfl() {
    puts("waiting for light");
    int initial = analog(0);
    until (analog(0) < (initial >> 1)) msleep(100);
    puts("start!!");
}

void setup() {
    enable_servos();
    puts("turn on Create");
    //create_connect();
    //tube_push(OUT);
    arm(UP);
    claw(CLOSED);
    arm(DOWN);
}

#define FAST 600
#define SLOW 125
int main() {
    setup();
    //wfl();
    shut_down_in(118);
    //tube push 
    
    //drive to center line
    move(-FAST, -FAST);
    msleep(2000);
    while (get_create_lcliff_amt() > 2000) {
        move(-FAST, -FAST);
    }
    arm(UP);
    grabber_turn(G_TOWER);
    claw(OPEN);
    msleep(100); 
    
    //rotate 45 deg
    drive(-SLOW, SLOW, 900);
    msleep(100);
    drive(-FAST, -FAST, 900);
    msleep(100);
    
    //grab botgal and go to drop
    claw(CLOSED);
    arm(DROP);
    //might have to wait for other bot
    drive(FAST, FAST, 950);
    msleep(500);
    drive(SLOW, -SLOW, 550);
    msleep(500);
    arm(UP);
    claw(OPEN);
    msleep(500);
    
    //come back and go to next tower
    drive(-SLOW, SLOW, 600);
    msleep(100);
    drive(-FAST, -FAST, 1960);
    msleep(100);
    drive(SLOW, SLOW, 500);
    drive(SLOW, -SLOW, 1250);
    msleep(100);
    
    //get cube 1
    while (get_create_lfcliff_amt() > 2000) {
    	move(-SLOW, -SLOW);
    }
    drive(-SLOW, -SLOW, 2000);
    msleep(300);
    drive(-SLOW, SLOW, 1250);
    drive(-SLOW, -SLOW, 1000);
    msleep(1500);
    drive(SLOW, SLOW, 300);
    grabby();
    
    //go to center pipe
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
    
    
    //push out cubes
        
    }
    return 0;
}
