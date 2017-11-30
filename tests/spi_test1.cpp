// To test SPI
// Author(s): Stephen Brooks
// compile with -std=c++14


#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <cstdint>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


static const char* device = "/dev/spidev3.0";
static uint8_t mode = 0;
static uint8_t bits = 8;
// static uint32_t speed = 500000;
static uint16_t delay = 0;
static struct spi_ioc_transfer* spi_xfrs = NULL;
static int ntransfers = 0;
static int toggle_cs = 0;
static int block_length = 0;
static int quiet = 0;


static void add_transfer(char* data, int len)
{
	struct spi_ioc_transfer *xfr;
	char *txbuf = new char(len);
	char *rxbuf = new char(len);

	std::memcpy(txbuf, data, len);
	std::memset(rxbuf, 0xff, len);
	
	ntransfers += 1;
	spi_xfrs = (spi_ioc_transfer*)realloc(spi_xfrs, sizeof(*spi_xfrs)* ntransfers);
	xfr = &spi_xfrs[ntransfers - 1];

	xfr->tx_buf = (unsigned long)txbuf;
	xfr->rx_buf = (unsigned long)rxbuf;
	xfr->len = len;
	// xfr->speed_hz = speed;
	xfr->delay_usecs = delay;
	xfr->bits_per_word = bits;
	xfr->cs_change =  toggle_cs;
	xfr->pad = 0;
}



int main() {
  int spi_fd = open(device, O_RDWR);
  if (spi_fd < 0) {
    perror("Can't open spi device");
    exit(1);
  }



}

