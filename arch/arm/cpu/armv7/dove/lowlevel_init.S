/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Dove SoC cache and reg base low level init
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <asm/arch/dove.h>
#include <generated/asm-offsets.h>
#include <linux/linkage.h>

/*
 * Enable Tauros2 L2 Cache
 */
.macro init_l2cc
	mrc	15, 0, r0, c1, c0, 1
	orr	r0, r0, #0x2
	mcr	15, 0, r0, c1, c0, 1
.endm /* init_l2cc */

/*
 * Init internal register base addresses
 */
.macro init_reg_base
	/* set SB reg base to 0xf1000000 */
	ldr	r1, =(DOVE_PREMAP_INT_REGS_BASE_ADDR)
	ldr	r6, =DOVE_SB_REGS_PHYS_BASE
	str	r6, [r1]

	/* set NB reg base to 0xf1800000 */
	ldr	r1, =(DOVE_CPU_CTRL_REG)
	ldr	r4, =0xffff0000
	ldr	r6, [r1]
	and	r6, r6, r4
	ldr	r4, =DOVE_NB_REGS_PHYS_BASE
	lsr	r4, r6, #16
	orr	r6, r6, r4
	str	r6, [r1]

	/* Set AXI bridge address mapping to 0xf1800000 */
	ldr	r1, =(DOVE_AXI_CTRL_REG)
	ldr	r4, =0x007fffff
	ldr	r6, [r1]
	and	r6, r6, r4
	ldr	r4, =DOVE_NB_REGS_PHYS_BASE
	orr	r6, r6, r4
	str	r6, [r1]

	/* set MC configuration register decode address to 0xf1800000 */
	ldr	r1, =(DOVE_PREMAP_MC_DECODE_REG)
	ldr	r4, =0x0000ffff
	ldr	r6, [r1]
	and	r6, r6, r4
	ldr	r4, =DOVE_NB_REGS_PHYS_BASE
	orr	r6, r6, r4
	str	r6, [r1]

.endm /* init_reg_base */

.section ".text.init", "x"

ENTRY(lowlevel_init)
	init_l2cc
	init_reg_base
	
	/* r12 saved upper lr*/
	mov pc,lr
ENDPROC(lowlevel_init)
