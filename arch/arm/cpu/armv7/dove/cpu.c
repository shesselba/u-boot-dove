/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Dove SoC cpu related functions
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
#include <netdev.h>
#include <asm/cache.h>
#include <u-boot/md5.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/dove.h>
#include <hush.h>

void reset_cpu(unsigned long ignored)
{
	struct dovecpu_registers *cpureg =
	    (struct dovecpu_registers *)DOVE_CPU_REG_BASE;

	/* Enable soft reset to assert RSTOUTn */
	writel(readl(&cpureg->rstoutn_mask) | (1 << 2),
	       &cpureg->rstoutn_mask);
	/* Assert soft reset */
	writel(readl(&cpureg->sys_soft_rst) | 1,
	       &cpureg->sys_soft_rst);
	while (1) ;
}

#if defined(CONFIG_DISPLAY_CPUINFO)
int dove_print_cpu(void)
{
	char *cpu;
	u32 idreg;
	u16 part, rev;

	__asm__ __volatile__("mrc p15, 0, %0, c0, c0, 0":"=r"(idreg));

	part = (idreg >> ARM_ID_PARTNUM_OFFSET) & ARM_ID_PARTNUM_MASK;
	rev = (idreg >> ARM_ID_REVISION_OFFSET) & ARM_ID_REVISION_MASK;

	switch (part) {
	case DOVECPU_PART_SHEEVA:
		cpu = "Marvell Sheeva";
		break;
	default:
		cpu = "Unknown";
	}

	printf("CPU:   %s (rev %d)\n", cpu, rev);
	return 0;
}

int dove_print_soc(void)
{
	char *soc, *rev;
	u16 devid = (readl(DOVE_REG_PCIE_DEVID) >> 16) & 0xffff;
	u8 revid = readl(DOVE_REG_PCIE_REVID) & 0xff;

	switch (devid) {
	case DOVESOC_DEVID_AP510:
		soc = "AP510";
		break;
	case DOVESOC_DEVID_F6781:
		soc = "F6781";
		break;
	default:
		printf("ERROR.. %s: Unsupported SoC %04x\n", __FUNCTION__, devid);
		return -1;
	}
	
	switch (revid) {
	case DOVESOC_REVID_Z0:
		rev = "Z0";
		break;
	case DOVESOC_REVID_Z1:
		rev = "Z1";
		break;
	case DOVESOC_REVID_Y0:
		rev = "Y0";
		break;
	case DOVESOC_REVID_Y1:
		rev = "Y1";
		break;
	case DOVESOC_REVID_X0:
		rev = "X0";
		break;
	case DOVESOC_REVID_A0:
		rev = "A0";
		break;
	case DOVESOC_REVID_A1:
		rev = "A1";
		break;
	default:
		rev = "???";
	};

	printf("SoC:   Dove 88%s (%s)\n", soc, rev);
	return 0;
}

int print_cpuinfo(void)
{
	if (dove_print_soc())
		return -1;
	if (dove_print_cpu())
		return -1;
	return 0;
}
#endif /* CONFIG_DISPLAY_CPUINFO */

/*
 * dove_init_gpio - initial GPIO configuration
 */
void dove_init_gpio(struct dove_gpio_init* gpp)
{
	struct dovegpio_registers *gpio0reg =
		(struct dovegpio_registers *)DOVE_GPIO0_BASE;
	struct dovegpio_registers *gpio1reg =
		(struct dovegpio_registers *)DOVE_GPIO1_BASE;
	struct dovegpio_registers *gpio2reg =
		(struct dovegpio_registers *)DOVE_GPIO2_BASE;

	/* Init GPIOS to default values as per board requirement */
	writel(gpp->val0, &gpio0reg->dout);
	writel(gpp->val1, &gpio1reg->dout);
	writel(gpp->val2, &gpio2reg->dout);
	writel(gpp->oe0_n, &gpio0reg->oe);
	writel(gpp->oe1_n, &gpio1reg->oe);
	writel(gpp->oe2_n, &gpio2reg->oe);
}

/*
 * Window Size
 * Used with the Base register to set the address window size and location.
 * Must be programmed from LSB to MSB as sequence of ones followed by
 * sequence of zeros. The number of ones specifies the size of the window in
 * 64 KByte granularity (e.g., a value of 0x00FF specifies 256 = 16 MByte).
 * NOTE: A value of 0x0 specifies 64-KByte size.
 */
unsigned int dove_winctrl_calcsize(unsigned int sizeval)
{
	int i;
	unsigned int j = 0;
	u32 val = sizeval >> 1;

	for (i = 0; val >= 0x10000; i++) {
		j |= (1 << i);
		val = val >> 1;
	}
	return (0x0000ffff & j);
}

/*
 * dove_config_adr_windows - Configure address Windows
 *
 * There are 8 address windows supported by Kirkwood Soc to addess different
 * devices. Windows 0-3 can be configured for size, BAR and remap addr.
 * Windows 4-7 can be configured for size and BAR.
 * Below configuration is standard for most of the cases
 *
 * If remap function not used, remap_lo must be set as base
 *
 * Reference Documentation:
 * Mbus-L to Mbus Bridge Registers Configuration.
 * (Sec 25.1 and 25.3 of Datasheet)
 */
int dove_config_adr_windows(void)
{
	struct dovewin_registers *winregs =
		(struct dovewin_registers *)DOVE_CPU_WIN_BASE;

	/* Window 0: 1M PCIE0 IO address space */
	writel(DOVECPU_WIN_CTRL_DATA(SZ_1M, DOVECPU_TARGET_PCIE0,
	     DOVECPU_ATTR_PCIE_IO, DOVECPU_WIN_ENABLE), &winregs[0].ctrl);

	writel(DOVE_DEFADR_PCIE0_IO, &winregs[0].base);
	writel(DOVE_DEFADR_PCIE0_IO_REMAP, &winregs[0].remap_lo);
	writel(0x0, &winregs[0].remap_hi);

	/* Window 1: 1M PCIE1 IO address space */
	writel(DOVECPU_WIN_CTRL_DATA(SZ_1M, DOVECPU_TARGET_PCIE1,
	     DOVECPU_ATTR_PCIE_IO, DOVECPU_WIN_ENABLE), &winregs[1].ctrl);

	writel(DOVE_DEFADR_PCIE1_IO, &winregs[1].base);
	writel(DOVE_DEFADR_PCIE1_IO_REMAP, &winregs[1].remap_lo);
	writel(0x0, &winregs[1].remap_hi);

	/* Window 2: 128M PCIE0 MEM address space */
	writel(DOVECPU_WIN_CTRL_DATA(SZ_128M, DOVECPU_TARGET_PCIE0,
	     DOVECPU_ATTR_PCIE_MEM, DOVECPU_WIN_ENABLE), &winregs[2].ctrl);

	writel(DOVE_DEFADR_PCIE0_MEM, &winregs[2].base);
	writel(DOVE_DEFADR_PCIE0_MEM, &winregs[2].remap_lo);
	writel(0x0, &winregs[2].remap_hi);

	/* Window 3: 128M PCIE1 MEM address space */
	writel(DOVECPU_WIN_CTRL_DATA(SZ_128M, DOVECPU_TARGET_PCIE1,
	     DOVECPU_ATTR_PCIE_MEM, DOVECPU_WIN_ENABLE), &winregs[3].ctrl);

	writel(DOVE_DEFADR_PCIE1_MEM, &winregs[3].base);
	writel(DOVE_DEFADR_PCIE1_MEM, &winregs[3].remap_lo);
	writel(0x0, &winregs[3].remap_hi);

	/* Window 4: 1M Cryptographic SRAM address space */
	writel(DOVECPU_WIN_CTRL_DATA(SZ_1M, DOVECPU_TARGET_SASRAM,
	     DOVECPU_ATTR_SASRAM, DOVECPU_WIN_ENABLE), &winregs[4].ctrl);
	writel(DOVE_DEFADR_SASRAM, &winregs[4].base);

	/* Window 5: 128M Bootrom address space */
	writel(DOVECPU_WIN_CTRL_DATA(SZ_128M, DOVECPU_TARGET_BOOTROM,
	     DOVECPU_ATTR_BOOTROM, DOVECPU_WIN_ENABLE), &winregs[5].ctrl);
	writel(DOVE_DEFADR_BOOTROM, &winregs[5].base);

	/* Window 6: 1M PMU Scratchpad address space */
	writel(DOVECPU_WIN_CTRL_DATA(SZ_1M, DOVECPU_TARGET_PMURAM,
	     DOVECPU_ATTR_PMURAM, DOVECPU_WIN_ENABLE), &winregs[6].ctrl);
	writel(DOVE_DEFADR_PMURAM, &winregs[6].base);

	/* Window 7: Disabled */
	writel(DOVECPU_WIN_DISABLE, &winregs[7].ctrl);

	return 0;
}

#ifdef CONFIG_ARCH_CPU_INIT
int arch_cpu_init(void)
{
	dove_config_adr_windows();
#ifdef CONFIG_USB_EHCI_MARVELL
	dove_ehci_phy_init(0);
	dove_ehci_phy_init(1);
#endif /* CONFIG_USB_EHCI_MARVELL */
	return 0;
}
#endif /* CONFIG_ARCH_CPU_INIT */

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in lowlevel_init.S */
	dcache_enable();
}
#endif /* CONFIG_SYS_DCACHE_OFF */

#ifdef CONFIG_MVGBE
int cpu_eth_init(bd_t *bis)
{
	mvgbe_initialize(bis);
	return 0;
}
#endif /* CONFIG_MVGBE */
