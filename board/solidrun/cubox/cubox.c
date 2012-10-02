/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * SolidRun CuBox board support
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
#include <miiphy.h>
#include <asm/arch/cpu.h>
#include <asm/arch/dove.h>
#include <asm/arch/mpp.h>
#include <orion_gpio.h>

DECLARE_GLOBAL_DATA_PTR;

static u16 mpp_config[] = {
	MPP0_GPIO,
	MPP1_GPIO, /* USB power enable */
	MPP2_GPIO, /* USB over-current indication */
	MPP3_GPIO, /* micro button beneath eSATA port */
	MPP4_GPIO,
	MPP5_GPIO,
	MPP6_GPIO,
	MPP7_GPIO,

	MPP8_GPIO,
	MPP9_GPIO,
	MPP10_GPIO,
	MPP11_GPIO,
	MPP12_GPIO, /* SDIO0 card detect */
	MPP13_AUDIO1_EXTCLK, /* Si5351a audio clock output */
	MPP14_GPIO,
	MPP15_GPIO,

	MPP16_GPIO,
	MPP17_GPIO,
	MPP18_GPIO, /* Red front LED */
	MPP19_UART3_RXD, /* IR sensor */
	MPP20_GPIO,
	MPP21_GPIO,
	MPP22_GPIO,
	MPP23_GPIO,

	MPP_CAMERA_GPIO,
	MPP_SDIO0_SDIO, /* SDIO0 */
	MPP_SDIO1_GPIO,
	MPP_AUDIO1_I2S_SPDIFO, /* SPDIF and HDMI audio */
	MPP_SPI_SPI, /* SPI */
	MPP_UART1_GPIO,
	MPP_NAND_GPO,

	MPP_AUDIO0_I2S,
	MPP_TWSI_OPTION1, /* TWSI on dedicated pins */
	0 };

int board_early_init_f(void)
{
	struct dove_gpio_init gpp = {
		.val0  = 0x00010186,
		.oe0_n = 0xffffffff,
		.val1  = 0x018000c0,
		.oe1_n = 0xffffffff,
		.val2  = 0x00000000,
		.oe2_n = 0xffffffff,
	};

	dove_init_gpio(&gpp); 

	return 0;
}

int board_init(void)
{
	/* adress of boot parameters */
	gd->bd->bi_boot_params = dove_dram_start(0) + 0x100;

	/* configure mpp */
	dove_mpp_conf(mpp_config);

	/* usb power enable */
	orion_gpio_direction_output(MPP1, GPIO_HIGH);

	/* blink led */
	orion_gpio_direction_output(MPP18, GPIO_HIGH);
	orion_gpio_set_blink(MPP18, 1);

	return 0;
}

#ifdef CONFIG_MMC
int board_mmc_init(bd_t *bis)
{
	dove_sdhci_init(0);
	return 0;
}
#endif

#ifdef CONFIG_RESET_PHY_R
void dove_eth_phy_init(char *name)
{
	u16 devadr;

	if (miiphy_set_current_dev(name))
		return;

	/* command to read PHY dev address */
	if (miiphy_read(name, 0xEE, 0xEE, (u16 *) &devadr)) {
		printf("Err..%s could not read PHY dev address\n",
			__FUNCTION__);
		return;
	}

	/* reset the phy */
	miiphy_reset(name, devadr);

	printf("%s PHY initialized\n", name);
}

void reset_phy(void)
{
	dove_eth_phy_init("egiga0");
}
#endif /* CONFIG_RESET_PHY_R */
