#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char *argv[]) 
{
    // Create I2C bus
    int file;
    char *bus = "/dev/i2c-1";
    if ((file = open(bus, O_RDWR)) < 0) 
    {
        printf("Failed to open the bus. \n");
        exit(1);
    }
    // Get I2C device, MCP9808 I2C address is 0x18(24)
    ioctl(file, I2C_SLAVE, 0x18);

    // Select configuration register(0x01)
    // Continuous conversion mode, Power-up default(0x00, 0x00)
    uint8_t  config[3] = {0};
    config[0] = 0x01;
    config[1] = 0x00;
    config[2] = 0x00;
    write(file, config, 3);
    // Select resolution rgister(0x08)
    // Resolution = +0.0625 / C(0x03)
    config[0] = 0x08;
    config[1] = 0x03;
    write(file, config, 2);
    sleep(1);
    while(true) {
    // Read 2 bytes of data from register(0x05)
    // temp msb, temp lsb
    uint8_t reg[1] = {0x05};
    write(file, reg, 1);
    uint8_t data[2] = {0};
    if(read(file, data, 2) != 2)
    {
        printf("Error : Input/Output error \n");
    }
    else
    {
        // Convert the data to 13-bits
        int temp = ((data[0] & 0x1F) * 256 + data[1]);
        if(temp > 4095)
        {
            temp -= 8192;
        }
        float cTemp = temp * 0.0625;
        float fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        printf("MCP Temperature in Celsius is : %.2f C \n", cTemp);
        printf("MCP Temperature in Fahrenheit is : %.2f F \n", fTemp);
    }
    sleep(2);
    }
}
