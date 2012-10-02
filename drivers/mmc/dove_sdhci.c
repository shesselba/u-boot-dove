/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * Based on linux drivers/mmc/host/sdhci-dove.c
 * by: Saeed Bishara <saeed@marvell.com>
 *     Mike Rapoport <mike@compulab.co.il>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/arch/dove.h>

static u16 dove_sdhci_readw(struct sdhci_host *host, int reg)
{
	u16 ret;

	switch (reg) {
	case SDHCI_HOST_VERSION:
	case SDHCI_SLOT_INT_STATUS:
		/* those registers don't exist */
		return 0;
	default:
		ret = readw(host->ioaddr + reg);
	}

	return ret;
}

static u32 dove_sdhci_readl(struct sdhci_host *host, int reg)
{
	u32 ret;

	switch (reg) {
	case SDHCI_CAPABILITIES:
		ret = readl(host->ioaddr + reg);
		/* Mask the support for 3.0V */
		ret &= ~SDHCI_CAN_VDD_300;
		break;
	default:
		ret = readl(host->ioaddr + reg);
	}

	return ret;
}

static struct sdhci_ops dove_sdhci_ops = {
	.read_w	= dove_sdhci_readw,
	.read_l	= dove_sdhci_readl,
};

static struct sdhci_host hosts[2] = {
	{
		.name = "Dove SDHCI0",
		.ioaddr = (void *)DOVE_SDIO0_BASE,
	},
	{
		.name = "Dove SDHCI1",
		.ioaddr = (void *)DOVE_SDIO1_BASE,
	},
};

int dove_sdhci_init(int num)
{
	struct sdhci_host *host;

	if (num < 0 || num > 1)
		return 1;

	host = &hosts[num];

	if (host->version)
		return 1;

	host->quirks =
		SDHCI_QUIRK_NO_HISPD_BIT | 
		SDHCI_QUIRK_BROKEN_R1B | 
		SDHCI_QUIRK_32BIT_DMA_ADDR;
	host->version = SDHCI_SPEC_200;
	host->ops = &dove_sdhci_ops;

	add_sdhci(host, 50000000, 4000000);
	return 0;
}
