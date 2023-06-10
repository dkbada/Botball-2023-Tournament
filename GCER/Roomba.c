#include <kipr/botball.h>

#define BLACK 1500
#define BACK_BLACK 3700

#define SLOW 125
#define FAST 600
#define FAST_BOTGAL 3000

#define CLAW 1
#define ARM 0

#define CLAW_HOME 900
#define CLAW_MID 1850
#define ARM_HOME 2047

#define CLAW_CLOSE 2100
#define ARM_UP 120
#define ARM_MID 1850

#define LS 1

int test = 0;

void stop() {
  create_drive_direct(0, 0);
}

void pause(int time) {
  stop();
  if (test) {
    printf("Press C to continue\n");
    while (!c_button());
  }
  msleep(time);
}

void drive(int left_power, int right_power, int duration) {
  create_drive_direct(left_power, right_power);
  msleep(duration);
  stop();
}

void wfl() {
  int START = analog(LS);
  while (analog(LS) > START / 2) {
    msleep(50);
  }
}

void servo_control(int servo_name, int end_pos, int rate) {
  int pos = get_servo_position(servo_name);
  printf("%d %d %d\n", servo_name, pos, end_pos);
  if (pos > end_pos) {
    for (int i = pos; i > end_pos; i -= rate) {
      set_servo_position(servo_name, i);
      msleep(rate);
    }
  } else {
    for (int i = pos; i < end_pos; i += rate) {
      set_servo_position(servo_name, i);
      msleep(rate);
    }
  }
}

void grabby(int st, int claw_pos) {
  set_servo_position(CLAW, claw_pos);
  set_servo_position(ARM, ARM_UP);
  pause(700);
  set_servo_position(CLAW, CLAW_CLOSE);
  pause(700);
  set_servo_position(ARM, 1000);
  if (st != -1) {
    printf("Got botgal at %.3f seconds\n", (float)(seconds() - st) / 1000.0);
  }
  msleep(400);
  servo_control(ARM, ARM_HOME, 1);
  pause(800);
}

void main() {
  int loop = 0;
  test = 0;
  printf("Press A to continue\n");
  printf("Press B to test\n");
  while (!loop) {
    if (a_button()) {
      printf("Continuing\n");
      loop = 1;
    } else if (b_button()) {
      test = 1;
      printf("Testing\n");
      loop = 1;
    }
  }
  printf("Turn on Create\n");
  create_connect();
  printf("Create Connected!\n");
  printf("Walaby Battery = %s\n", power_level());
  printf("Create Battery = %s\n", get_create_battery_capacity());
  enable_servos();
  printf("Servos Enabled\n");
  set_servo_position(ARM, ARM_HOME);
  set_servo_position(CLAW, CLAW_HOME);
  printf("Wait for Start Light\n");
  wfl();
  shut_down_in(118);
  
  int start_time = seconds();
  shut_down_in(115);
  create_drive_direct(-FAST_BOTGAL, -FAST_BOTGAL);
  msleep(2000);
  printf("  Left Cliff Sensor = %d\n", get_create_lcliff_amt());
  while (get_create_lcliff_amt() > 2000);
  printf("  Left Cliff Sensor = %d\n", get_create_lcliff_amt());
  pause(100);
  drive(-SLOW, SLOW, 900);
  set_servo_position(ARM, 1000);
  pause(100);
  drive(-FAST, -FAST, 900);
  pause(100);
  grabby(start_time, -1);
  drive(FAST, FAST, 950);
  pause(500);
  drive(SLOW, -SLOW, 550);
  pause(500);
  set_servo_position(CLAW, CLAW_HOME);
  pause(500);
  drive(-SLOW, SLOW, 600);
  pause(100);
  drive(-FAST, -FAST, 1960);
  pause(100);
  drive(SLOW, SLOW, 500);
  drive(SLOW, -SLOW, 1250);
  pause(100);
  while (get_create_lfcliff_amt() > 2000) {
    create_drive_direct(-SLOW, -SLOW);
  }
  drive(-SLOW, -SLOW, 1600);
  pause(300);
  drive(-SLOW, SLOW, 1250);
  drive(-SLOW, -SLOW, 1000);
  pause(1500);
  drive(SLOW, SLOW, 400);
  grabby(-1, 950);
  msleep(500);
  drive(SLOW, -SLOW, 600);
  drive(FAST, FAST, 1480);
  set_servo_position(CLAW, CLAW_HOME);
  msleep(500);
  drive(-FAST, FAST, 305);
  drive(-FAST, -FAST, 1800);
  drive(-SLOW, -SLOW, 1300);
  drive(SLOW, SLOW, 950);
  msleep(300);
  drive(-SLOW, SLOW, 1250);
  while (get_create_rfcliff_amt() > 2000) {
    create_drive_direct(SLOW, SLOW);
  }
  pause(300);
  drive(SLOW, SLOW, 2510);
  drive(SLOW, -SLOW, 1250);
  set_servo_position(ARM, ARM_MID);
  pause(200);
  drive(SLOW, SLOW, 3750);
  pause(2000);
  set_servo_position(CLAW, CLAW_CLOSE);
  pause(500);
  set_servo_position(ARM, ARM_UP);
  pause(500);
  drive(-SLOW, -SLOW, 200);
  drive(SLOW, -SLOW, 1250);
  drive(SLOW, SLOW, 600);
  servo_control(ARM, ARM_HOME, 1);
  pause(300);
  set_servo_position(CLAW, CLAW_HOME);
  pause(300);
  drive(SLOW, SLOW, 1200);
  set_servo_position(ARM, ARM_UP);
  set_servo_position(CLAW, CLAW_MID);
  pause(300);
  drive(-SLOW, -SLOW, 1000);
  drive(-SLOW, SLOW, 1270);
  drive(-SLOW, -SLOW, 600);
  pause(500);
  set_servo_position(ARM, ARM_MID);
  pause(500);
  drive(SLOW, SLOW, 1400);
  set_servo_position(CLAW, CLAW_CLOSE);
  pause(1000);
  drive(-SLOW, -SLOW, 300);
  pause(300);
  set_servo_position(ARM, ARM_UP);
  pause(500);
  drive(SLOW, -SLOW, 1275);
  drive(SLOW, SLOW, 600);
  drive(-SLOW, -SLOW, 1100);
  servo_control(ARM, ARM_HOME, 1);
  pause(300);
  set_servo_position(CLAW, CLAW_HOME);
  pause(500);
  drive(SLOW, SLOW, 1100);
  set_servo_position(ARM, ARM_UP);
  pause(400);
  drive(-SLOW, SLOW, 640);
  drive(-SLOW, -SLOW, 400);
  set_servo_position(ARM, ARM_MID);
  set_servo_position(CLAW, CLAW_HOME);
  pause(500);
  drive(SLOW, SLOW, 1050);
  set_servo_position(CLAW, CLAW_CLOSE);
  pause(300);
  set_servo_position(ARM, ARM_UP);
  drive(SLOW, -SLOW, 925);
  drive(-SLOW, -SLOW, 1000);
  servo_control(ARM, ARM_HOME, 1);
  pause(300);
  set_servo_position(CLAW, CLAW_HOME);
  drive(SLOW, SLOW, 1000);
  pause(500);
  drive(-SLOW, -SLOW, 600);
  ao();
  create_stop();
  disable_servos();
  create_disconnect();
  printf("Tada\n");
}

int main() {
  sys.stdout = sys.stderr = fopen("/dev/null", "w");
  main_program();
  return 0;
}
