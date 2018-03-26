#include <unistd.h>
#include "ZX_Sensor.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Constants
const int ZX_ADDR = 0x10;    // ZX Sensor I2C address

// Global Variables
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
GestureType gesture;
uint8_t gesture_speed;

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
    exit(1);
  }
  
  // Read the register map version and ensure the library will work
  ver = zx_sensor.getRegMapVersion();
  if ( ver == ZX_ERROR ) {
    printf("Error reading register map version number\n");
  } else {
    printf("Register Map Version: \n");
    printf("%u\n", ver);
  }
  if ( ver != ZX_REG_MAP_VER ) {
    printf("Register map version needs to be ");
    printf("0x%x", ZX_REG_MAP_VER);
    printf(" to work with this library. Stopping.\n");
    exit(1);
  }
  


while(1) {
  // If there is gesture data available, read and print it
//  if ( zx_sensor.gestureAvailable() ) {
    gesture = zx_sensor.readGesture();
    gesture_speed = zx_sensor.readGestureSpeed();
    printf("Gesture: %u, \t Gesture speed: %u\n", (uint8_t)gesture, gesture_speed);

    switch ( gesture ) {
      case NO_GESTURE:
        printf("No Gesture\n");
        break;
      case RIGHT_SWIPE:
        printf("Right Swipe. Speed: ");
        printf("%u\n", gesture_speed);
        break;
      case LEFT_SWIPE:
        printf("Left Swipe. Speed: ");
        printf("%u\n", gesture_speed);
        break;
      case UP_SWIPE:
        printf("Up Swipe. Speed: ");
        printf("%u\n", gesture_speed);
        break;
      default:
        break;
    }
    usleep(1000000);
//  }
}
return 0;
}
