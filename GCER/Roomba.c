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

#define GRABBER_TURN_SERVO 1
typedef enum { G_TOWER = 61, G_TUBE = 1853, G_START = 1552} grabber_pos_t;
void grabber_turn(grabber_pos_t pos) { set_servo_position(GRABBER_TURN_SERVO, pos); msleep(400); }

#define ARM_SERVO 0
typedef enum { DOWN = 0, UP = 1150, CUBE = 1240, BOTGAL_LOW_CUBES = 2047, DROP = 850, STOW = 490 } arm_pos_t;
void arm(arm_pos_t pos) { slow_servo(ARM_SERVO, pos, 1); }

#define CLAW_SERVO 2
typedef enum { OPEN = 1350, CLOSED = 0 } claw_pos_t;
void claw(claw_pos_t pos) { set_servo_position(CLAW_SERVO, pos); msleep(500); }

//grab cube from towers
void grabby () {
    arm(CUBE);
    claw(CLOSED);
    msleep(200);
    arm(STOW);
    msleep(500);
    drive(600, 600, 700);
    msleep(200);
    arm(UP);
    msleep(200);
    grabber_turn(G_TOWER);
    msleep(200);
    arm(DROP);
    msleep(1500);
    claw(OPEN);
}  

#define abs(x) ({int z___ = (x); z___ < 0 ? -z___ : z___; })
typedef enum { IN = 4000, OUT = 50, START = 1520 } tube_pos; 
void tube(tube_pos pos) { 
    mav(0, analog(0) < pos ? 70 : -70);
    int x;
    do {
        x = analog(0) - pos;
        if (x < 0) x = -x;
    } while (x > 5);
    mav(0, 0);
}

void wfl() {
    puts("waiting for light");
    int initial = analog(1);
    until (analog(1) < (initial >> 1)) msleep(100);
    puts("start!!");
}

void setup() {
    enable_servos();
    tube(START);
    puts("turn on Create");
    create_connect();
    arm(UP);
    grabber_turn(G_START);
    claw(CLOSED);
    msleep(400);
    arm(DOWN);
    tube(OUT);
}

#define FAST 600
#define SLOW 125
int main() {
    setup();
    wfl();
    //msleep(5000);
    shut_down_in(118);
    
    //drive to center line
    //drive(SLOW, -SLOW, 295);
    move(-410, -FAST);
    arm(UP);
    grabber_turn(G_TOWER);
    msleep(500);
    move(-FAST, -FAST);
    until (get_create_lcliff_amt() < 2000);
    move(0, 0);
    msleep(200);
    drive(SLOW, SLOW, 940);
    
    //rotate 45 deg
    claw(OPEN);
    //drive(-SLOW, SLOW, 950);
    drive(-SLOW, SLOW, 1290);
    msleep(100);
    drive(-SLOW, -SLOW, 1500);
    claw(CLOSED);
    
    //grab botgal and go to drop
    //set_servo_position(0, 490);
    arm(STOW);
    //might have to wait for other bot
    msleep(500);
    drive(FAST, FAST, 950);
    msleep(500);
    drive(-SLOW, SLOW, 2400);
    msleep(500);
    arm(UP);
    claw(OPEN);
    msleep(300);
    
    //come back and go to next tower
    drive(SLOW, -SLOW, 2500);
    msleep(100);
    arm(STOW);
    drive(-FAST, -FAST, 1960);
    msleep(100);
    drive(SLOW, SLOW, 900);
    drive(SLOW, -SLOW, 1250);
    msleep(100);
    tube(IN);
    
    //get cube 1
    move(-SLOW, -SLOW);
    until (get_create_lfcliff_amt() < 2000);
    //drive(-SLOW, -SLOW, 2000);
    drive(-SLOW, -SLOW, 1500);
    msleep(300);
    drive(-SLOW, SLOW, 1250);
    drive(-SLOW, -SLOW, 1000);
    msleep(500);
    drive(SLOW, SLOW, 300);
    grabby();
    
    /*//go to center pipe
    drive(SLOW, SLOW, 500);
    drive(SLOW, -SLOW, 1250);
    move(SLOW, SLOW);
    until (get_create_lfcliff_amt() < 2000);
    
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

