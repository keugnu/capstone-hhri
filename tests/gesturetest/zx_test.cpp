/* ZX gesture test, measures x and z distance */


// System
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>

// Custom
#include <ZX_Sensor.h>

// Constants
const int ZX_ADDR = 0x10;  // ZX Sensor I2C address

// Global Variables
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
uint8_t x_pos[10];
uint8_t z_pos[10];

int main(int argc, char *argv[]) {
	uint8_t ver;
	uint8_t last_x = 0;
	
  // Initialize ZX Sensor (configure I2C and read model ID)
  if ( zx_sensor.init() ) {
    printf("ZX Sensor initialization complete");
  } else {
    printf("Something went wrong during ZX Sensor init!");
  }

	while(1) {
		// Store 10 position readings
		for (int i = 0; i < 5; ) {
			if ( zx_sensor.positionAvailable() ) {
				x_pos[i] = zx_sensor.readX();
				z_pos[i] = zx_sensor.readZ();
				i++;
				usleep(50000);
			}
		}

		int i = 0;
		for (i = 0; i < 5; i++) {
			if (z_pos[i] > 20)
				break;
		}
		if (i == 5) {
			printf("Wave\n");
			printf("\n \n");
			sleep(1);			
		}

	}
	return 1;
}
