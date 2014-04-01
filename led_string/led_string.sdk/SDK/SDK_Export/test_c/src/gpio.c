/*
 * gpio.c
 *
 *  Created on: Mar 25, 2014
 *      Author: DGronlund
 */

#include "gpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

static int data = 64;
static int gpio_created = -1;
static void *mapped_base, *mapped_dev_base;

int gpio_init() {
	if (gpio_created != -1) {
		printf("GPIO device already created\n");
		return GPIO_FAILURE;
	}
	gpio_created = 0;

	off_t dev_base = GPIO_BASE_ADDRESS;

	int memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if (memfd == -1) {
		printf("Can't open /dev/mem.\n");
		return GPIO_FAILURE;
	} else {
		printf("/dev/mem opened.\n");
	}

	// Map one page of memory into user space such that the device is in that page, but it may not
	// be at the start of the page.
	mapped_base = mmap(0, GPIO_MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~GPIO_MAP_MASK);
	if (mapped_base == (void *) -1) {
		printf("Can't map the memory to user space.\n");
		exit(0);
	}
	printf("Memory mapped at address %p.\n", mapped_base);

	// get the address of the device in user space which will be an offset from the base
	// that was mapped as memory is mapped at the start of a page

	mapped_dev_base = mapped_base + (dev_base & GPIO_MAP_MASK);

	// write to the direction register so all the GPIOs are on output to drive LEDs
	*((volatile unsigned long *) (mapped_dev_base + GPIO_DIRECTION_OFFSET)) = 0;

	close(memfd);

	return GPIO_SUCCESS;
}

void gpio_write_bit(int bit, char value) {
	if (value) {
		data |= (1 << bit);
	} else {
		data &= ~(1 << bit);
	}
	gpio_write(data);
}

void gpio_write(int value) {
	data = value;
	*((volatile unsigned long *) (mapped_dev_base + GPIO_DATA_OFFSET)) = value;
}

int gpio_close() {
	if (munmap(mapped_base, GPIO_MAP_SIZE) == -1) { // unmap the memory before exiting
		printf("Can't unmap memory from user space.\n");
		return GPIO_FAILURE;
	}
	return GPIO_SUCCESS;
}
