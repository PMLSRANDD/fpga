/*
 * main.c
 *
 *  Created on: Mar 23, 2014
 *      Author: DGronlund
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "gpio.h"
#include "spi.h"
#include "block_ram.h"

#define NUM_LEDS 80

char oled_byte(char in) {
	return ((in & 8) << 3) | ((in & 4) << 2) | ((in & 2) << 1) | (in & 1);
}

void vdd(char value) {
	gpio_write_bit(0, value);
}

void vbat(char value) {
	gpio_write_bit(1, value);
}

void reset(char value) {
	gpio_write_bit(2, value);
}

void sclk(char value) {
	gpio_write_bit(3, value);
}

void sdata(char value) {
	gpio_write_bit(4, value);
}

void scmd(char value) {
	gpio_write_bit(5, value);
}

// LED String Pins
// CHIP
// 1   - 2
// CLK - DATA

void led_write_1(char value) {
	gpio_write_bit(7, value);
}

void led_write_2(char value) {
	gpio_write_bit(6, value);
}

void spi_delay() {
	int i;
	for (i = 0; i < 0; i++) {
	}
}

void spi_write(char data, char cmd) {
	scmd(cmd);
	sclk(0);
	int i;
	for (i = 7; i >= 0; i--) {
		sdata((data >> i) & 1);
		sclk(1);
		spi_delay();
		sclk(0);
	}
}

void led_spi_write(char data) {
	int i;
	for (i = 7; i >= 0; i--) {
		led_write_1(0);
		usleep(2);
		led_write_2((data >> i) & 1);
		usleep(2);
		led_write_1(1);
		usleep(4);
	}
}

long get_millis() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return ((time.tv_sec) * 1000 + time.tv_usec/1000.0) + 0.5;
}

int main() {
	gpio_init();
	spi_init();
	block_ram_init();

	write_register(0x4, 0x5);
	printf("Register: %x\n", read_register(0x4));

	gpio_write_bit(8, 0);
	spi_control(0x2);
	usleep(10);
	printf("SPI Control: %x\n", spi_control_read());
	printf("SPI Status: 0x%x\n", spi_status());
	spi_write_data(0x55);

	printf("Power Up Sequence Start\n");
	vdd(0);
	usleep(5000);
	reset(0);
	usleep(5000);
	reset(1);
	usleep(5000);
	vbat(0);
	printf("Power Up Sequence Done\n");

	spi_write(0xAE, 0); // Display Off
	spi_write(0x8D, 0); // Pump 1
	spi_write(0x14, 0);
	spi_write(0xD9, 0); // Precharge
	spi_write(0xF1, 0);
	spi_write(0x81, 0); // Contrast
	spi_write(0xFF, 0);
	spi_write(0xA4, 0); // Display from RAM
	spi_write(0xAF, 0); // Display On

	usleep(5000);

	spi_write(0x20, 0); // Vertical Addressing
	spi_write(0x01, 0);
	spi_write(0xFF, 1);

	int i;
	for (i = 0; i < 64 * 16; i++) {
		spi_write(oled_byte(0xff), 1);
	}

	long start;


	//gpio_write_bit(8, 0);

	led_write_1(0);
	usleep(2000);
	for (i = 0; i < NUM_LEDS; i++) {
		led_spi_write(0x00);
		led_spi_write(0x00);
		led_spi_write(0x00);
	}

	start = get_millis();
	for (i = 0; i < NUM_LEDS / 2; i++) {
		led_spi_write(0xff);
		led_spi_write(0x00);
		led_spi_write(0x00);
	}
	printf("Duration: %d\n", get_millis() - start);
	printf("Delay\n");
	//sleep(5);
	led_write_1(0);
	printf("Clk Low\n");
	//sleep(5);
	printf("Ending\n");
	//return 0;

	usleep(2000);
	while (1) {
		for (i = 0; i < NUM_LEDS / 2; i++) {
			led_spi_write(0x00);
			led_spi_write(0xff);
			led_spi_write(0x00);
		}
		usleep(2000);
		for (i = 0; i < NUM_LEDS / 2; i++) {
			led_spi_write(0x00);
			led_spi_write(0x00);
			led_spi_write(0xff);
		}
		usleep(2000);
		for (i = 0; i < NUM_LEDS / 2; i++) {
			led_spi_write(0xff);
			led_spi_write(0xff);
			led_spi_write(0xff);
		}
		usleep(2000);
	}

	return 0;
}
