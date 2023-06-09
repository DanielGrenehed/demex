/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#define DMX_BAUD 250000

const int uart_buffer_size = (1024 * 2);
QueueHandle_t uart_queue;

void set_dmx_io(const uart_port_t uart, int rx_pin, int tx_pin) {
	uart_config_t uart_config = {
		.baud_rate = DMX_BAUD,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_2,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_DEFAULT,
	};
	ESP_ERROR_CHECK(uart_param_config(uart, &uart_config));
	ESP_ERROR_CHECK(uart_set_pin(uart, tx_pin, rx_pin, -1, -1));
	ESP_ERROR_CHECK(uart_driver_install(uart, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));
}

void print_hw_info() {
 	/* Print chip information */
	esp_chip_info_t chip_info;
	uint32_t flash_size;
    	esp_chip_info(&chip_info);
    	printf("This is %s chip with %d CPU core(s), WiFi%s%s%s, ",
        	   CONFIG_IDF_TARGET,
           	chip_info.cores,
           	(chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           	(chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "",
           	(chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    	unsigned major_rev = chip_info.revision / 100;
    	unsigned minor_rev = chip_info.revision % 100;
    	printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    	if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        	printf("Get flash size failed");
        	return;
    	}

    	printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           	(chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    	printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
	fflush(stdout);
}

void app_main(void) {
	print_hw_info();
	const uart_port_t dmx_ser = UART_NUM_2;
	set_dmx_io(dmx_ser, 19, 18);
	gpio_pulldown_en(4); // set RE & DE on MAX485
	printf("Config done!\n");
}
