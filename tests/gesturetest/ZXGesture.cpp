/*
*  ZX Gesture Sensor test
*  
*  This test periodically polls for gestures and displays 
*  what gesture is made along with its speed.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char *argv[]) {

    // Create I2C bus
    int file;
    char *bus = "/dev/i2c-1";
    if ((file = open(bus, O_RDWR)) < 0) {
	printf("Failed to open the bus. \n");
	exit(1);
    }
    
    // Get I2C device, ZX Gesture sensor default I2C address is 0x10
    ioctl(file, I2C_SLAVE, 0x10);
    char buff[10] = {0};
    char data[10] = {0};

    // Read x coordinate (register 0x08)
    buff[0] = 0x08;

while(true) {
    write(file, buff, 1);
    read(file, data, 1);
    uint8_t xcoord = data[0];
    printf("X coordinate: %u\n", xcoord);
    usleep(50000);
}
    
}
