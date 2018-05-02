#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <ZX_Sensor.h>

// Constants
const int ZX_ADDR = 0x10;    // ZX Sensor I2C address

// Global Variables
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
GestureType gesture;
uint8_t gesture_speed;

int main(int argc, char *argv[]) {
  
  uint8_t ver;
  
  // Initialize ZX Sensor (configure I2C and read model ID)
  zx_sensor.init();
  //if ( zx_sensor.init() ) {
    printf("ZX Sensor initialization complete\n");
  //} else {
  //  printf("Something went wrong during ZX Sensor init!\n");
  //}
  
  while(1) {
    bool gest_avail = zx_sensor.gestureAvailable();
    printf("gesture avail: %d\n", gest_avail);
    // If there is gesture data available, read and print it
    if ( gest_avail ) {
      gesture = zx_sensor.readGesture();
      gesture_speed = zx_sensor.readGestureSpeed();
      printf("Gesture: %u, \t Gesture speed: %u\n", (uint8_t)gesture, gesture_speed);

      switch ( gesture ) {
        case NO_GESTURE:
          printf("No Gesture\n");
          break;
        case RIGHT_SWIPE:
          printf("Right Swipe. Speed: \n");
          printf("%u", gesture_speed);
          break;
        case LEFT_SWIPE:
          printf("Left Swipe. Speed: \n");
          printf("%u", gesture_speed);
          break;
        case UP_SWIPE:
          printf("Up Swipe. Speed: \n");
          printf("%u", gesture_speed);
          break;
        default:
          break;
      } // end switch
      usleep(100000);
    } // end if
  } // end while
  return 0;
} // end main
