/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Dove SoC DRAM initialization
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

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/dove.h>

DECLARE_GLOBAL_DATA_PTR;

#define DOVE_REG_DRAM_MAP(x)	(DOVE_REGISTER(0x800100) + (x * 0x10))
#define DRAM_START_MASK		0xff800000
#define DRAM_SIZE_MASK		0xf
#define DRAM_SIZE_SHIFT		16

/*
 * dove_sdram_start - reads start from Memory Address Map Register n
 */
u32 dove_dram_start(enum memory_bank bank)
{
	u32 reg = readl(DOVE_REG_DRAM_MAP(bank));
	u32 enable = 0x01 & reg;

	if ((!enable) || (bank > BANK1))
		return 0;

	return reg & DRAM_START_MASK;
}

/*
 * dove_sdram_size - reads size from Memory Address Map Register n
 */
u32 dove_dram_size(enum memory_bank bank)
{
	u32 reg = readl(DOVE_REG_DRAM_MAP(bank));
	u32 enable = 0x01 & reg;
	u32 size;

	if ((!enable) || (bank > BANK1))
		return 0;

	/*
	 * area_length: 7 = 8M, 8 = 16M, ..., 15 = 2048M
	 * size = 1 << (area_length + 16)
	 */
	size = (reg >> DRAM_SIZE_SHIFT) & DRAM_SIZE_MASK;
	size = 1 << (size + 16);
	return size;
}

#ifndef CONFIG_SYS_BOARD_DRAM_INIT
int dram_init(void)
{
	int i;

	gd->ram_size = 0;
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		gd->bd->bi_dram[i].start = dove_dram_start(i);
		gd->bd->bi_dram[i].size = dove_dram_size(i);

		/*
		 * It is assumed that all memory banks are consecutive
		 * and without gaps.
		 * If the gap is found, ram_size will be reported for
		 * consecutive memory only
		 */
		if (gd->bd->bi_dram[i].start != gd->ram_size)
			break;

		gd->ram_size += gd->bd->bi_dram[i].size;

	}

	for (; i < CONFIG_NR_DRAM_BANKS; i++) {
		/* If above loop terminated prematurely, we need to set
		 * remaining banks' start address & size as 0. Otherwise other
		 * u-boot functions and Linux kernel gets wrong values which
		 * could result in crash */
		gd->bd->bi_dram[i].start = 0;
		gd->bd->bi_dram[i].size = 0;
	}

	return 0;
}

/*
 * If this function is not defined here,
 * board.c alters dram bank zero configuration defined above.
 */
void dram_init_banksize(void)
{
	dram_init();
}
#endif /* CONFIG_SYS_BOARD_DRAM_INIT */
