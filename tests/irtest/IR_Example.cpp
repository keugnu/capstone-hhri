#include "VL53L0X.h"
#include <stdio.h>

VL53L0X sensor;

int main (int argc, char *argv[])
{      
    sensor.init(0);
    sensor.setTimeout(500);
    sensor.startContinuous();

    while(1)
    {
        printf("Input: %u\n", sensor.readRangeContinuousMillimeters());
        if (sensor.timeoutOccurred())
        {
            printf("Timeout.\n");
        }
    }
}
