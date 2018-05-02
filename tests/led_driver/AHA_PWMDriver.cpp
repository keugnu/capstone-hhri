#include "AHA_PWMDriver.h"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

AHA_PWMDriver::AHA_PWMDriver(uint8_t addr)
{
    _i2caddr = addr;
}

void AHA_PWMDriver::begin(void)
{
    reset();

    setPWMFreq(1000);
}

void AHA_PWMDriver::reset(void)
{
    write8(PCA9685_MODE1, 0x80);

    usleep(10);
}

void AHA_PWMDriver::setPWMFreq(float freq)
{
#ifdef ENABLE_DEBUG_OUTPUT
    printf("Attempting to set the freq %d\r\n", freq);
#endif

    freq *= 0.9;
    float prescaleval = 25000000;
    prescaleval /= 4096;
    prescaleval /= freq;
    prescaleval -=1;

#ifdef ENABLE_DEBUG_OUTPUT
    printf("Estimated pre-scale: %d\r\r\n", prescale)
#endif

    uint8_t prescale = floor(prescaleval + 0.5);
    
    uint8_t oldmode = read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode & 0x7F) | 0x10; //sleep
    write8(PCA9685_MODE1, newmode);            //go to sleep
    write8(PCA9685_MODE1, prescale);           //set the prescaler
    write8(PCA9685_MODE1, oldmode);

    usleep(5);

    write8(PCA9685_MODE1, oldmode | 0xa0);   //This sets the MODE1 register to 
                                             //turn on auto increment;

#ifdef ENABLE_DEBUG_OUTPUT
    printf("Mode now 0x%x\r\n",read8(PCA9685_MODE1));
#endif
}

void AHA_PWMDriver::setPWM(uint8_t num, uint16_t on, uint16_t off)
{
#ifdef ENABLE_DEBUG_OUTPUT
    printf("Setting PWM %d: %d -> %d", num, on, off);
#endif

    int ret;
    int len;
    int file;

    char write_reg[5];
    char const *bus = "/dev/i2c-1";

    write_reg[0] = LED0_ON_L + 4 *num;
    write_reg[1] = on;
    write_reg[2] = (on>>8);
    write_reg[3] = off;
    write_reg[4] = (off>>8);

    if((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open the bus(setPWM)\r\n");
        return;
    }

    ioctl(file, I2C_SLAVE, _i2caddr);

    ret = write(file, &write_reg[0], sizeof(write_reg));
    if(ret == 0)
    {
        printf("Failed to write to the registers(setPWM)\r\n");
        return;
    }

    if (close(file) < 0)
    {
        printf("Failed to close the bus(setPWM)\r\n");
        return;
    }
}

void AHA_PWMDriver::setPin(uint8_t num, uint16_t val, bool invert)
{    
    if(val > (uint16_t)4095)
    {
    val = (uint16_t)4095;
    }

    if(invert)
    {
        setPWM(num, 4096, 0);
    }
    else if(val == 4095)
    {
        setPWM(num, 0, 4096);
    }
    else
    {
        if(val == 4096)
        {
            setPWM(num, 4096, 0);
        }
        else if(val == 0)
        {
            setPWM(num, 0, 4096);
        }
        else
        {
            setPWM(num, 0, val);
        }
    }
}

uint8_t AHA_PWMDriver::read8(uint8_t addr)
{
    int ret;
    int len;
    int file;

    char write_reg[1];
    char read_reg[1];
    char const *bus = "/dev/i2c-1";

    write_reg[0] = addr;

    if((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open the bus(read8)\r\n");
        return 0;
    }
    
    ioctl(file, I2C_SLAVE, _i2caddr);
    
    ret = write(file, &write_reg[0], sizeof(write_reg));
    if(ret == 0)
    {
        printf("Failed to write to the registers(read8)\r\n");
        return 0;
    }

    ret = read(file, &read_reg[0], sizeof(read_reg));
    if(ret == 0)
    {
        printf("Failed to read from the registers(read8)\r\n");
        return 0;
    }

    if (close(file) < 0)
    {
        printf("Failed to close the bus(read8)\r\n");
        return 0;
    }

    return (uint8_t)read_reg[0];
}

void AHA_PWMDriver::write8(uint8_t addr, uint8_t d)
{
    int ret;
    int len;
    int file;

    char write_reg[2];
    char const *bus = "/dev/i2c-1";

    write_reg[0] = addr;
    write_reg[1] = d;

    if((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open the bus(write8)\r\n");
        return;
    }

    ioctl(file, I2C_SLAVE, _i2caddr);

    ret = write(file, &write_reg[0], sizeof(write_reg));
    if(ret == 0)
    {
        printf("Failed to write to the registers(write8)\r\n");
        return;
    }

    if (close(file) < 0)
    {
        printf("Failed to close the bus(write8)\r\n");
        return;
    }
}
