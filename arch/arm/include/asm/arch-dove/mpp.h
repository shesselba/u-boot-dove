/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Dove SoC pinmux
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

#ifndef _DOVE_MPP_H
#define _DOVE_MPP_H

#define DOVE_GPI	(1 << 0)
#define DOVE_GPO	(1 << 1)
#define DOVE_GPIO	(DOVE_GPO | DOVE_GPI)

#define MPP(_num, _sel, _gpio) (			 \
	/* MPP number */        ((_num) & 0xff) |	 \
	/* MPP select value */  (((_sel) & 0x1f) << 8) | \
	/* is gpio config */ 	((!!(_gpio)) << 15))

/* MPP0-15 allow PMU function */
#define MPP_PMU_MAX		15
#define PMU			(0x10)
/* MPP0-23 have standard mpp register layout */
#define MPP_STD_MAX		23

#define MPP0			0
#define MPP0_GPIO		MPP(  0, 0x0, 1)
#define MPP0_UART2_RTS		MPP(  0, 0x2, 0)
#define MPP0_SDIO0_CD		MPP(  0, 0x3, 0)
#define MPP0_LCD0_PWM		MPP(  0, 0xf, 0)
#define MPP0_PMU		MPP(  0, PMU, 0)

#define MPP1			1
#define MPP1_GPIO		MPP(  1, 0x0, 1)
#define MPP1_UART2_CTS		MPP(  1, 0x2, 0)
#define MPP1_SDIO0_WP		MPP(  1, 0x3, 0)
#define MPP1_LCD1_PWM		MPP(  1, 0xf, 0)
#define MPP1_PMU		MPP(  1, PMU, 0)

#define MPP2			2
#define MPP2_GPIO		MPP(  2, 0x0, 1)
#define MPP2_SATA_PRSNT		MPP(  2, 0x1, 0)
#define MPP2_UART2_TXD		MPP(  2, 0x2, 0)
#define MPP2_SDIO0_BUSPWR	MPP(  2, 0x3, 0)
#define MPP2_UART1_RTS		MPP(  2, 0x4, 0)
#define MPP2_PMU		MPP(  2, PMU, 0)

#define MPP3			3
#define MPP3_GPIO		MPP(  3, 0x0, 1)
#define MPP3_SATA_ACT		MPP(  3, 0x1, 0)
#define MPP3_UART2_RXD		MPP(  3, 0x2, 0)
#define MPP3_SDIO0_LEDCTRL	MPP(  3, 0x3, 0)
#define MPP3_UART1_CTS		MPP(  3, 0x4, 0)
#define MPP3_LCD_SPI_CS1	MPP(  3, 0xf, 0)
#define MPP3_PMU		MPP(  3, PMU, 0)

#define MPP4			4
#define MPP4_GPIO		MPP(  4, 0x0, 1)
#define MPP4_UART3_RTS		MPP(  4, 0x2, 0)
#define MPP4_SDIO1_CD		MPP(  4, 0x3, 0)
#define MPP4_SPI1_MISO		MPP(  4, 0x4, 0)
#define MPP4_PMU		MPP(  4, PMU, 0)

#define MPP5			5
#define MPP5_GPIO		MPP(  5, 0x0, 1)
#define MPP5_UART3_CTS		MPP(  5, 0x2, 0)
#define MPP5_SDIO1_WP		MPP(  5, 0x3, 0)
#define MPP5_SPI1_CS		MPP(  5, 0x4, 0)
#define MPP5_PMU		MPP(  5, PMU, 0)

#define MPP6			6
#define MPP6_GPIO		MPP(  6, 0x0, 1)
#define MPP6_UART3_TXD		MPP(  6, 0x2, 0)
#define MPP6_SDIO1_BUSPWR	MPP(  6, 0x3, 0)
#define MPP6_SPI1_MOSI		MPP(  6, 0x4, 0)
#define MPP6_PMU		MPP(  6, PMU, 0)

#define MPP7			7
#define MPP7_GPIO		MPP(  7, 0x0, 1)
#define MPP7_UART3_RXD		MPP(  7, 0x2, 0)
#define MPP7_SDIO1_LEDCTRL	MPP(  7, 0x3, 0)
#define MPP7_SPI1_SCK		MPP(  7, 0x4, 0)
#define MPP7_PMU		MPP(  7, PMU, 0)

#define MPP8			8
#define MPP8_GPIO		MPP(  8, 0x0, 1)
#define MPP8_WATCHDOG_RSTOUT	MPP(  8, 0x1, 0)
#define MPP8_PMU		MPP(  8, PMU, 0)

#define MPP9			9
#define MPP9_GPIO		MPP(  9, 0x0, 1)
#define MPP9_PEX1_CLKREQ	MPP(  9, 0x5, 0)
#define MPP9_PMU		MPP(  9, PMU, 0)

#define MPP10			10
#define MPP10_GPIO		MPP( 10, 0x0, 1)
#define MPP10_SSP_SCLK		MPP( 10, 0x5, 0)
#define MPP10_PMU		MPP( 10, PMU, 0)

#define MPP11			11
#define MPP11_GPIO		MPP( 11, 0x0, 1)
#define MPP11_SATA_PRSNT	MPP( 11, 0x1, 0)
#define MPP11_SATA_ACT		MPP( 11, 0x2, 0)
#define MPP11_SDIO0_LEDCTRL	MPP( 11, 0x3, 0)
#define MPP11_SDIO1_LEDCTRL	MPP( 11, 0x4, 0)
#define MPP11_PEX0_CLKREQ	MPP( 11, 0x5, 0)
#define MPP11_PMU		MPP( 11, PMU, 0)

#define MPP12			12
#define MPP12_GPIO		MPP( 12, 0x0, 1)
#define MPP12_SATA_ACT		MPP( 12, 0x1, 0)
#define MPP12_UART2_RTS		MPP( 12, 0x2, 0)
#define MPP12_AUDIO0_EXTCLK	MPP( 12, 0x3, 0)
#define MPP12_SDIO1_CD		MPP( 12, 0x4, 0)
#define MPP12_PMU		MPP( 12, PMU, 0)

#define MPP13			13
#define MPP13_GPIO		MPP( 13, 0x0, 1)
#define MPP13_UART2_CTS		MPP( 13, 0x2, 0)
#define MPP13_AUDIO1_EXTCLK	MPP( 13, 0x3, 0)
#define MPP13_SDIO1_WP		MPP( 13, 0x4, 0)
#define MPP13_SSP_EXTCLK	MPP( 13, 0x5, 0)
#define MPP13_PMU		MPP( 13, PMU, 0)

#define MPP14			14
#define MPP14_GPIO		MPP( 14, 0x0, 1)
#define MPP14_UART2_TXD		MPP( 14, 0x2, 0)
#define MPP14_SDIO1_BUSPWR	MPP( 14, 0x4, 0)
#define MPP14_SSP_TXD		MPP( 14, 0x5, 0)
#define MPP14_PMU		MPP( 14, PMU, 0)

#define MPP15			15
#define MPP15_GPIO		MPP( 15, 0x0, 1)
#define MPP15_UART2_RXD		MPP( 15, 0x2, 0)
#define MPP15_SDIO1_LEDCTRL	MPP( 15, 0x4, 0)
#define MPP15_SSP_SFRM		MPP( 15, 0x5, 0)
#define MPP15_PMU		MPP( 15, PMU, 0)

#define MPP16			16
#define MPP16_GPIO		MPP( 16, 0x0, 1)
#define MPP16_UART3_RTS		MPP( 16, 0x2, 0)
#define MPP16_SDIO0_CD		MPP( 16, 0x3, 0)
#define MPP16_LCD_SPI_CS1	MPP( 16, 0x4, 0)
#define MPP16_AC97_SDI1		MPP( 16, 0x5, 0)

#define MPP17			17
#define MPP17_GPIO		MPP( 17, 0x0, 1)
#define MPP17_AC97_SYSCLKO	MPP( 17, 0x1, 0)
#define MPP17_UART3_CTS		MPP( 17, 0x2, 0)
#define MPP17_SDIO0_WP		MPP( 17, 0x3, 0)
#define MPP17_TWSI_SDA		MPP( 17, 0x4, 0)
#define MPP17_AC97_SDI2		MPP( 17, 0x5, 0)

#define MPP18			18
#define MPP18_GPIO		MPP( 18, 0x0, 1)
#define MPP18_UART3_TXD		MPP( 18, 0x2, 0)
#define MPP18_SDIO0_BUSPWR	MPP( 18, 0x3, 0)
#define MPP18_LCD0_PWM		MPP( 18, 0x4, 0)
#define MPP18_AC97_SDI3		MPP( 18, 0x5, 0)

#define MPP19			19
#define MPP19_GPIO		MPP( 19, 0x0, 1)
#define MPP19_UART3_RXD		MPP( 19, 0x2, 0)
#define MPP19_SDIO0_LEDCTRL	MPP( 19, 0x3, 0)
#define MPP19_TWSI_SCK		MPP( 19, 0x4, 0)

#define MPP20			20
#define MPP20_GPIO		MPP( 20, 0x0, 1)
#define MPP20_AC97_SYSCLKO	MPP( 20, 0x1, 0)
#define MPP20_LCD_SPI_MISO	MPP( 20, 0x2, 0)
#define MPP20_SDIO1_CD		MPP( 20, 0x3, 0)
#define MPP20_SDIO0_CD		MPP( 20, 0x5, 0)
#define MPP20_SPI1_MISO		MPP( 20, 0x6, 0)

#define MPP21			21
#define MPP21_GPIO		MPP( 21, 0x0, 1)
#define MPP21_UART1_RTS		MPP( 21, 0x1, 0)
#define MPP21_LCD_SPI_CS0	MPP( 21, 0x2, 0)
#define MPP21_SDIO1_WP		MPP( 21, 0x3, 0)
#define MPP21_SSP_SFRM		MPP( 21, 0x4, 0)
#define MPP21_SDIO0_WP		MPP( 21, 0x5, 0)
#define MPP21_SPI1_CS		MPP( 21, 0x6, 0)

#define MPP22			22
#define MPP22_GPIO		MPP( 22, 0x0, 1)
#define MPP22_UART1_CTS		MPP( 22, 0x1, 0)
#define MPP22_LCD_SPI_MOSI	MPP( 22, 0x2, 0)
#define MPP22_SDIO1_BUSPWR	MPP( 22, 0x3, 0)
#define MPP22_SSP_TXD		MPP( 22, 0x4, 0)
#define MPP22_SDIO0_BUSPWR	MPP( 22, 0x5, 0)
#define MPP22_SPI1_MOSI		MPP( 22, 0x6, 0)

#define MPP23			23
#define MPP23_GPIO		MPP( 23, 0x0, 1)
#define MPP23_LCD_SPI_SCK	MPP( 23, 0x2, 0)
#define MPP23_SDIO1_LEDCTRL	MPP( 23, 0x3, 0)
#define MPP23_SSP_SCLK		MPP( 23, 0x4, 0)
#define MPP23_SDIO0_LEDCTRL	MPP( 23, 0x5, 0)
#define MPP23_SPI1_SCK		MPP( 23, 0x6, 0)

/* MPP_CAMERA = MPP[24:39] */
#define MPP_CAMERA		24
#define MPP_CAMERA_CAMERA	MPP( 24, 0x0, 0)
#define MPP_CAMERA_GPIO		MPP( 24, 0x1, 1)
#define MPP_CAMERA_MAX		39

/* MPP_SDIO0 = MPP[40:45] */
#define MPP_SDIO0		40
#define MPP_SDIO0_SDIO		MPP( 40, 0x0, 0)
#define MPP_SDIO0_GPIO		MPP( 40, 0x1, 1)
#define MPP_SDIO0_MAX		45

/* MPP_SDIO1 = MPP[46:51] */
#define MPP_SDIO1		46
#define MPP_SDIO1_SDIO		MPP( 46, 0x0, 0)
#define MPP_SDIO1_GPIO		MPP( 46, 0x1, 1)
#define MPP_SDIO1_MAX		51

/* MPP_AUDIO1 = MPP[52:57] */
#define MPP_AUDIO1		52
#define MPP_AUDIO1_I2S_SPDIFO	MPP( 52, 0x0, 0)
#define MPP_AUDIO1_I2S		MPP( 52, 0x2, 0)
#define MPP_AUDIO1_SPDIFO	MPP( 52, 0x8, 0)
#define MPP_AUDIO1_GPIO		MPP( 52, 0xa, 1)
#define MPP_AUDIO1_TWSI		MPP( 52, 0xb, 0)
#define MPP_AUDIO1_SSP_SPDIFO	MPP( 52, 0xc, 0)
#define MPP_AUDIO1_SSP		MPP( 52, 0xe, 0)
#define MPP_AUDIO1_SSP_TWSI	MPP( 52, 0xf, 0)
#define MPP_AUDIO1_MAX		57

/* MPP_SPI = MPP[58:61] */
#define MPP_SPI			58
#define MPP_SPI_SPI		MPP( 58, 0x0, 0)
#define MPP_SPI_GPIO		MPP( 58, 0x1, 1)
#define MPP_SPI_MAX		61

/* MPP_UART1 = MPP[62:63] */
#define MPP_UART1		62
#define MPP_UART1_UART1		MPP( 62, 0x0, 0)
#define MPP_UART1_GPIO		MPP( 62, 0x1, 1)
#define MPP_UART1_MAX		63

/* MPP_NAND = MPP[64:71] */
#define MPP_NAND		64
#define MPP_NAND_NAND		MPP( 64, 0x0, 0)
#define MPP_NAND_GPO		MPP( 64, 0x1, 1)
#define MPP_NAND_MAX		71

/* MPP_AUDIO0 = Internal AC97/I2S mux for audio0 pins */
#define MPP_AUDIO0		72
#define MPP_AUDIO0_I2S		MPP( 72, 0x0, 0)
#define MPP_AUDIO0_AC97		MPP( 72, 0x1, 0)

/* MPP_TWSI = Internal TWSI option mux */
#define MPP_TWSI		73
#define MPP_TWSI_NONE		MPP( 73, 0x0, 0)
#define MPP_TWSI_OPTION1	MPP( 73, 0x1, 0)
#define MPP_TWSI_OPTION2	MPP( 73, 0x2, 0)
#define MPP_TWSI_OPTION3	MPP( 73, 0x3, 0)

#define MPP_MAX			MPP_TWSI

u8 dove_mpp_get_gpio_caps(u8 num);
void dove_mpp_conf(u16 *mpp_list);

#endif
