#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants
#define BLACK 1500
#define BACK_BLACK 3700

#define SLOW 125
#define FAST 400

#define CLAW 1
#define ARM 0

#define CLAW_HOME 1000
#define ARM_HOME 2000

#define CLAW_CLOSE 2024
#define ARM_UP 0

#define LS 1

// Function prototypes
void stop();
void pause(int time);
void servo_control(int servo, int end_pos, int tick_delay);
void drive(int left_power, int right_power, int duration);
void wfl();
void main_program();

// Main program
int main()
{
    main_program();
    return 0;
}

// Function definitions

void stop()
{
    // All off (also K.create_stop())
    drive(0, 0, 0);
}

void pause(int time)
{
    stop();
    // Delay for the specified time
    if (time > 0)
        usleep(time * 1000);
}

void servo_control(int servo, int end_pos, int tick_delay)
{
    // Function for using servo
    int pos = get_servo_position(servo);
    printf("%d %d %d\n", servo, pos, end_pos);
    if (pos > end_pos)
    {
        for (int i = pos; i > end_pos; i -= 2)
        {
            set_servo_position(servo, i);
            pause(tick_delay);
        }
    }
    else
    {
        for (int i = pos; i < end_pos; i += 2)
        {
            set_servo_position(servo, i);
            pause(tick_delay);
        }
    }
}

void drive(int left_power, int right_power, int duration)
{
    // Drive for n milliseconds
    create_drive_direct(left_power, right_power);
    pause(duration);
    stop();
}

void wfl()
{
    // Wait for start light
    int START = analog(LS);
    while (analog(LS) > START / 2)
    {
        pause(50);
    }
}

void main_program()
{
    // Create Setup
    int test = 0;
    int loop = 0;

    printf("Press A to continue\n");
    printf("Press B to test\n");
    while (!loop)
    {
        if (a_button())
        {
            printf("Continuing\n");
            loop = 1;
        }
        else if (b_button())
        {
            test = 1;
            printf("Testing\n");
            loop = 1;
        }
    }

    printf("Turn on Create\n");
    create_connect();
    printf("Create Connected!\n");

    printf("Walaby Battery = %d\n", power_level());
    printf("Create Battery = %d\n", get_create_battery_capacity());
    enable_servos();
    printf("Servos Enabled\n");
    set_servo_position(ARM, ARM_HOME);
    set_servo_position(CLAW, CLAW_HOME);

    printf("Wait for Start Light\n");
    wfl();
    shut_down_in(118)
    double start_time = seconds();

    // Do stuff

    // Drive to middle line
    int lw = -FAST;
    int rw = -FAST;
    create_drive_direct(lw, rw);
    pause(1200); // Skip over first black line and knock over ring toy
    set_servo_position(ARM, 1000);

    // Look for Middle Line
    while ((lw > 0) && (rw > 0))
    {
        if (get_create_lcliff_amt() < 2000)
        {
            lw = 0;
            rw = 0;
        }
    }
    pause(500);

    // Rotate 45 degrees Counter-Clockwise
    drive(-SLOW, SLOW, 900);
    pause(500);

    // Drive to Center Pipe
    create_drive_direct(-lw, -rw);
    pause(1000);
    pause(500);

    // Get Botgal
    set_servo_position(ARM, ARM_UP);
    pause(500);
    set_servo_position(CLAW, CLAW_CLOSE);
    pause(500);
    set_servo_position(ARM, ARM_HOME);
    pause(500);

    // Drive Straight to Drop Position
    create_drive_direct(FAST, FAST);
    pause(1200);
    pause(500);

    // Rotate 135 degrees Counter-Clockwise
    drive(-SLOW, SLOW, 1200);
    pause(500);

    // Drop Botgal
    set_servo_position(CLAW, CLAW_HOME);
    pause(500);

    // Clean up
    create_stop();
    disable_servos();
    create_disconnect();
    printf("Tada\n");
}
