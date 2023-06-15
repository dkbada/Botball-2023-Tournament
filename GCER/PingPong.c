#include <kipr/wombat.h>

typedef int bool;
#define true 1
#define false 0
#define until(x) while (!(x))
#define puts(x) printf(x "\n")

#define L_MOTOR 0
#define R_MOTOR 1
void move(int l_power, int r_power) {
    motor(L_MOTOR, l_power);
    motor(R_MOTOR, r_power);
}
#define stop() move(0, 0)

void drive_for(int l_power, int r_power, int sleep_time) {
    move(l_power, r_power);
    msleep(sleep_time);
    move(0, 0);
}

void slow_servo(int which, int to) {
    int from = get_servo_position(which), i;
    if (from < to) {
        for (i = from; i < to; i++) {
            set_servo_position(which, i);
            msleep(1);
        }
    } else {
        for (i = from; i > to; i--) {
            set_servo_position(which, i);
            msleep(1);
        }
    }
}

// Line Following sensors and constants
#define LS_R 2
#define LS_L 1
#define on_black(ls) (analog(ls) > BLACK)
#define BLACK 1500
void line_follow(int aggression) {
    if (on_black(LS_L)) move(aggression, 100);
    else if (on_black(LS_R)) move(100, aggression);
    else move(90, 100);
}
#define line_follow_until(cond, aggression) do { \
    line_follow(aggression); \
    printf("line follow until " #cond "\n"); \
    msleep(100); \
} while (!(cond))

typedef enum { LEFT, RIGHT } edge_t;
void line_follow_one_sided(int aggression, int sensor, edge_t edge) {
    if (on_black(sensor) ^ (edge == LEFT)) move(100, aggression);
    else move(aggression, 100);
}
#define line_follow_one_sided_until(cond, side, edge, aggression) do { \
    line_follow_one_sided(aggression, side, edge); \
    printf("line follow one sided until " #cond "\n"); \
    msleep(100); \
} while (!(cond))

// Line Counter Sensor
#define LC_BLACK 2700
bool lc_on_black(int threshold) {
    return analog(3) > threshold;
}

// Servos
#define DOOR_SERVO 1
typedef enum { D_CLOSED = 600, D_OPEN = 0 } door_pos_t;
void ball_door(door_pos_t pos) { set_servo_position(DOOR_SERVO, pos); msleep(400); }

#define ARM_SERVO 0
typedef enum { FLOOR = 2047, DOWN = 1970, WIRESHARK = 1860, UP = 1300, OUT_OF_WAY = 1500 } tube_arm_pos_t;
void tube_arm(tube_arm_pos_t pos) { slow_servo(ARM_SERVO, pos); }

#define GRABBER_SERVO 2
typedef enum { G_OPEN = 0, G_CLOSED = 1160 } grabber_pos_t;
void grabber(grabber_pos_t pos) { slow_servo(GRABBER_SERVO, pos); }

void jiggle() {
    int i;
    for (i = 0; i < 20; i++) {
        puts("jiggle jiggle jiggle");
        move(100, -100);
        msleep(25);
        move(-100, -100);
        msleep(25);
        move(-100, 100);
        msleep(25);
        move(100, 100);
        msleep(25);
    }
    move(0, 0);
}

// Switch sensors
bool wireshark_switch() { return digital(9); }
bool tube_switch() { return digital(8); }

void sleep_until_balls_drop(double start) {
    puts("sleeping until balls");
    while (seconds() - start < 45) { ////////////////////////////////////////////////////////FIXME//////////
        printf("time: %lg seconds\n", seconds() - start);
        msleep(100);
    }
    puts("at 61 seconds");
}

void setup() {
    enable_servos();
    ball_door(D_CLOSED);
    tube_arm(UP);
    grabber(G_OPEN);
    drive_for(-100, -100, 300);
}

void wfl() {
    puts("waiting for light");
    int initial = analog(0);
    until (analog(0) < (initial >> 1)) msleep(100);
    puts("start!!");
}

int main() {
    setup();
    wfl();
    double start = seconds();
    //    shut_down_in(118.);
    puts("Going to line");
    until (on_black(LS_L) && on_black(LS_R)) move(100, 100);
    drive_for(100, 100, 1000);
    puts("turning");
    until (on_black(LS_R)) move(50, -50);
    // Go to wireshark
    line_follow_until(wireshark_switch(), 70);
    puts("at wireshark");
    drive_for(-100, -100, 200);
    puts("grabbing");
    move(20, 0);
    grabber(G_CLOSED);
    // Turn around
    drive_for(50, -50, 500);
    puts("turning around with wireshark");
    until (on_black(LS_R)) move(50, -50);
    until (on_black(LS_L)) move(50, -50);
    drive_for(0, 100, 500);
    // Linefollow down, counting the right side lines
    puts("linefollowing down");
    int lines = 0;
    bool on_black = false;
    while (lines < 2) {
        line_follow(70);
        if (lc_on_black(LC_BLACK + (on_black ? -20 : 20)) != on_black) { // Implement some hysteresis
            if (on_black) {
                lines++;
                printf("line number %i\n", lines);
            }
            on_black = !on_black;
        }
    }
    // Jump out
    drive_for(100, 0, 300);
    // Go to tube
    tube_arm(DOWN);
    puts("linefollow one sided");
    line_follow_one_sided_until(tube_switch(), LS_L, RIGHT, 50);
    stop();
    tube_arm(FLOOR);
    sleep_until_balls_drop(start);
    jiggle();
    tube_arm(DOWN);
    puts("backing up");
    until (lc_on_black(LC_BLACK)) move(-100, -100);
    tube_arm(OUT_OF_WAY);
    puts("turning out of way");
    drive_for(0, 100, 1500);
    stop();
    grabber(G_OPEN);
    drive_for(-60, -100, 1500);
    drive_for(100, 100, 300);
    tube_arm(WIRESHARK);
    ball_door(D_OPEN);
    msleep(1000);
    tube_arm(UP);
    puts("grabbing wireshark again");
    drive_for(70, 100, 1800);
    grabber(G_CLOSED);
    puts("done");
    puts("clean up");
    //drive_for(-50, -100, 2000);
    stop();
    tube_arm(OUT_OF_WAY);
    msleep(100);
    set_servo_position(GRABBER_SERVO, G_CLOSED);
    msleep(400);
    set_servo_position(ARM_SERVO, FLOOR);
    msleep(100);
    return 0;
}
