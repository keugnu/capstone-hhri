#include <stdio.h>
#include <unistd.h>
#include "AHA_PWMDriver.h"

int main(int argc, char *argv[])
{
	unsigned char addr = 0x40;
	AHA_PWMDriver pwm;
       
	pwm = AHA_PWMDriver(addr);

	pwm.begin();
	pwm.setPWMFreq(1600);

	while(1)
	{
		//Drive each pwm in a 'wave'
		for(uint16_t i = 0; i < 4096; i+= 8)
		{
			for(uint16_t pwmnum = 0; pwmnum < 16; pwmnum++)
			{
				pwm.setPWM(0, 0, 2048); //red = pwm.setPWM(0,0,2048)
				pwm.setPWM(1, 0, 1000); //green = pwm.setPWM(1,0,2048)
				pwm.setPWM(2, 0, 0); //blue = pwm.setPWM(2, 0, 2048);
				// red+blue = purple, red+green = yellow
			}
		}
	}

	return 0;
}
