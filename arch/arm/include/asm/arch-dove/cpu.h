/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Dove SoC CPU
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

#ifndef _DOVECPU_H
#define _DOVECPU_H

#include <asm/system.h>

#ifndef __ASSEMBLY__

#define DOVECPU_WIN_CTRL_DATA(size, target, attr, en) (en | (target << 4) \
			| (attr << 8) | (dove_winctrl_calcsize(size) << 16))

#define DOVEGBE_PORT_SERIAL_CONTROL1_REG	(DOVE_EGIGA_BASE + 0x44c)
#define DOVE_REG_PCIE_DEVID			(DOVE_REG_PCIE0_BASE + 0x00)
#define DOVE_REG_PCIE_REVID			(DOVE_REG_PCIE0_BASE + 0x08)
#define DOVE_REG_SYSRST_CNT			(DOVE_MPP_BASE + 0x50)
#define SYSRST_CNT_1SEC_VAL			(25*1000000)
#define DOVE_REG_MPP_OUT_DRV_REG		(DOVE_MPP_BASE + 0xE0)

enum memory_bank {
	BANK0,
	BANK1,
};

enum dovecpu_winen {
	DOVECPU_WIN_DISABLE,
	DOVECPU_WIN_ENABLE
};

enum dovecpu_target {
	DOVECPU_TARGET_DRAM = 0x0,
	DOVECPU_TARGET_SASRAM = 0x3,
	DOVECPU_TARGET_NAND = 0xc,
	DOVECPU_TARGET_PMURAM = 0xd,
	DOVECPU_TARGET_PCIE0 = 0x4,
	DOVECPU_TARGET_PCIE1 = 0x8,
	DOVECPU_TARGET_SPI = 0x1,
	DOVECPU_TARGET_BOOTROM = 0x1,
};

enum dovecpu_attrib {
	DOVECPU_ATTR_DRAM = 0x00,
	DOVECPU_ATTR_SASRAM = 0x00,
	DOVECPU_ATTR_NAND = 0x00,
	DOVECPU_ATTR_PMURAM = 0x00,
	DOVECPU_ATTR_PCIE_IO = 0xe0,
	DOVECPU_ATTR_PCIE_MEM = 0xe8,
	DOVECPU_ATTR_SPI0 = 0xfe,
	DOVECPU_ATTR_SPI1 = 0xfb,
	DOVECPU_ATTR_BOOTROM = 0xfd,
};

enum dovecpu_part {
	DOVECPU_PART_SHEEVA = 0x581,
};

enum dovesoc_devid {
	DOVESOC_DEVID_F6781 = 0x6781,
	DOVESOC_DEVID_AP510 = 0x0510,
};

enum dovesoc_revid {
	DOVESOC_REVID_Z0 = 0,
	DOVESOC_REVID_Z1 = 1,
	DOVESOC_REVID_Y0 = 2,
	DOVESOC_REVID_Y1 = 3,
	DOVESOC_REVID_X0 = 4,
	DOVESOC_REVID_A0 = 6,
	DOVESOC_REVID_A1 = 7,
};

/*
 * Default Device Address MAP BAR values
 */
#define DOVE_DEFADR_PCIE0_MEM		0xe0000000
#define DOVE_DEFADR_PCIE0_IO		0xf2000000
#define DOVE_DEFADR_PCIE0_IO_REMAP	0x00000000
#define DOVE_DEFADR_PCIE1_MEM		0xe8000000
#define DOVE_DEFADR_PCIE1_IO		0xf2100000
#define DOVE_DEFADR_PCIE1_IO_REMAP	0x00100000
#define DOVE_DEFADR_SASRAM		0xc8000000
#define DOVE_DEFADR_BOOTROM		0xf8000000
#define DOVE_DEFADR_PMURAM		0xf0000000

/*
 * ARM CPUID register
 */
#define ARM_ID_REVISION_OFFSET	0
#define ARM_ID_REVISION_MASK	0xf
#define ARM_ID_PARTNUM_OFFSET	4
#define ARM_ID_PARTNUM_MASK	0xfff
#define ARM_ID_ARCH_OFFSET	16
#define ARM_ID_ARCH_MASK	0xf
#define ARM_ID_VAR_OFFSET	20
#define ARM_ID_VAR_MASK		0xf
#define ARM_ID_ASCII_OFFSET	24
#define ARM_ID_ASCII_MASK	0xff

/*
 * read feroceon/sheeva core extra feature register
 * using co-proc instruction
 */
static inline unsigned int readfr_extra_feature_reg(void)
{
	unsigned int val;
	asm volatile ("mrc p15, 1, %0, c15, c1, 0 @ readfr exfr":"=r"
			(val)::"cc");
	return val;
}

/*
 * write feroceon/sheeva core extra feature register
 * using co-proc instruction
 */
static inline void writefr_extra_feature_reg(unsigned int val)
{
	asm volatile ("mcr p15, 1, %0, c15, c1, 0 @ writefr exfr"::"r"
			(val):"cc");
	isb();
}

/*
 * Downstream Bridge Registers
 * Ref: Datasheet sec:A.9.1
 */
struct dovewin_registers {
	u32 ctrl;
	u32 base;
	u32 remap_lo;
	u32 remap_hi;
};

/*
 * CPU control and status Registers
 * Ref: Datasheet sec:A.9.2
 */
struct dovecpu_registers {
	u32 config;		/* 0x20100 */
	u32 ctrl_stat;		/* 0x20104 */
	u32 rstoutn_mask;	/* 0x20108 */
	u32 sys_soft_rst;	/* 0x2010C */
	u32 bridge_cause_irq;	/* 0x20110 */
	u32 bridge_mask_irq;	/* 0x20114 */
	u32 pad1;
	u32 pmu_ctrl;		/* 0x2011c */
};

/*
 * GPIO 0/1 Registers
 * Ref: Datasheet sec:A.26.1
 * GPIO 2 Registers (no din/irq)
 * Ref: Datasheet sec:A.26.1
 */
struct dovegpio_registers {
	u32 dout;
	u32 oe;
	u32 blink_en;
	u32 din_pol;
	u32 din;
	u32 irq_cause;
	u32 irq_mask;
	u32 irq_level;
};

struct dove_gpio_init {
	u32 val0;
	u32 val1;
	u32 val2;
	u32 oe0_n;
	u32 oe1_n;
	u32 oe2_n;
};

/*
 * functions
 */
void reset_cpu(unsigned long ignored);
unsigned char get_random_hex(void);
u32 dove_dram_start(enum memory_bank bank);
u32 dove_dram_size(enum memory_bank bank);
int dove_config_adr_windows(void);
void dove_init_gpio(struct dove_gpio_init *);
unsigned int dove_winctrl_calcsize(unsigned int sizeval);

#endif /* __ASSEMBLY__ */
#endif /* _DOVECPU_H */
