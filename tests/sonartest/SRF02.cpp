/* SRF02 Sonar Sensor
*  Datasheet specifies default address as 0xE0, but I2c addressing uses
*  the high 7 bits, so it's 0x70 (see below)
*  
*  0xE0 = 1110 0000 binary ---> high 7 bits = 0111 000 binary = 0x70
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

	unsigned char data[10];
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) {
		printf("Failed to open the bus. \n");
		exit(1);
	}

	// Get I2C device, SRF02 I2C address is 0x70
	ioctl(file, I2C_SLAVE, 0x70);

	while(true) {
	    // Set register pointer to command register (0x00)
	    // Reading range in centimeters (register 0x51), 2 bytes
	    // Range high byte and range low byte	    
	    data[0] = 0x00;
	    data[1] = 0x51;
	    write(file, data, 2);
	    usleep(900000);

	    // Register to read from
	    data[0] = 0x00;
	    write(file, data, 1);
	    
	    if(read(file, data, 4) != 4) {
		printf("Error : Input/Output error \n");
	    } else {
		// Shift the high byte (data[2]) to be higher 8 bits
		unsigned char high = data[2];
		unsigned char low = data[3];
	        unsigned int range = high;
		range <<= 8;
		// Set lower byte (data[3]) to be lower 8 bits
		range += low;

		// Output data to screen
		printf("SRF02 Range in centimeters : %.2f C \n", range);
	    }
	    usleep(900000);
	}
}
