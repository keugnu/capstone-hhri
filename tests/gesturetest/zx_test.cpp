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

  // Read the model version number and ensure the library will work
  ver = zx_sensor.getModelVersion();
  if ( ver == ZX_ERROR ) {
    printf("Error reading model version number\n");
  } else {
    printf("Model version: ");
    printf("%u\n", ver);
  }
  if ( ver != ZX_MODEL_VER ) {
    printf("Model version needs to be ");
    printf("0x%x", ZX_MODEL_VER);
    printf(" to work with this library. Stopping.\n");
    while(1);
  }
  
  // Read the register map version and ensure the library will work
  ver = zx_sensor.getRegMapVersion();
  if ( ver == ZX_ERROR ) {
    printf("Error reading register map version number\n");
  } else {
    printf("Register Map Version: ");
    printf("%u\n", ver);
  }
  if ( ver != ZX_REG_MAP_VER ) {
    printf("Register map version needs to be ");
    printf("0x%x", ZX_REG_MAP_VER);
    printf(" to work with this library. Stopping.\n");
    while(1);
  }

	while(1) {
		// If there is position data available, read and print it
		if ( zx_sensor.positionAvailable() ) {
			x_pos = zx_sensor.readX();
			if ( x_pos != ZX_ERROR ) {
				printf("X: ");
				printf("%u", x_pos);
			}
			z_pos = zx_sensor.readZ();
			if ( z_pos != ZX_ERROR ) {
				printf(" Z: ");
				printf("%u\n", z_pos);
			}
      usleep(500000);
		}
	}
	return 1;
}



	while(1) {
		// If there is position data available, read and print it
    uint8_t x_array[30] = {0};
		for (int i = 0; i < 30; i++) {
      if ( zx_sensor.positionAvailable() ) {
        x_array[i] = zx_sensor.readX();
      }
    }
			
			

      usleep(500000);
		}
	}
