#ifndef
#define _SPI_H

//based on http://www.raspberry-projects.com/pi/programming-in-c/spi/using-the-spi-interface

#include <linux/spi/spidev.h>

//spi_device	0=/dev/spidev0.0, 1=/dev/spidev0.1
//SPI_MODE_0    CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
//SPI_MODE_1    CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
//SPI_MODE_2    CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
//SPI_MODE_3    CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
//wordlength    bits per word, only 8 or 9 allowed by the raspberry pi
//speed         SPI clock frequency: 1,2,4,8,16 and 32 MHz supported: 1Mhz:speed=1000000
int SpiOpenPort(int spi_device, unsigned char mode, unsigned char wordlength, unsigned int speed);

//spi_device	0=/dev/spidev0.0, 1=/dev/spidev0.1
int SpiClosePort (int spi_device);

//spi_device	0=/dev/spidev0.0, 1=/dev/spidev0.1
//data          overwritten by read data
int SpiWriteAndRead (int spi_device, unsigned char *data, int length);
#endif
