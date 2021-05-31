#ifndef __SPIDEV_HELPER_SPI_H
#define __SPIDEV_HELPER_SPI_H
#include <stdint.h>

extern uint8_t bits;
extern uint32_t speed;
extern uint16_t delay;

uint32_t SpiDrvReadWriteReg(int fd, uint16_t RegisterAddress, uint8_t Data);
void SpiDrvWriteReg(int fd, uint16_t RegisterAddress, uint8_t Data);
uint8_t SpiDrvReadReg(int fd, uint16_t RegisterAddress);

#endif
