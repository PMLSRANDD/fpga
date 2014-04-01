/*
 * gpio.h
 *
 *  Created on: Mar 26, 2014
 *      Author: DGronlund
 */

#ifndef GPIO_H_
#define GPIO_H_

#define GPIO_BASE_ADDRESS     0x41200000

#define GPIO_DATA_OFFSET      0
#define GPIO_DIRECTION_OFFSET 4

#define GPIO_MAP_SIZE 4096UL
#define GPIO_MAP_MASK (GPIO_MAP_SIZE - 1)

#define GPIO_SUCCESS 1
#define GPIO_FAILURE 0

int gpio_init();
void gpio_write(int value);
int gpio_close();
void gpio_write_bit(int bit, char value);

#endif /* GPIO_H_ */
