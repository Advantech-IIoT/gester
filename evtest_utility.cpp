#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <errno.h>

#include "evtest_utility.h"

const char *ABS_VAL[PROPERTY_LENGTH] = { "Value", "Min", "Max", "Fuzz", "Flat", "Resolution" };

void init_touch_device_max(const char* touch_device, int &x_max, int &y_max) {
    int fd = open(touch_device, O_RDONLY);
    if ((fd < 0) && (errno == EACCES) && (getuid() != 0)) {
        printf("Could not open device file. Try running as root.\n");
        return;
    }

    int absX[PROPERTY_LENGTH] = {0};
    int absY[PROPERTY_LENGTH] = {0};

    ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), absX);
    ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), absY);

    // printf("ABS_MT_POSITION_X Properties\n");
    // for (int k = 0; k < PROPERTY_LENGTH; k++) {
    //     if ((k < 3) || absX[k]) {
    //         printf("      %s %6d\n", ABS_VAL[k], absX[k]);
    //     }
    // }

    // printf("ABS_MT_POSITION_Y Properties\n");
    // for (int k = 0; k < PROPERTY_LENGTH; k++) {
    //     if ((k < 3) || absY[k]) {
    //         printf("      %s %6d\n", ABS_VAL[k], absY[k]);
    //     }
    // }

    x_max = absX[PROPERTY_MAX_INDEX];
    y_max = absY[PROPERTY_MAX_INDEX];

    close(fd);
}
