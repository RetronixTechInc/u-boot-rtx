/*
 * Copyright (C) 2011 Freescale Semiconductor, Inc.
 * Jason Liu <r64343@freescale.com>
 *
 * Configuration settings for Freescale MX53 low cost board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MX53

#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_MACH_TYPE	MACH_TYPE_RTX_A53_MX53

#include <asm/arch/imx-regs.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#define CONFIG_SYS_GENERIC_BOARD

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(10 * 1024 * 1024)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO
#define CONFIG_REVISION_TAG

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE	UART1_BASE

/* MMC Configs */
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_ESDHC_NUM	2

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_FS_GENERIC

/* bootz: zImage/initrd.img support */
#define CONFIG_CMD_BOOTZ
#define CONFIG_SUPPORT_RAW_INITRD
#define CONFIG_DOS_PARTITION

/* Eth Configs */
#define CONFIG_MII

#define CONFIG_FEC_MXC
#define IMX_FEC_BASE	FEC_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR	0x1F

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET

/* I2C Configs */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC

/* PMIC Controller */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_DIALOG_POWER
#define CONFIG_POWER_FSL
#define CONFIG_POWER_FSL_MC13892
#define CONFIG_SYS_DIALOG_PMIC_I2C_ADDR	0x48
#define CONFIG_SYS_FSL_PMIC_I2C_ADDR	0x8

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

/* Command definition */
#include <config_cmd_default.h>
#define CONFIG_CMD_BOOTZ

#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY	1

#define CONFIG_ETHPRIME		"FEC0"

#define CONFIG_LOADADDR		    0x70800000	/* loadaddr env var */
#define CONFIG_RD_LOADADDR      0x70E00000
#define CONFIG_DTB_LOADADDR     0x70D00000
#define CONFIG_SYS_TEXT_BASE    0x77800000

//#define CONFIG_BOOTARGS         "console=ttymxc0,115200 rootfs=/dev/ram0 rdinit=/init rootwait video=mxcfb0:dev=mipi_dsi,TRULY-WVGA,if=RGB24"
//#define CONFIG_BOOTCOMMAND      "bootm 0x70800000 0x70E00000 0x70D00000"

#define CONFIG_BOOTARGS         "console=ttymxc0,115200 root=/dev/mmcblk0p1 init=/sbin/init rootwait rw video=mx3fb:bpp=32"
#define CONFIG_BOOTCOMMAND      "bootm 0x70800000 - 0x70D00000"

#define CONFIG_ARP_TIMEOUT	200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_PROMPT		"RTX-A53 MX53 U-Boot > "
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */

#define CONFIG_SYS_MEMTEST_START       0x70000000
#define CONFIG_SYS_MEMTEST_END         0x70010000

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_CMDLINE_EDITING

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS	2
#define PHYS_SDRAM_1			CSD0_BASE_ADDR
#define PHYS_SDRAM_1_SIZE		(gd->bd->bi_dram[0].size)
#define PHYS_SDRAM_2			CSD1_BASE_ADDR
#define PHYS_SDRAM_2_SIZE		(gd->bd->bi_dram[1].size)
#define PHYS_SDRAM_SIZE			(gd->ram_size)

#define CONFIG_SYS_SDRAM_BASE		(PHYS_SDRAM_1)
#define CONFIG_SYS_INIT_RAM_ADDR	(IRAM_BASE_ADDR)
#define CONFIG_SYS_INIT_RAM_SIZE	(IRAM_SIZE)

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH
#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_ENV_OFFSET      (6 * 64 * 1024)
#define CONFIG_ENV_SIZE        (8 * 1024)

#define CONFIG_OF_LIBFDT

#endif				/* __CONFIG_H */
