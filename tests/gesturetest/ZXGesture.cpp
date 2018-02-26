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
    char buff[1] = {0};
    char data[1] = {0};
    uint8_t status = 0x00;
    uint8_t gesture = 0x00;
/*
    while(true) {
	buff[0] = 0x08;
	write(file, buff, 1);
	if((read(file, data, 1)) != 1) {
	    printf("I/O error.\n");
	    exit(1);
	}
	status = data[0];
	printf("X: %u \n", status);
	usleep(25000);
    }
*/

/*    
    while(1) {
	// Initialize sensor, check status register to see if swipe gesture available	
	buff[0] = 0x00;
	write(file, buff, 1);
	// If swipe gesture data is available
	if ((read(file, data, 1)) != 1) {
	    printf("I/O error. \n");
	    exit(1);
	}
	status = data[0];
	
	// bitwise AND with 0001 1100. If this value is 1, then a 
	// hover gesture, hover-move gesture, or swipe gesture available.
	status &= 0b00011100;

	// Read from gesture register if there is a gesture available
	// Gesture reg = 0x04. 
	if (status) {
	    buff[0] = 0x04;
	    write(file, buff, 1);
	    read(file, data, 1);
	    gesture = data[0];
	    printf("Gesture data: %u\n", gesture);

	    switch (gesture) {
		case 0x01:
		    printf("Right swipe\n"); break;
		case 0x02:
		    printf("Left swipe\n"); break;
		case 0x03:
		    printf("Up swipe\n"); break;
		default:
		    printf("No gesture\n"); break;
	    }
	}
	
	usleep(250000);
    }
*/ 
}
