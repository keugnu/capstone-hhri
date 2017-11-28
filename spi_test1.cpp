// To test SPI
// Author(s): Stephen Brooks


#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


static void write_byte () {

}


int main() {
  int spi_fd = open("/dev/spidev3.0", 0_RDWR);
  if (spi_dev < 0) {
    pabort("Can't open spi device");
    return -1;
  }

}

