/*
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 *
 * SolidRun CuBox config
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

#ifndef _CONFIG_CUBOX_H
#define _CONFIG_CUBOX_H
#include <asm/sizes.h>

/*
 * Version number information
 */
#define CONFIG_IDENT_STRING	"\SolidRun CuBox"

/*
 * High Level Configuration Options
 */
#define CONFIG_SHEEVA_88SV581		/* CPU Core subversion */
#define CONFIG_DOVE			/* SOC Family Name */
#define CONFIG_MACH_SOLIDRUN_CUBOX	/* Machine type */

#define CONFIG_SHOW_BOOT_PROGRESS

#define CONFIG_RANDOM_MACADDR
#define CONFIG_ORION_GPIO               /* Orion GPIO driver */
#define CONFIG_OF_LIBFDT

#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

/*
 * Enable u-boot API for standalone programs.
 */
#define CONFIG_API

/*
 * Commands configuration
 */
#include <config_cmd_default.h>
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ELF
#define CONFIG_CMD_ENV
#define CONFIG_CMD_IDE
#define CONFIG_CMD_PING
#define CONFIG_CMD_SF
#define CONFIG_CMD_SPI
#define CONFIG_CMD_USB
#define CONFIG_CMD_MMC

#define CONFIG_DOS_PARTITION
#define CONFIG_EFT_PARTITION

/*
 * kwbimage for SPI or UART
 */
#ifdef CONFIG_CUBOX_BOOT_UART
#define CONFIG_SYS_KWD_CONFIG $(SRCTREE)/$(CONFIG_BOARDDIR)/kwbimage-uart.cfg
#else
#define CONFIG_SYS_KWD_CONFIG $(SRCTREE)/$(CONFIG_BOARDDIR)/kwbimage-spi.cfg
#endif

/*
 * mv-common.h should be defined after CMD configs since it used them
 * to enable certain macros
 */
#include "mv-common.h"

#undef CONFIG_SYS_MALLOC_LEN
#define CONFIG_SYS_MALLOC_LEN	(4 * 1024 * 1024) /* 4MiB for malloc() */

/* There is no arch_misc_init on armv7 */
#undef CONFIG_ARCH_MISC_INIT

#undef CONFIG_SYS_PROMPT	/* previously defined in mv-common.h */
#define CONFIG_SYS_PROMPT	"CuBox> "	/* Command Prompt */

/*
 *  Environment variables configurations
 */
#if defined(CONFIG_CMD_SF)
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_ENV_IS_IN_SPI_FLASH	1
#define CONFIG_ENV_SECT_SIZE		0x1000	/* 4k */
#define CONFIG_ENV_SIZE			0x20000 /* 128k */
#define CONFIG_ENV_OFFSET		0xc0000 /* env starts here */
#else
#define CONFIG_ENV_IS_NOWHERE		1	/* if env in SDRAM */
#endif


/*
 * Default CuBox bootscript environment
 */
#define CONFIG_EXTRA_ENV_SETTINGS					\
        "bootscript=boot.scr\0"						\
        "loadaddr=0x02000000\0"						\
        "mmc_started=0\0"						\
        "ide_started=0\0"						\
        "usb_started=0\0"						\
	"mtdparts=mtdparts=spi0.0:768k(u-boot)ro,128k(uboot_env),128k(fdt),-(empty)\0"	\
	"ethaddr=00:50:43:15:17:17\0"
	

#define CONFIG_BOOTCOMMAND						\
        "for devn in usb mmc ide ; do "					\
	"  for part in 0 1; do "					\
	"    for dir  in / /boot/;do "					\
	"      for fs in ext2 fat; do "					\
	"        echo ===> Executing ${fs}load ${devn} 0:${part} ${loadaddr} ${dir}${bootscript};" \
	"        if itest.s $devn -eq mmc; then"			\
	"          if itest.s $mmc_started -ne 1; then"			\
	"            mmcinfo; setenv mmc_started '1';fi;fi;"		\
	"        if itest.s $devn -eq usb; then"			\
	"          if itest.s $usb_started -ne 1; then"			\
	"            usb start; setenv usb_started '1';fi;fi;"		\
	"        if itest.s $devn -eq ide; then"			\
	"          if itest.s $ide_started -ne 1; then"			\
	"            ide reset; setenv ide_started '1';fi;fi;"		\
	"        if ${fs}load ${devn} 0:${part} ${loadaddr} ${dir}${bootscript}; then" \
	"          source ${loadaddr}; fi;"				\
	"        if itest.s $devn -eq usb; then"			\
	"          echo ===> Executing ${fs}load ${devn} 1:${part} ${loadaddr} ${dir}${bootscript};" \
	"          if ${fs}load ${devn} 1:${part} ${loadaddr} ${dir}${bootscript}; then" \
	"            source ${loadaddr}; fi;"				\
        "        fi;"							\
	"      done;"							\
	"    done;"							\
	"  done;"							\
        "done;"								\
        "tftp ${loadaddr} ${bootscript};"				\
        "source ${loadaddr};"

/*
 * Ethernet Driver configuration
 */
#ifdef CONFIG_CMD_NET
#define CONFIG_MVGBE_PORTS	{1}	/* enable port 0 */
#define CONFIG_PHY_BASE_ADR	1
#define CONFIG_PHYLIB
#define CONFIG_PHY_MARVELL
#endif /* CONFIG_CMD_NET */

#ifdef CONFIG_CMD_IDE
#undef CONFIG_IDE_LED
#undef CONFIG_SYS_IDE_MAXBUS
#define CONFIG_SYS_IDE_MAXBUS		1
#undef CONFIG_SYS_IDE_MAXDEVICE
#define CONFIG_SYS_IDE_MAXDEVICE	1
#define CONFIG_SYS_ATA_IDE0_OFFSET	MV_SATA_PORT0_OFFSET
#endif

/*
 * File system
 */
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_FAT
#define CONFIG_RBTREE
#define CONFIG_MTD_DEVICE               /* needed for mtdparts commands */
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_LZO

#endif /* _CONFIG_CUBOX_H */
