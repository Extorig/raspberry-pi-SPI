#include "spi.hpp"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

//file descriptors for the spi devices
int spi_cs0_fd;
int spi_cs1_fd;

//SPI_MODE_0 (0,0) 	CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
//SPI_MODE_1 (0,1) 	CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
//SPI_MODE_2 (1,0) 	CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
//SPI_MODE_3 (1,1) 	CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
unsigned char spi_mode;

//raspberry pi only supports 8 and 9 bits per word
unsigned char spi_bitsPerWord;

//1, 2, 4, 8, 16 and 32 MHz supported
unsigned int spi_speed;


//spi_device	0=/dev/spidev0.0, 1=/dev/spidev0.1
//SPI_MODE_0    CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
//SPI_MODE_1    CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
//SPI_MODE_2    CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
//SPI_MODE_3    CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
//wordlength    bits per word, only 8 or 9 allowed by the raspberry pi
//speed         SPI clock frequency: 1,2,4,8,16 and 32 MHz supported: 1Mhz:speed=1000000
int SpiOpenPort (int spi_device, unsigned char mode, unsigned char wordlength, unsigned int speed)
{
    int status_value = -1;
    int spi_cs_fd;

    spi_mode = mode;

    spi_bitsPerWord = wordlength;

    spi_speed = speed;


    if (spi_device)
        spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
    else
        spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);

    if (spi_cs_fd < 0)
    {
        perror("Error - Could not open SPI device");
        exit(1);
    }

    status_value = ioctl(spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    status_value = ioctl(spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status_value < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }

    if (spi_device)
        spi_cs1_fd= spi_cs_fd;
    else
        spi_cs0_fd= spi_cs_fd;

    return(status_value);
}


int SpiClosePort (int spi_device)
{
    int status_value = -1;
    if (spi_device)
        status_value = close(spi_cs1_fd);
    else
        status_value = close(spi_cs0_fd);

    if(status_value < 0)
    {
        perror("Error - Could not close SPI device");
        exit(1);
    }
    return(status_value);
}


int SpiWriteAndRead (int spi_device, unsigned char *data, int length)
{
    struct spi_ioc_transfer spi;
    int retVal = -1;

    //one spi transfer for all data
    spi.tx_buf        = (unsigned long)(data);
    spi.rx_buf        = (unsigned long)(data);
    spi.len           = sizeof(*data)*length;
    spi.delay_usecs   = 0 ;
    spi.speed_hz      = spi_speed ;
    spi.bits_per_word = spi_bitsPerWord ;
    spi.cs_change = 0;

    if (spi_device)
        retVal = ioctl(spi_cs1_fd, SPI_IOC_MESSAGE(1), &spi);
    else
        retVal = ioctl(spi_cs0_fd, SPI_IOC_MESSAGE(1), &spi);

    if(retVal < 0)
    {
        perror("Error - Problem transmitting spi data..ioctl");
        exit(1);
    }

    return retVal;
}
