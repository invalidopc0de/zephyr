/*
 * Copyright (c) 2020 Mark Olsson <mark@markolsson.se>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <soc.h>
#include <zephyr/drivers/kscan.h>

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

#define LOG_LEVEL LOG_LEVEL_DBG
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

const struct device *const kscan_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_keyboard_scan));
const struct gpio_dt_spec signal = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, signal_gpios);

static void k_callback(const struct device *dev, uint32_t row, uint32_t col, bool pressed)
{
	ARG_UNUSED(dev);
	if (pressed) {
		printk("row = %u col = %u\n", row, col);
	}
}

void main(void)
{
	printk("Kscan touch panel sample application\n");

	/* Configure the pin */
	gpio_pin_configure_dt(&signal, GPIO_OUTPUT_INACTIVE);
	/* Set the pin to its active level */
	gpio_pin_set_dt(&signal, 1);

	if (!device_is_ready(kscan_dev)) {
		LOG_ERR("kscan device %s not ready", kscan_dev->name);
		return;
	} else {
		LOG_INF("kscan device %s ready", kscan_dev->name);
	}

	kscan_config(kscan_dev, k_callback);
	kscan_enable_callback(kscan_dev);
}
