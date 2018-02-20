/*
* MPR121 Touch Sensor
*
* This test is performed by seeing the response registered by each pin
* on the left side of the breakout board.
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
    
    // Get I2C device, MPR121 default I2C address is 0x5A
    ioctl(file, I2C_SLAVE, 0x5A);
    char buff[10];

    // Perform a soft reset by writing 0x63 to register 0x80 (Soft Reset Register)
    buff[0] = 0x80; buff[1] = 0x63;
    write(file, buff, 2);

    // MPR121 goes into "Run mode" when register 0x5E (ECR Register) is NOT all 0s
    // MPR121 goes into "Stop mode" when this register is all 0s.
    buff[0] = 0x5E; buff[1] = 0x00;
    write(file, buff, 2);
    sleep(1);

    // Each of the 13 touch channels has its own set of touch & release threshold regs
    // Touch threshold reg starts at 0x41, release at 0x42.
    // For simple test, touch threshold = 12, release = 6. Loop through all channels and set:
    char t_buff[2] = {0x41, 12};
    char r_buff[2] = {0x42, 6};

    for (int i = 0; i<12; i++) {
	write(file, t_buff, 2);
        t_buff[0] += i*2;

	write(file, r_buff, 2);
	r_buff[0] += i*2;
    }

    // Setup registers:
    // MHD rising reg 0x2B, NHD rising reg 0x2C, NCL rising reg 0x2D, FDL rising reg 0x2E
    buff[0] = 0x2B, buff[1] = 0x01; write(file, buff, 2);
    buff[0] = 0x2C, buff[1] = 0x01; write(file, buff, 2);
    buff[0] = 0x2D, buff[1] = 0x0E; write(file, buff, 2);
    buff[0] = 0x2E, buff[1] = 0x00; write(file, buff, 2);

    // MHD falling reg, 0x2F, NHD falling reg 0x30, NCL falling reg 0x31, FDL falling reg 0x32
    buff[0] = 0x2F, buff[1] = 0x01; write(file, buff, 2);
    buff[0] = 0x30, buff[1] = 0x05; write(file, buff, 2);
    buff[0] = 0x31, buff[1] = 0x01; write(file, buff, 2);
    buff[0] = 0x32, buff[1] = 0x00; write(file, buff, 2);

    // NHD touched reg 0x33, NCL touched 0x34, FDL touched 0x35
    buff[0] = 0x33, buff[1] = 0x00; write(file, buff, 2);
    buff[0] = 0x34, buff[1] = 0x00; write(file, buff, 2);
    buff[0] = 0x35, buff[1] = 0x00; write(file, buff, 2);

    // Debounce touch & release reg 0x5B, config 1 reg 0x5c, config 2 ref 0x5D
    buff[0] = 0x5B, buff[1] = 0x00; write(file, buff, 2);
    buff[0] = 0x5C, buff[1] = 0x10; write(file, buff, 2); // default, 16uA charge current
    buff[0] = 0x5D, buff[1] = 0x20; write(file, buff, 2); // 0.5us encoding, 1ms period
    // Start with first 5 bits of baseline tracking
    buff[0] = 0x5E; buff[1] = 0x8F; write(file, buff, 2);

    while(true) {

	uint16_t wasTouched = 0x0000, currentlyTouched[2] = {0x0000}, cTouched = 0;
	buff[0] = 0x00; // Touch status register
	write(file, buff, 1);
	read(file, currentlyTouched, 2);
	cTouched = currentlyTouched[0];
	cTouched |= currentlyTouched[1] << 8;
	cTouched &= 0x0FFF;

	for(int i = 0; i < 12; i++) {
	    if ((cTouched & (1 << i)) && !(wasTouched & (1 << i)))
		printf("Pin %i was touched.\n", i);
	    if (!(cTouched & (1 << i)) && (wasTouched & (1 << i)))
		printf("Pin %i was released.\n", i);
	}

	wasTouched = cTouched;
	usleep(50000);


    }
}

