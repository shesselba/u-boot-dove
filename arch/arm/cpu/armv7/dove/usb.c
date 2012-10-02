/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Marvell Dove SoC USB PHY init
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

/* Dove USB2.0 PHY registers */
#define USB20_POWER_CTRL		((base) + 0x400)
#define USB20_PHY_PLL_CTRL		((base) + 0x410)
#define  PHY_PLL_VCO_RECALIBRATE	(1 << 21)
#define USB20_PHY_TX_CTRL		((base) + 0x420)
#define  PHY_TX_HS_STRESS_CTRL		(1 << 31)
#define  PHY_TX_BLOCK_EN		(1 << 21)
#define  PHY_TX_IMP_CAL_VTH(x)		(x << 14)
#define  PHY_TX_IMP_CAL_VTH_MASK	PHY_TX_IMP_CAL_VTH(0x7)
#define  PHY_TX_RCAL_START		(1 << 12)
#define  PHY_TX_LOWVDD_EN		(1 << 11)
#define USB20_PHY_RX_CTRL		((base) + 0x430)
#define  PHY_RX_EDGE_DET(x)		(x << 26)
#define  PHY_RX_EDGE_DET_MASK		PHY_RX_EDGE_DET(0x3)
#define  PHY_RX_CDR_FASTLOCK_EN		(1 << 21)
#define  PHY_RX_SQ_LENGTH(x)		(x << 15)
#define  PHY_RX_SQ_LENGTH_MASK		PHY_RX_SQ_LENGTH(0x3)
#define  PHY_RX_SQ_THRESH(x)		(x << 4)
#define  PHY_RX_SQ_THRESH_MASK		PHY_RX_SQ_THRESH(0xf)
#define  PHY_RX_LPF_COEFF(x)		(x << 2)
#define  PHY_RX_LPF_COEFF_MASK		PHY_RX_LPF_COEFF(0x3)
#define USB20_PHY_IVREF_CTRL		((base) + 0x440)
#define  PHY_IVREF_TXVDD12(x)		(x << 8)
#define  PHY_IVREF_TXVDD12_MASK		PHY_IVREF_TXVDD12(0x3)
#define USB20_PHY_TESTGRP_CTRL		((base) + 0x450)
#define  PHY_TESTGRP_SQ_RST		(1 << 15)

void dove_ehci_phy_init(int port)
{
	u32 base = (port == 0) ? DOVE_USB20_0_BASE : DOVE_USB20_1_BASE;
	u32 reg;

	/* USB PHY PLL control */
	reg = readl(USB20_PHY_PLL_CTRL);
	writel(reg | PHY_PLL_VCO_RECALIBRATE, USB20_PHY_PLL_CTRL);
	udelay(100);
	writel(reg & ~PHY_PLL_VCO_RECALIBRATE, USB20_PHY_PLL_CTRL);

	/* USB PHY Tx control */
	reg = readl(USB20_PHY_TX_CTRL);
	reg &= ~PHY_TX_IMP_CAL_VTH_MASK;
	reg |= PHY_TX_IMP_CAL_VTH(0x5);
	reg |= PHY_TX_LOWVDD_EN;
	reg |= PHY_TX_RCAL_START;
	reg |= PHY_TX_BLOCK_EN;
	reg |= PHY_TX_HS_STRESS_CTRL;
	writel(reg, USB20_PHY_TX_CTRL);
	udelay(100);
	writel(reg & ~PHY_TX_RCAL_START, USB20_PHY_TX_CTRL);

	/* USB PHY RX control */
	reg = readl(USB20_PHY_RX_CTRL);
	reg &= ~(PHY_RX_LPF_COEFF_MASK | PHY_RX_SQ_THRESH_MASK |
		 PHY_RX_SQ_LENGTH_MASK | PHY_RX_EDGE_DET_MASK);
	reg |= PHY_RX_LPF_COEFF(0x1);
	reg |= PHY_RX_SQ_THRESH(0xc);
	reg |= PHY_RX_SQ_LENGTH(0x1);
	reg |= PHY_RX_EDGE_DET(0x0);
	reg &= ~PHY_RX_CDR_FASTLOCK_EN;
	writel(reg, USB20_PHY_RX_CTRL);

	/* USB PHY IVREF control */
	reg = readl(USB20_PHY_IVREF_CTRL);
	reg &= ~PHY_IVREF_TXVDD12_MASK;
	reg |= PHY_IVREF_TXVDD12(0x3);
	writel(reg, USB20_PHY_IVREF_CTRL);
	
	/* USB PHY TEST GROUP control */
	reg = readl(USB20_PHY_TESTGRP_CTRL);
	reg &= ~PHY_TESTGRP_SQ_RST;
	writel(reg, USB20_PHY_TESTGRP_CTRL);
}
