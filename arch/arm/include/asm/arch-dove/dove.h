/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Dove SoC register offsets and config
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

#ifndef _DOVE_H
#define _DOVE_H

/* Dove TCLK is fixed to 166MHz */
#define CONFIG_SYS_TCLK			166666667

/* SOC specific definitions */
#define DOVE_PREMAP_REGS_PHYS_BASE	0xd0000000
#define DOVE_PREMAP_INT_REGS_BASE_ADDR  (DOVE_PREMAP_REGS_PHYS_BASE + 0x20080)
#define DOVE_PREMAP_MC_DECODE_REG       (DOVE_PREMAP_REGS_PHYS_BASE + 0x800010)

#define DOVE_SB_REGS_PHYS_BASE		0xf1000000
#define DOVE_NB_REGS_PHYS_BASE		0xf1800000
#define DOVE_REGISTER(x)		(DOVE_SB_REGS_PHYS_BASE + x)

#define DOVE_AXI_CTRL_REG		(DOVE_REGISTER(0xd0224))
#define DOVE_CPU_CTRL_REG		(DOVE_REGISTER(0xd025c))
#define DOVE_MC_CTRL_REG		(DOVE_REGISTER(0xd0260))
#define DOVE_MC_DECODE_REG		(DOVE_REGISTER(0x800010))

#define DOVE_SPI_BASE			(DOVE_REGISTER(0x10600))
#define DOVE_TWSI_BASE			(DOVE_REGISTER(0x11000))
#define DOVE_UART0_BASE			(DOVE_REGISTER(0x12000))
#define DOVE_UART1_BASE			(DOVE_REGISTER(0x12100))
#define DOVE_UART2_BASE			(DOVE_REGISTER(0x12200))
#define DOVE_UART3_BASE			(DOVE_REGISTER(0x12300))
#define DOVE_CPU_WIN_BASE		(DOVE_REGISTER(0x20000))
#define DOVE_CPU_REG_BASE		(DOVE_REGISTER(0x20100))
#define DOVE_TIMER_BASE			(DOVE_REGISTER(0x20300))
#define DOVE_REG_PCIE0_BASE		(DOVE_REGISTER(0x40000))
#define DOVE_REG_PCIE1_BASE		(DOVE_REGISTER(0x80000))
#define DOVE_USB20_0_BASE		(DOVE_REGISTER(0x50000))
#define DOVE_USB20_1_BASE		(DOVE_REGISTER(0x51000))
#define DOVE_EGIGA_BASE			(DOVE_REGISTER(0x72000))
#define DOVE_SDIO1_BASE			(DOVE_REGISTER(0x90000))
#define DOVE_SDIO0_BASE			(DOVE_REGISTER(0x92000))
#define DOVE_CAMERA_BASE		(DOVE_REGISTER(0x94000))
#define DOVE_SATA_BASE			(DOVE_REGISTER(0xa0000))
#define DOVE_NANDF_BASE			(DOVE_REGISTER(0xc0000))
#define DOVE_PMU_BASE			(DOVE_REGISTER(0xd0000))
#define DOVE_MPP_BASE			(DOVE_REGISTER(0xd0200))
#define DOVE_GPIO0_BASE			(DOVE_REGISTER(0xd0400))
#define DOVE_GPIO1_BASE			(DOVE_REGISTER(0xd0420))
#define DOVE_RTC_BASE			(DOVE_REGISTER(0xd8500))
#define DOVE_AC97_BASE			(DOVE_REGISTER(0xe0000))
#define DOVE_PDMA_BASE			(DOVE_REGISTER(0xe4000))
#define DOVE_GPIO2_BASE			(DOVE_REGISTER(0xe8400))
#define DOVE_SSP_BASE			(DOVE_REGISTER(0xec000))

/* Dove Sata controller has one port */
#define DOVE_SATA_PORT0_OFFSET		0x2000

/* Dove GbE controller has one port */
#define MAX_MVGBE_DEVS			1
#define MVGBE0_BASE			DOVE_EGIGA_BASE

/* Dove USB Host controller */
#define MVUSB0_BASE			DOVE_USB20_0_BASE
#define MVUSB0_CPU_ATTR_DRAM_CS0	DOVECPU_ATTR_DRAM
#define MVUSB0_CPU_ATTR_DRAM_CS1	DOVECPU_ATTR_DRAM
#define MVUSB0_CPU_ATTR_DRAM_CS2	DOVECPU_WIN_DISABLE
#define MVUSB0_CPU_ATTR_DRAM_CS3	DOVECPU_WIN_DISABLE

/* Dove CPU memory windows */
#define MVCPU_WIN_CTRL_DATA		DOVECPU_WIN_CTRL_DATA
#define MVCPU_WIN_ENABLE		DOVECPU_WIN_ENABLE
#define MVCPU_WIN_DISABLE		DOVECPU_WIN_DISABLE

#endif
