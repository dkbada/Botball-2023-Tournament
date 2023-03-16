#include <kipr/wombat.h>

int main() {
    while (1) {
        printf("gyro x: %hi, y: %hi, z: %hi\n", gyro_x(), gyro_y(), gyro_z());
        msleep(1000);
    }
    return 0;
}
