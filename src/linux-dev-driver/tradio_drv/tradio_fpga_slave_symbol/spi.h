#ifndef __SPIDEV_HELPER_SPI_H
#define __SPIDEV_HELPER_SPI_H
#include <stdint.h>

extern uint8_t bits;
extern uint32_t speed;
extern uint16_t delay;

uint32_t SpiDrvReadWriteReg(int fd, uint16_t RegisterAddress, uint8_t Data);
void SpiDrvWriteReg(int fd, uint16_t RegisterAddress, uint8_t Data);
uint8_t SpiDrvReadReg(int fd, uint16_t RegisterAddress);
//read or write ad9361 via fpga
//fd_mem	: fd of fpga
//addr      : addr in ad9361
//val       : val to write
//read_spi return val in addr
//write_spi return 0 if write finished
int read_spi(int fd_mem, int addr);
int write_spi(int fd_mem, int addr, int val);


#endif
