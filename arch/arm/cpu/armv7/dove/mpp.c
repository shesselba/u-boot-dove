/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Dove SoC MPP pinmux
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

#include <common.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/dove.h>
#include <asm/arch/mpp.h>
#include <asm/arch/gpio.h>

#define MPP_BITS	4
#define MPP_MASK	0xf
#define MPPS_PER_REG	8

#define MPP_NUM(_x)	((_x) & 0xff)
#define MPP_SEL(_x)	(((_x) >> 8) & 0x1f)
#define MPP_GPIO(_x)	((_x) & (1 << 15))

#define MPP_CTRL(i)			(DOVE_MPP_BASE + (i* MPP_BITS))
#define MPP_PMU_GENERAL_CTRL		(DOVE_MPP_BASE + 0x010)
#define  PMUG_AUDIO0_AC97_SEL		(1 << 16)
#define MPP4_CTRL			(DOVE_MPP_BASE + 0x240)
#define  MPP4_SDIO0_GPIO_SEL		(1 << 0)
#define  MPP4_SDIO1_GPIO_SEL		(1 << 1)
#define  MPP4_CAMERA_GPIO_SEL		(1 << 2)
#define  MPP4_AUDIO1_GPIO_SEL		(1 << 3)
#define  MPP4_UART1_GPIO_SEL		(1 << 4)
#define  MPP4_SPI_GPIO_SEL		(1 << 5)
#define MPP_GENERAL_CTRL		(DOVE_PDMA_BASE + 0x403c)
#define  MPPG_NAND_GPIO_SEL		(1 << 0)
#define  MPPG_AUDIO1_SPDIFO_GPIO_SEL	(1 << 1)
#define GENERAL_CONFIG_1		(DOVE_PDMA_BASE + 0x002c)
#define  GENERAL_TWSI_MUXEN_OPTION1	(1 << 7)
#define GENERAL_CONFIG_2		(DOVE_PDMA_BASE + 0x0030)
#define  GENERAL_TWSI_OPTION3_SEL	(1 << 22)
#define  GENERAL_TWSI_MUXEN_OPTION3	(1 << 21)
#define  GENERAL_TWSI_MUXEN_OPTION2	(1 << 20)
#define SSP_CONFIG_STATUS_1		(DOVE_PDMA_BASE + 0x0034)
#define  SSP_SSP_ON_AUDIO1		(1 << 0)

/*
 * MPP0-23 have standard MPP register layout
 */
static void dove_mpp_std_set(u16 config)
{
	u8 num = MPP_NUM(config);
	u32 off = (num / MPPS_PER_REG) * MPP_BITS;
	u32 shift = (num % MPPS_PER_REG) * MPP_BITS;
	u32 reg;
	
	/* configure standard MPP pin */		
	reg  = readl(MPP_CTRL(off));
	reg &= ~(MPP_MASK << shift);
	reg |= MPP_SEL(config) << shift;
	writel(reg, MPP_CTRL(off));

	/* configure gpio capabilities */
	if (MPP_GPIO(config))
		orion_gpio_set_valid(num, GPIO_INPUT_OK | GPIO_OUTPUT_OK);
	else
		orion_gpio_set_valid(num, 0);
}

/*
 * MPP0-15 also allow to mux PMU functions
 */
static void dove_mpp_pmu_set(u16 config)
{
	u8 num = MPP_NUM(config);

	if (MPP_SEL(config) == PMU) {
		/* enable PMU on MPP */
		writel(readl(MPP_PMU_GENERAL_CTRL) | (1 << num),
		       MPP_PMU_GENERAL_CTRL);
		/* disable gpio capabilities */
		orion_gpio_set_valid(num, 0);
	} else {
		/* disable PMU on MPP */
		writel(readl(MPP_PMU_GENERAL_CTRL) & ~(1 << num),
		       MPP_PMU_GENERAL_CTRL);
		/* configure MPP */
		dove_mpp_std_set(config);
	}
}

/*
 * MPP groups on MPP4_CTRL have different register layout
 * and allow GPIO or special function only
 */
static void dove_mpp4_set(u16 config)
{
	u32 reg = readl(MPP4_CTRL);
	u32 mask;
	u8  n, nmin, nmax;
	int gpio;

	switch (MPP_NUM(config)) {
	case MPP_CAMERA:
		mask = MPP4_CAMERA_GPIO_SEL;
		nmin = MPP_CAMERA;
		nmax = MPP_CAMERA_MAX;
		break;
	case MPP_SDIO0:
		mask = MPP4_SDIO0_GPIO_SEL;
		nmin = MPP_SDIO0;
		nmax = MPP_SDIO0_MAX;
		break;
	case MPP_SDIO1:
		mask = MPP4_SDIO1_GPIO_SEL;
		nmin = MPP_SDIO1;
		nmax = MPP_SDIO1_MAX;
		break;
	case MPP_SPI:
		mask = MPP4_SPI_GPIO_SEL;
		nmin = MPP_SPI;
		nmax = MPP_SPI_MAX;
		break;
	case MPP_UART1:
		mask = MPP4_UART1_GPIO_SEL;
		nmin = MPP_UART1;
		nmax = MPP_UART1_MAX;
		break;
	default:
		return;
	}

	reg &= ~mask;
	if (MPP_SEL(config))
		reg |= mask;
	writel(reg, MPP4_CTRL);

	/* configure gpio capabilities */
	gpio = 0;
	if (MPP_GPIO(config))
		gpio = GPIO_INPUT_OK | GPIO_OUTPUT_OK;
	for (n=nmin; n <= nmax; n++)
		orion_gpio_set_valid(n, gpio);
}

/*
 * MPP_GENERAL_CTRL allows GPIO on NAND pins
 */
static void dove_mpp_nand_set(u16 config)
{
	u32 reg = readl(MPP_GENERAL_CTRL);
	u8 n;
	int gpio;

	reg &= ~MPPG_NAND_GPIO_SEL;
	if (config == MPP_NAND_GPO)
		reg |= MPPG_NAND_GPIO_SEL;
	writel(reg, MPP_GENERAL_CTRL);

	/* configure gpio capabilities */
	gpio = (config == MPP_NAND_GPO) ? GPIO_OUTPUT_OK : 0;
	for (n=MPP_NAND; n <= MPP_NAND_MAX; n++)
		orion_gpio_set_valid(n, gpio);
}

/*
 * Dedicated audio1 pins can carry i2s, spdif, ssp or twsi
 * and gpio in various combinations
 */
#define AUDIO1_TWSI	(1 << 0)
#define AUDIO1_SPDIFO	(1 << 1)
#define AUDIO1_SSP	(1 << 2)
#define AUDIO1_GPIO	(1 << 3)

static void dove_mpp_audio1_set(u16 config)
{
	u32 mpp4  = readl(MPP4_CTRL);
	u32 sspc1 = readl(SSP_CONFIG_STATUS_1);
	u32 gmpp  = readl(MPP_GENERAL_CTRL);
	u32 gcfg2 = readl(GENERAL_CONFIG_2);
	u8 n, sel = MPP_SEL(config);
	int gpio;

	if (sel & AUDIO1_TWSI)
		gcfg2 |= GENERAL_TWSI_OPTION3_SEL;
	if (sel & AUDIO1_SPDIFO)
		gmpp |= MPPG_AUDIO1_SPDIFO_GPIO_SEL;
	if (sel & AUDIO1_SSP)
		sspc1 |= SSP_SSP_ON_AUDIO1;
	if (sel & AUDIO1_GPIO)
		mpp4 |= MPP4_AUDIO1_GPIO_SEL;

	writel(mpp4, MPP4_CTRL);
	writel(sspc1, SSP_CONFIG_STATUS_1);
	writel(gmpp, MPP_GENERAL_CTRL);
	writel(gcfg2, GENERAL_CONFIG_2);

	/* configure gpio capabilities */
	/* gpio allows gpio on all audio1 mpp pins */
	gpio = 0;
	if (config == MPP_AUDIO1_GPIO)
		gpio = GPIO_INPUT_OK | GPIO_OUTPUT_OK;

	for (n=MPP_AUDIO1; n <= MPP_AUDIO1_MAX; n++)
		orion_gpio_set_valid(n, gpio);

	switch (config) {
	/* spdifo and twsi allow gpio on mpp[52:55] */
	case MPP_AUDIO1_SPDIFO:
	case MPP_AUDIO1_TWSI:
		orion_gpio_set_valid(52, GPIO_INPUT_OK | GPIO_OUTPUT_OK);
		orion_gpio_set_valid(53, GPIO_INPUT_OK | GPIO_OUTPUT_OK);
		orion_gpio_set_valid(54, GPIO_INPUT_OK | GPIO_OUTPUT_OK);
		orion_gpio_set_valid(55, GPIO_INPUT_OK | GPIO_OUTPUT_OK);
		break;
	/* i2s and ssp allow gpio on mpp[56:57] */
	case MPP_AUDIO1_I2S:
	case MPP_AUDIO1_SSP:
		orion_gpio_set_valid(56, GPIO_INPUT_OK | GPIO_OUTPUT_OK);
		orion_gpio_set_valid(57, GPIO_INPUT_OK | GPIO_OUTPUT_OK);
		break;
	}
}

/*
 * MPP PMU switches audio0 pins to ac97 or i2s0 
 */
static void dove_mpp_audio0_set(u16 config)
{
	/* switch i2s or ac97 to audio0 */
	u32 reg = readl(MPP_PMU_GENERAL_CTRL);

	reg &= ~PMUG_AUDIO0_AC97_SEL;
	if (config == MPP_AUDIO0_AC97)
		reg |= PMUG_AUDIO0_AC97_SEL;
	writel(reg, MPP_PMU_GENERAL_CTRL);
}

/*
 * TWSI has 3 optional pin sets that can be switched during runtime
 */
static void dove_mpp_twsi_set(u16 config)
{
	u32 gcfg1 = readl(GENERAL_CONFIG_1);
	u32 gcfg2 = readl(GENERAL_CONFIG_2);

	gcfg1 &= ~GENERAL_TWSI_MUXEN_OPTION1;
	gcfg2 &= ~(GENERAL_TWSI_MUXEN_OPTION2 | GENERAL_TWSI_MUXEN_OPTION3);

	switch (config) {
	case MPP_TWSI_OPTION1:
		gcfg1 |= GENERAL_TWSI_MUXEN_OPTION1;
		break;
	case MPP_TWSI_OPTION2:
		gcfg2 |= GENERAL_TWSI_MUXEN_OPTION2;
		break;
	case MPP_TWSI_OPTION3:
		gcfg2 |= GENERAL_TWSI_MUXEN_OPTION3;
		break;
	}

	writel(gcfg1, GENERAL_CONFIG_1);
	writel(gcfg2, GENERAL_CONFIG_2);
}

void dove_mpp_conf(u16 *mpp_list)
{
	while (*mpp_list) {
		u8 num = MPP_NUM(*mpp_list);

		if (num <= MPP_PMU_MAX)
			dove_mpp_pmu_set(*mpp_list);
		else if (num <= MPP_STD_MAX)
			dove_mpp_std_set(*mpp_list);
		else {
			switch (num) {
			case MPP_CAMERA:
			case MPP_SDIO0:
			case MPP_SDIO1:
			case MPP_SPI:
			case MPP_UART1:
				dove_mpp4_set(*mpp_list);
				break;
			case MPP_NAND:
				dove_mpp_nand_set(*mpp_list);
				break;
			case MPP_AUDIO0:
				dove_mpp_audio0_set(*mpp_list);
				break;
			case MPP_AUDIO1:
				dove_mpp_audio1_set(*mpp_list);
				break;
			case MPP_TWSI:
				dove_mpp_twsi_set(*mpp_list);
				break;
			}
		}
		mpp_list++;
	}
}
