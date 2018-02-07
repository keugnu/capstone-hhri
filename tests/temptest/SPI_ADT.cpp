/* ADT7411 Temperature Sensor using SPI:
*  SPI Command Words: Write: 0x90, Read: 0x91
*  SPI uses "chip select" ID method instead of "address" of peripheral.
*  SPI is double directional: read and write always executed in parallel.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include "spidev_lib++.h"

spi_config_t spi_config;

int main(int argc, char *argv[]) {

    uint8_t lsb, msb;

    // Configure the SPI bus
    spi_config.mode = 0;
    spi_config.speed = 1000000;
    spi_config.delay = 0;
    spi_config.bits_per_word = 8;

    SPI *mySPI = new SPI("/dev/spidev3.0", &spi_config);

    if(mySPI->begin()) {
	uint8_t config[3] = {0};
	// Write to configuration Registers (0x18, 0x19)
	config[0] = 0x90;
	config[1] = 0x18;
	config[2] = 0x01;
	mySPI->write(config,3);

	config[1] = 0x19;
	config[2] = 0x11;
	mySPI->write(config,3);

	uint8_t data[1];
	uint8_t reg[2];
	reg[0] = 0x90;
	reg[1] = 0x4D;
	data[0] = 0x91;

	// Device id register: 0x4D
	// Reading  device id before attempting to read temperature
	while(true) {
	    mySPI->xfer(reg, 2, data, 1);
      	    printf("Device ID: %u \n", data[0]);
	    sleep(1);
	}

/*	
	mySPI->write(reg,1);
	mySPI->read(data,1);
	lsb = data[0]%4;

	reg[0] = 0x07;
	mySPI->write(reg,1);
	mySPI->read(data,1);
	msb = data[0];

	// Print temperatures in C and F
	float temp = ((msb << 2) + lsb) * 0.25;
	float ftemp = temp * 1.8 + 32;
	printf("ADT Temperature in Celsius: %.2f C \n", temp);
	printf("ADT Temperature in Fahrenheit: %.2f F \n", ftemp);
*/
	delete mySPI;
    } else {
	printf("Failed to access SPI bus. \n");
    }

    return 1;
}
