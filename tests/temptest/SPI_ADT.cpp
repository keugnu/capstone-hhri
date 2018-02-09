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

    // Configure the SPI bus
    spi_config.mode = 0;
    spi_config.speed = 10000;
    spi_config.delay = 0;
    spi_config.bits_per_word = 8;

    SPI *mySPI = new SPI("/dev/spidev3.0", &spi_config);

    if(mySPI->begin()) {
	uint8_t txBuffer[3] = {0x90, 0x4D, 0x00};
	uint8_t rxBuffer[3] = {0x00, 0x00, 0x00};

	// Device id register: 0x4D
	// Reading  device id before attempting to read temperature
	while(true) {
	    mySPI->xfer(txBuffer, 3, rxBuffer, 3);
   	    printf("ID: %u - %u - %u \n", rxBuffer[2], rxBuffer[1], rxBuffer[0]);
	    sleep(1);	
	}
	delete mySPI;
    } else {
	printf("Failed to access SPI bus. \n");
    }

    return 1;

}

