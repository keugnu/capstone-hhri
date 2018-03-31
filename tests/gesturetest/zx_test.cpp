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
uint8_t x_pos;
uint8_t z_pos;

int main(int argc, char *argv[]) {
	uint8_t ver;
	
  // Initialize ZX Sensor (configure I2C and read model ID)
  if ( zx_sensor.init() ) {
    printf("ZX Sensor initialization complete\n");
  } else {
    printf("Something went wrong during ZX Sensor init!\n");
  }

	while(1) {
		// If there is position data available, read and print it
		if ( zx_sensor.positionAvailable() ) {
			x_pos = zx_sensor.readX();
			z_pos = zx_sensor.readZ();

			if ( x_pos != ZX_ERROR ) {
				printf("X: ");
				printf("%u\t", x_pos);
			}		
			if ( z_pos != ZX_ERROR ) {
				printf(" Z: ");
				printf("%u\n", z_pos);
			}
      usleep(500000);
		}
	}
	return 1;
}

