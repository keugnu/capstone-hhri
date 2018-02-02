/* ADT7411 temperature sensor */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    // Create i2c bus
    int file;
    char *bus = "/dev/i2c-1";
    if ((file = open(bus, O_RDWR)) < 0) {
	printf("Failed to open the bus. \n");
	exit(1);
    }

    // Get i2c device address, ADT7411 0x4a
    ioctl(file, I2C_SLAVE, 0x4A);

    // Control configuration 2 register (0x19)
    // Temperature data is stored in twos complement form.
    // Offset is added to the measured value to enable the transfer function to
    // match the thermal characteristics, offset stored in internet temperature offset reg.
    // Positive temp = ADC code/4. MSB: 0x07, LSB: 0x03

    char config[2] = {0};

    // Control configuration register 2 (0x19), set up operating mode for device
    // Write 0x11 to select internal temperature sensor and use single-channel conversion cycle
    config[0] = 0x18;
    config[1] = 0x01;
    write(file, config, 2);

    config[0] = 0x19;
    config[1] = 0x11;
    write(file, config, 2);
    sleep(1);

    int8_t lsb, msb;

while(true) {    
    // Read LSB using internal temperature sensor
    char reg[1] = {0x03};
    char data[1] = {0};
    write(file, reg, 1);
    if(read(file, data, 1) != 1) {
	printf("LSB I/O error \n");
    }
    
    lsb = data[0]%4;
    
    // Read MSB using internal temperature sensor
    reg[0] = 0x07;
    write(file, reg, 1);
    if(read(file, data, 1) != 1) {
	printf("MSB I/O error \n");
    }

    msb = data[0];

    // Convert to C/F using MSB and LSB values
    float temp = ((msb << 2) + lsb) * 0.25;
    float ftemp = temp * 1.8 + 32;

    printf("ADT Temperature in Celsius: %.2f C \n", temp);
    printf("ADT Temperature in Fahrenheit: %.2f F \n", ftemp);
    sleep(2);
}
}
