/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 * 
 * Marvell Orion SoCs common spi driver
 * based on kirkwood_spi.c written by
 *  Prafulla Wadaskar <prafulla@marvell.com>
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
#include <malloc.h>
#include <spi.h>
#include <asm/io.h>
#include <asm/arch/config.h>

/* SPI Registers on orion SOC */
struct orionspi_registers {
	u32 ctrl;	/* 0x00 */
	u32 cfg;	/* 0x04 */
	u32 dout;	/* 0x08 */
	u32 din;	/* 0x0c */
	u32 irq_cause;	/* 0x10 */
	u32 irq_mask;	/* 0x14 */
};

#define ORIONSPI_CLKPRESCL_MASK	0x1f
#define ORIONSPI_CLKPRESCL_MIN	0x12
#define ORIONSPI_CSN_ACT	1 /* Activates serial memory interface */
#define ORIONSPI_SMEMRDY	(1 << 1) /* SerMem Data xfer ready */
#define ORIONSPI_IRQUNMASK	1 /* unmask SPI interrupt */
#define ORIONSPI_IRQMASK	0 /* mask SPI interrupt */
#define ORIONSPI_SMEMRDIRQ	1 /* SerMem data xfer ready irq */
#define ORIONSPI_XFERLEN_1BYTE	0
#define ORIONSPI_XFERLEN_2BYTE	(1 << 5)
#define ORIONSPI_XFERLEN_MASK	(1 << 5)
#define ORIONSPI_ADRLEN_1BYTE	0
#define ORIONSPI_ADRLEN_2BYTE	(1 << 8)
#define ORIONSPI_ADRLEN_3BYTE	(2 << 8)
#define ORIONSPI_ADRLEN_4BYTE	(3 << 8)
#define ORIONSPI_ADRLEN_MASK	(3 << 8)
#define ORIONSPI_TIMEOUT	10000

static struct orionspi_registers *spireg =
	(struct orionspi_registers *)ORION_SPI_BASE;

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				unsigned int max_hz, unsigned int mode)
{
	struct spi_slave *slave;
	u32 data;
	u32 kwspi_mpp_config[] = { 0, 0 };

	if (!spi_cs_is_valid(bus, cs))
		return NULL;

	slave = malloc(sizeof(struct spi_slave));
	if (!slave)
		return NULL;

	slave->bus = bus;
	slave->cs = cs;

	writel(~ORIONSPI_CSN_ACT | ORIONSPI_SMEMRDY, &spireg->ctrl);

	/* calculate spi clock prescaller using max_hz */
	data = ((CONFIG_SYS_TCLK / 2) / max_hz) + 0x10;
	data = data < ORIONSPI_CLKPRESCL_MIN ? ORIONSPI_CLKPRESCL_MIN : data;
	data = data > ORIONSPI_CLKPRESCL_MASK ? ORIONSPI_CLKPRESCL_MASK : data;

	/* program spi clock prescaller using max_hz */
	writel(ORIONSPI_ADRLEN_3BYTE | data, &spireg->cfg);
	debug("data = 0x%08x \n", data);

	writel(ORIONSPI_SMEMRDIRQ, &spireg->irq_cause);
	writel(ORIONSPI_IRQMASK, &spireg->irq_mask);

	return slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	free(slave);
}

#if defined(CONFIG_SYS_KW_SPI_MPP)
u32 spi_mpp_backup[4];
#endif

__attribute__((weak)) int board_spi_claim_bus(struct spi_slave *slave)
{
	return 0;
}

int spi_claim_bus(struct spi_slave *slave)
{
	return board_spi_claim_bus(slave);
}

__attribute__((weak)) void board_spi_release_bus(struct spi_slave *slave)
{
}

void spi_release_bus(struct spi_slave *slave)
{
	board_spi_release_bus(slave);
}

#ifndef CONFIG_SPI_CS_IS_VALID
/*
 * you can define this function board specific
 * define above CONFIG in board specific config file and
 * provide the function in board specific src file
 */
int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return (bus == 0 && (cs == 0 || cs == 1));
}
#endif

void spi_init(void)
{
}

void spi_cs_activate(struct spi_slave *slave)
{
	writel(readl(&spireg->ctrl) | ORIONSPI_IRQUNMASK, &spireg->ctrl);
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	writel(readl(&spireg->ctrl) & ORIONSPI_IRQMASK, &spireg->ctrl);
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
	unsigned int tmpdout, tmpdin;
	int tm, isread = 0;

	debug("spi_xfer: slave %u:%u dout %p din %p bitlen %u\n",
	      slave->bus, slave->cs, dout, din, bitlen);

	if (flags & SPI_XFER_BEGIN)
		spi_cs_activate(slave);

	/*
	 * handle data in 8-bit chunks
	 * TBD: 2byte xfer mode to be enabled
	 */
	writel(((readl(&spireg->cfg) & ~ORIONSPI_XFERLEN_MASK) |
		ORIONSPI_XFERLEN_1BYTE), &spireg->cfg);

	while (bitlen > 4) {
		debug("loopstart bitlen %d\n", bitlen);
		tmpdout = 0;

		/* Shift data so it's msb-justified */
		if (dout)
			tmpdout = *(u32 *) dout & 0x0ff;

		writel(~ORIONSPI_SMEMRDIRQ, &spireg->irq_cause);
		writel(tmpdout, &spireg->dout);	/* Write the data out */
		debug("*** spi_xfer: ... %08x written, bitlen %d\n",
		      tmpdout, bitlen);

		/*
		 * Wait for SPI transmit to get out
		 * or time out (1 second = 1000 ms)
		 * The NE event must be read and cleared first
		 */
		for (tm = 0, isread = 0; tm < ORIONSPI_TIMEOUT; ++tm) {
			if (readl(&spireg->irq_cause) & ORIONSPI_SMEMRDIRQ) {
				isread = 1;
				tmpdin = readl(&spireg->din);
				debug
					("spi_xfer: din %p..%08x read\n",
					din, tmpdin);

				if (din) {
					*((u8 *) din) = (u8) tmpdin;
					din += 1;
				}
				if (dout)
					dout += 1;
				bitlen -= 8;
			}
			if (isread)
				break;
		}
		if (tm >= ORIONSPI_TIMEOUT)
			printf("*** spi_xfer: Time out during SPI transfer\n");

		debug("loopend bitlen %d\n", bitlen);
	}

	if (flags & SPI_XFER_END)
		spi_cs_deactivate(slave);

	return 0;
}
