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

/*typedef enum { G_TOWER = 0, G_TUBE = 1900, G_START = 1700} grabber_pos_t;
void grabber_turn(grabber_pos_t pos) { set_servo_position(GRABBER_TURN_SERVO, pos); msleep(400); }*/

#define ARM_SERVO 2
typedef enum { DOWN = 0, UP = 1700, DROP = 300, STOW = 1100 } arm_pos_t;
//drop = 910
void arm(arm_pos_t pos) { set_servo_position(ARM_SERVO, pos); }

#define CLAW_SERVO 1
typedef enum { OPEN = 1450, CLOSED = 270 } claw_pos_t;
void claw(claw_pos_t pos) { set_servo_position(CLAW_SERVO, pos); msleep(500); }

void jiggle() {
    puts("JIGGLE");
    int i;
    for (i = 0; i < 10; i++) {
        motor(0, 100); msleep(75);
        motor(0, -100); msleep(75);
    }
    mav(0, 0);
}


typedef enum { IN = 4070, OUT = 100, CUBE = 2000 } tube_pos; 
void tube(tube_pos pos) { 
    mav(0, analog(0) < pos ? 1000 : -1000);
    int x;
    do {
        x = analog(0) - pos;
        if (x < 0) x = -x;
    } while (x > 5);
    mav(0, 0);
}

//grab cube from towers
void grabby () {
    arm(UP);
    msleep(500);
    claw(CLOSED);
    msleep(500);
    tube(CUBE);
    msleep(300);
    arm(DROP);
    msleep(800);
    slow_servo(1, 1450, 1);
    jiggle();
    msleep(500);
    drive(-125, -125, 600);
}  

void wfl() {
    puts("waiting for light");
    int initial = analog(1);
    until (analog(1) < (initial/2)) msleep(100);
    puts("start!!");
}

void setup() {
    enable_servos();
    arm(DOWN);
    puts("turn on Create");
    create_connect();
    claw(OPEN);
    msleep(400);
}

#define FAST 600
#define SLOW 125
int main() {
    setup();
    wfl();
    shut_down_in(118);
    
    //drive to center line
    drive(SLOW, -SLOW, 350);
    drive(-FAST, -FAST, 1800);
    tube(OUT);
    msleep(660);
    arm(UP);
    //grabber_turn(G_TOWER);
    claw(OPEN);
    arm(STOW);
    msleep(500);
    move(-SLOW, -SLOW);
    until (get_create_lcliff_amt() < 2000);
    move(0, 0);
    msleep(100);
    //drive(SLOW, SLOW, 940);
    //drive(FAST, FAST, 288);
    
    //rotate to tower
    drive(-SLOW, SLOW, 950);
    //drive(-SLOW, SLOW, 1290);
    msleep(100);
    drive(-SLOW, -SLOW, 2400);
    drive(SLOW, SLOW, 800);
    arm(UP);
    msleep(300);
    claw(CLOSED);
    msleep(1000);
    
    //grab botgal and go to drop
    arm(STOW);
    msleep(500);
    drive(FAST, FAST, 650);
    msleep(500);
    drive(-SLOW, SLOW, 2100);
    msleep(500);
    arm(2047);
    slow_servo(1, 1450, 1);
    msleep(300);
    
    //come back and go to next tower
    drive(SLOW, -SLOW, 2120);
    msleep(100);
    arm(STOW);
    msleep(300);
    drive(-FAST, -FAST, 1600);
    msleep(100);
    drive(SLOW, SLOW, 900);
    drive(SLOW, -SLOW, 1180);
    tube(CUBE);
    drive(-SLOW, -SLOW, 2400);
    msleep(100);
    
    //get cube 1
    move(SLOW, SLOW);
    until (get_create_lfcliff_amt() < 2000);
    //drive(-SLOW, -SLOW, 2000);
    move(0, 0);
    msleep(200);
    //drive(SLOW, SLOW, 5200);
    drive(FAST, FAST, 800);
    drive(SLOW, SLOW, 900);
    msleep(300);
    drive(-SLOW, SLOW, 1250);
    arm(STOW);
    msleep(300);
    drive(-SLOW, -SLOW, 1500);
    drive(SLOW, SLOW, 900);
    arm(UP);
    msleep(500);
    tube(CUBE);
    grabby();
    msleep(100);
    drive(-SLOW, -SLOW, 800);
    move(0,0);
    msleep(9000);
    //drive(FAST, FAST, 200);
    //drive(-FAST, -FAST, 200);
    drive(SLOW, SLOW, 1400);
    
    //go to center pipe
    drive(-SLOW, SLOW, 1300);
    move(SLOW, SLOW);
    until (get_create_lfcliff_amt() < 2000);
    move(0, 0);
    msleep(200);
    
    //grab cube 2
    //drive(SLOW, SLOW, 4820);
    //drive(SLOW, SLOW, 3300);
    drive(FAST, FAST, 800);
    drive(SLOW, SLOW, 900);
    //grabber_turn(G_TOWER);
    msleep(300);
    drive(SLOW, -SLOW, 1250);
    arm(STOW);
    drive(-SLOW, -SLOW, 3000);
    drive(SLOW, SLOW, 700);
    msleep(300);
    arm(UP);
    drive(SLOW, SLOW, 100);
    tube(CUBE);
    grabby();
    //drive(FAST, FAST, 200);
    //drive(-FAST, -FAST, 200);
    
    /*//grab cube 3 (lower cube)
    drive(SLOW, SLOW, 600);
    msleep(300);
    drive(-SLOW, SLOW, 1270);
    drive(SLOW, SLOW, 2800);
    drive(SLOW, -SLOW, 1250);
    drive(-SLOW, -SLOW, 2900);
    drive(SLOW, SLOW, 1000);
    msleep(300);
    //grabber_turn(G_TOWER);
    arm(LOW_CUBES);
    drive(SLOW, SLOW, 100);
    claw(CLOSED);
    msleep(200);
    arm(UP);
    //set_servo_position(0, 490);
    msleep(500);
    drive(600, 600, 300);
    msleep(200);
    //grabber_turn(G_TUBE);
    tube(CUBE);
    arm(UP);
    msleep(300);
    tube(CUBE);
    arm(DROP);
    msleep(500);
    claw(OPEN);
    jiggle();
    arm(250);
    msleep(400);
    arm(UP);
    msleep(400);
    tube(CUBE);
    drive(-SLOW, -SLOW, 300);*/
    
    
    //go to analysis lab
    /*drive(SLOW, -SLOW, 2560);
    msleep(1000);
    drive(-FAST, -FAST, 800);
    drive(-SLOW, -SLOW, 1000);
    drive(SLOW, SLOW, 1000);
    drive(-SLOW, SLOW, 1250);
    drive(FAST, FAST, 1580);*/
    drive(SLOW, -SLOW, 1300);
    drive(FAST, FAST, 1100);
    
    
    //push out cubes
    tube(OUT);
    jiggle();
    
    
    return 0;
}
