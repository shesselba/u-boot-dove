/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Orion SoCs common gpio
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#ifndef __ORION_GPIO_H
#define __ORION_GPIO_H

/*
 * SoC-specific gpio.h defines 
 * GPIO_MAX and GPIO_BASE(pin) macro
 */

#define GPIO_INPUT_OK		(1 << 0)
#define GPIO_OUTPUT_OK		(1 << 1)
#define GPIO_LOW		0
#define GPIO_HIGH		1

/* got from kernel include/linux/bitops.h */
#define BITS_PER_BYTE 8
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))

#define GPIO_OUT(base)		((base) + 0x00)
#define GPIO_IO_CONF(base)	((base) + 0x04)
#define GPIO_BLINK_EN(base)	((base) + 0x08)
#define GPIO_IN_POL(base)	((base) + 0x0c)
#define GPIO_DATA_IN(base)	((base) + 0x10)
#define GPIO_EDGE_CAUSE(base)	((base) + 0x14)
#define GPIO_EDGE_MASK(base)	((base) + 0x18)
#define GPIO_LEVEL_MASK(base)	((base) + 0x1c)

/*
 * Orion-specific GPIO API
 */

void orion_gpio_set_valid(unsigned pin, int mode);
int orion_gpio_is_valid(unsigned pin, int mode);
int orion_gpio_direction_input(unsigned pin);
int orion_gpio_direction_output(unsigned pin, int value);
int orion_gpio_get_value(unsigned pin);
void orion_gpio_set_value(unsigned pin, int value);
void orion_gpio_set_blink(unsigned pin, int blink);
void orion_gpio_set_unused(unsigned pin);

#endif
