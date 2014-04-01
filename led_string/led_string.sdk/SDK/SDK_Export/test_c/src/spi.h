/*
 * spi.h
 *
 *  Created on: Mar 29, 2014
 *      Author: DGronlund
 */

#ifndef SPI_H_
#define SPI_H_

#define SPI_BASE_ADDRESS 0x41E00000

#define SPI_RESET_OFFSET      0x40
#define SPI_CONTROL_OFFSET    0x60
#define SPI_STATUS_OFFSET     0x64
#define SPI_DATA_OUT_OFFSET   0x68
#define SPI_DATA_IN_OFFSET    0x6C
#define SPI_OUT_STATUS_OFFSET 0x74
#define SPI_IN_STATUS_OFFSET  0x78

#define SPI_MAP_SIZE 4096UL
#define SPI_MAP_MASK (SPI_MAP_SIZE - 1)

#define SPI_SUCCESS 1
#define SPI_FAILURE 0

int spi_init();
int spi_control_read();
int spi_close();
void spi_reset();
void spi_control(int value);
int spi_status();
void spi_write_data();

#endif /* SPI_H_ */
