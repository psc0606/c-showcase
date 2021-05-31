#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>


#include "../spi.h"


uint8_t bits;
uint32_t speed = 1000000;
uint16_t delay;

uint32_t SpiDrvReadWriteReg(int fd, uint16_t RegisterAddress, uint8_t Data)
{
        int ret;
        uint8_t tx[3] = {(uint8_t)(RegisterAddress >> 8),
                         (uint8_t)(RegisterAddress & 0xFF),
                         Data
                        };
        uint8_t rx[3] = {0, };
        struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx,
                .rx_buf = (unsigned long)rx,
                .len = 3,
                .delay_usecs = delay,
                .speed_hz = speed,
                .bits_per_word = bits,
        };

        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if (ret < 1) {
                perror("can't send spi message");
                abort();
        }

#ifdef DEBUG
        for (ret = 0; ret < 3; ret++) {
                if (!(ret % 6))
                        puts("");
                printf("%.2X ", rx[ret]);
        }
        puts("");
#endif
        return rx[2];
}

/**
 * @brief Function to write a basic SPI write operation common to many SPI based chips
 * To write a 16 bit address and do an 8 bit read where the most significant bit of the 16
 * bit address is 1 for doing a write operation and 0 for doing a read
 *
 * @param fd              [in] - SPI device opened file descriptor
 * @param RegisterAddress [in] - 16 bit register address to send on write command (highest bit set to indicate write operation to device)
 * @param Data            [in] - 8 bit data to send after register  
 *
 * @return -  void return
 *
 */
void SpiDrvWriteReg(int fd, uint16_t RegisterAddress, uint8_t Data)
{

	(void)SpiDrvReadWriteReg(
			fd, 
			RegisterAddress | 0x8000, 
			Data
		);
}

/**
 * @brief Function to write a basic SPI read or write operation common to many SPI based chips
 * To write a 16 bit address and do an 8 bit read
 *
 * @param fd			  [in] - SPI device opened file descriptor
 * @param RegisterAddress [in] - 16 bit register address to send on read command
 *
 * @return - Returns 8 bit read from SPI that occurs at the same time that 3rd byte (dummy byte) is written
 */
uint8_t SpiDrvReadReg(int fd, uint16_t RegisterAddress)
{
	uint8_t SpiDataReadFromBus;

	SpiDataReadFromBus = 
		SpiDrvReadWriteReg(fd, 
				   RegisterAddress, 
				   0
				  );
	return (SpiDataReadFromBus);
}
