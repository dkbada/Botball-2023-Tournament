#include <kipr/wombat.h>

int test = 0;
int ARM = 0;
int TRAY = 1;
int CLAWTURN_SERVO = 0;
int CLAW = 1;


void stop() {
  create_drive_direct(0, 0);
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

int main()
{
    #turn on and wfl
    printf("Turn on Create\n");
    create_connect();
    printf("Create Connected!\n");
    enable_servos();
    printf("Servos Enabled\n");
    set_servo_position(ARM, ARM_HOME);
    set_servo_position(CLAW, CLAW_HOME);
    printf("Wait for Start Light\n");
    wfl();
    shut_down_in(118);
    
    
}
