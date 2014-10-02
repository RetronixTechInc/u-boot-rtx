/*
 * Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
 *
 * (C) Copyright 2009 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the MX51EVK Board
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

 /* High Level Configuration Options */

#define CONFIG_MX51	/* in a mx51 */
#define CONFIG_MACH_TYPE	MACH_TYPE_MX51_BABBAGE

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_SYS_TEXT_BASE	0x1FFE5000

#include <asm/arch/imx-regs.h>

#define CONFIG_CMDLINE_TAG			/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SYS_GENERIC_BOARD

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(5 * 1024)

#define CONFIG_BOARD_LATE_INIT

/*
 * Hardware drivers
 */
#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONFIG_MXC_GPIO

/*
 * SPI Configs
 * */

/* PMIC Controller */

/*
 * MMC Configs
 * */
//#define CONFIG_FSL_ESDHC
//#define CONFIG_SYS_FSL_ESDHC_ADDR	0
//#define CONFIG_SYS_FSL_ESDHC_NUM	2

//#define CONFIG_MMC

//#define CONFIG_CMD_MMC
//#define CONFIG_GENERIC_MMC

/*
 * Eth Configs
 */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

/***********************************************************
 * Command definition
 ***********************************************************/

//#define CONFIG_CMD_BDI		/* bdinfo			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop */
#define CONFIG_CMD_RUN		/* run command in env variable	*/
//#define CONFIG_CMD_SOURCE	/* "source" command support	*/
//#define CONFIG_CMD_XIMG		/* Load part of Multi Image	*/

#define CONFIG_BOOTDELAY				1

#define CONFIG_LOADADDR					0x90800000	/* loadaddr env var */
#define CONFIG_RD_LOADADDR				0x91000000
#define CONFIG_DTB_LOADADDR 			0x90F00000

#define CONFIG_BOOTARGS         "console=ttymxc0,115200 root=/dev/ram0 init=/sbin/init rootwait rw video=off fec_mac=fa:3a:65:c7:14:ea"
#define CONFIG_BOOTCOMMAND      "bootm 0x90800000 0x91000000"

#define CONFIG_ARP_TIMEOUT				200UL

/*
 * Miscellaneous configurable options
 */
//#define CONFIG_SYS_LONGHELP				/* undef to save memory */
#define CONFIG_SYS_PROMPT		       	"RTX-AEG200 MX51 U-Boot > "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE				256	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE 				(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS				16	/* max number of command args */
#define CONFIG_SYS_BARGSIZE 			CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */

#define CONFIG_SYS_MEMTEST_START       	0x90000000
#define CONFIG_SYS_MEMTEST_END         	0x90010000

#define CONFIG_SYS_LOAD_ADDR			CONFIG_LOADADDR

#define CONFIG_CMDLINE_EDITING

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS			1
#define PHYS_SDRAM_1					CSD0_BASE_ADDR
#define PHYS_SDRAM_1_SIZE				(512 * 1024 * 1024)

#define CONFIG_SYS_SDRAM_BASE			(PHYS_SDRAM_1)
#define CONFIG_SYS_INIT_RAM_ADDR		(IRAM_BASE_ADDR)
#define CONFIG_SYS_INIT_RAM_SIZE		(IRAM_SIZE)

#define CONFIG_BOARD_EARLY_INIT_F

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_SYS_DDR_CLKSEL	0
#define CONFIG_SYS_CLKTL_CBCDR	0x59E35100
#define CONFIG_SYS_MAIN_PWR_ON

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_OFFSET      			(6 * 64 * 1024)
#define CONFIG_ENV_SIZE        			(8 * 1024)
#define CONFIG_SYS_MMC_ENV_DEV 			0
#define CONFIG_ENV_IS_NOWHERE

#endif
