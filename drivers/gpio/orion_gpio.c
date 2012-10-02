/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Orion SoC GPIO handling.
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

/*
 * Based on (mostly copied from) plat-orion based Linux 2.6 kernel driver.
 * Removed orion_gpiochip struct and kernel level irq handling.
 *
 * Dieter Kiermaier dk-arm-linux@gmx.de
 */

#include <common.h>
#include <asm/bitops.h>
#include <asm/io.h>
#include <asm/arch/gpio.h>
#include <orion_gpio.h>

static unsigned long gpio_valid_input[BITS_TO_LONGS(GPIO_MAX)];
static unsigned long gpio_valid_output[BITS_TO_LONGS(GPIO_MAX)];

void __set_direction(unsigned pin, int input)
{
	u32 base = GPIO_BASE(pin);
	u32 u;

	u = readl(GPIO_IO_CONF(base));
	if (input)
		u |= 1 << (pin & 31);
	else
		u &= ~(1 << (pin & 31));
	writel(u, GPIO_IO_CONF(base));

	u = readl(GPIO_IO_CONF(base));
}

void __set_level(unsigned pin, int high)
{
	u32 base = GPIO_BASE(pin);
	u32 u;

	u = readl(GPIO_OUT(base));
	if (high)
		u |= 1 << (pin & 31);
	else
		u &= ~(1 << (pin & 31));
	writel(u, GPIO_OUT(base));
}

void __set_blinking(unsigned pin, int blink)
{
	u32 base = GPIO_BASE(pin);
	u32 u;

	u = readl(GPIO_BLINK_EN(base));
	if (blink)
		u |= 1 << (pin & 31);
	else
		u &= ~(1 << (pin & 31));
	writel(u, GPIO_BLINK_EN(base));
}

int orion_gpio_is_valid(unsigned pin, int mode)
{
	if (pin < GPIO_MAX) {
		if ((mode & GPIO_INPUT_OK) && !test_bit(pin, gpio_valid_input))
			goto err_out;

		if ((mode & GPIO_OUTPUT_OK) && !test_bit(pin, gpio_valid_output))
			goto err_out;
		return 0;
	}

err_out:
	printf("%s: invalid GPIO %d/%d\n", __func__, pin, GPIO_MAX);
	return 1;
}

void orion_gpio_set_valid(unsigned pin, int mode)
{
	if (mode & GPIO_INPUT_OK)
		__set_bit(pin, gpio_valid_input);
	else
		__clear_bit(pin, gpio_valid_input);
	if (mode & GPIO_OUTPUT_OK)
		__set_bit(pin, gpio_valid_output);
	else
		__clear_bit(pin, gpio_valid_output);
}
/*
 * GENERIC_GPIO primitives.
 */
int orion_gpio_direction_input(unsigned pin)
{
	if (orion_gpio_is_valid(pin, GPIO_INPUT_OK) != 0)
		return 1;

	/* Configure GPIO direction. */
	__set_direction(pin, 1);

	return 0;
}

int orion_gpio_direction_output(unsigned pin, int value)
{
	if (orion_gpio_is_valid(pin, GPIO_OUTPUT_OK) != 0)
	{
		printf("%s: invalid GPIO %d\n", __func__, pin);
		return 1;
	}

	__set_blinking(pin, 0);

	/* Configure GPIO output value. */
	__set_level(pin, value);

	/* Configure GPIO direction. */
	__set_direction(pin, 0);

	return 0;
}

int orion_gpio_get_value(unsigned pin)
{
	u32 base = GPIO_BASE(pin);
	int val;

	if (readl(GPIO_IO_CONF(base)) & (1 << (pin & 31)))
		val = readl(GPIO_DATA_IN(base)) ^ readl(GPIO_IN_POL(base));
	else
		val = readl(GPIO_OUT(base));

	return (val >> (pin & 31)) & 1;
}

void orion_gpio_set_value(unsigned pin, int value)
{
	/* Configure GPIO output value. */
	__set_level(pin, value);
}

void orion_gpio_set_blink(unsigned pin, int blink)
{
	/* Set output value to zero. */
	__set_level(pin, 0);

	/* Set blinking. */
	__set_blinking(pin, blink);
}
