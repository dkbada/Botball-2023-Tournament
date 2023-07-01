#include <kipr/wombat.h>

typedef int bool;
#define true 1
#define false 0
#define until(x) while (!(x))
#define puts(x) printf(x "\n")
#define clamp(x, lo, hi) (x < lo ? lo : x > hi ? hi : x)

#define move(l_power, r_power) create_drive_direct(l_power, r_power)
#define stop() move(0, 0)

void drive_for(int l_power, int r_power, int sleep_time) {
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
typedef enum { DOWN = 0, UP = 1050, BOTGAL_LOW_CUBES = 2047 } arm_pos_t;
void arm(arm_pos_t pos) { slow_servo(ARM_SERVO, pos, 1); }

#define TUBE_PUSH_SERVO 2
typedef enum { OUT = 1490, START = 2047, IN = 1600 } tube_pos_t;
void tube_push(tube_pos_t pos) { slow_servo(TUBE_PUSH_SERVO, pos, 1); }

#define CLAW_PSEUDO_SERVO 0
#define CLAW_ENCODER 5
thread pst;
int target_position = 4070;
#define Kp 2
void _control_claw() {
    while (true) {
        int power = Kp * (analog(CLAW_ENCODER) - target_position);
        power = clamp(power, -1500, 1500);
        mav(CLAW_PSEUDO_SERVO, power);
    }
}
void start_controlling_claw() {
    pst = thread_create(_control_claw);
    thread_start(pst);
}
void done_controlling_claw() {
    thread_destroy(pst);
}
typedef enum { OPEN = 4080, CLOSED = 200 } claw_pos_t;
void claw(claw_pos_t pos) { target_position = pos; msleep(500); }

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
    tube_push(OUT);
    arm(UP);
    start_controlling_claw();
    claw(CLOSED);
    arm(DOWN);
}

int main() {
    setup();
    //wfl();
    shut_down_in(118);
    arm(UP);
	int i;
    for (i = 0; i < 4; i++) {
        msleep(1000);
        claw(CLOSED);
        msleep(1000);
        claw(OPEN);
    }
	done_controlling_claw();
    return 0;
}
