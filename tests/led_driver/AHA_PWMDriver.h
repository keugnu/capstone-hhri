#ifndef  AHA_PWMDriver_H
#define  AHA_PWMDriver_H

#include <stdint.h>

#define PCA9685_SUBADR1   0x02
#define PCA9685_SUBADR2   0x03
#define PCA9685_SUBADR3   0x04

#define PCA9685_MODE1     0x00
#define PCA9685_PRESCALE  0xFE

#define LED0_ON_L         0x06
#define LED0_ON_H         0x07
#define LED0_OFF_L        0x08
#define LED0_OFF_H        0x09

#define ALLLED_ON_L       0xFA
#define ALLLED_ON_H       0xFB
#define ALLLED_OFF_L      0xFC
#define ALLLED_OFF_H      0xFD

//#define ENABLE_DEBUG_OUTPUT 1

class AHA_PWMDriver 
{
    public:
        AHA_PWMDriver(uint8_t addr = 0x40);
        void begin(void);
        void reset(void);
        void setPWMFreq(float freq);
        void setPWM(uint8_t num, uint16_t on, uint16_t off);
        void setPin(uint8_t num, uint16_t val, bool invert=false);
    
    private:
        uint8_t _i2caddr;

        void    write8(uint8_t, uint8_t d);
        uint8_t read8(uint8_t addr);
};

#endif
