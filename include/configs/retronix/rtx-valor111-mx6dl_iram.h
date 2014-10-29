/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __RTX_VALOR_MX6DL_CONFIG_H
#define __RTX_VALOR_MX6DL_CONFIG_H

#define CONFIG_MX6
#define CONFIG_MX6DL
#define CONFIG_MACH_TYPE	MACH_TYPE_RTX_VALOR111_MX6DL
#define CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_DCACHE_OFF
#define CONFIG_SYS_TEXT_BASE  0x00907000
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#define CONFIG_ARM_ERRATA_743622
#define CONFIG_ARM_ERRATA_751472
#define CONFIG_ARM_ERRATA_794072
#define CONFIG_ARM_ERRATA_761320
#define CONFIG_BOARD_POSTCLK_INIT

#ifndef CONFIG_SYS_L2CACHE_OFF
	#define CONFIG_SYS_L2_PL310
	#define CONFIG_SYS_PL310_BASE	L2_PL310_BASE
#endif

#ifndef CONFIG_SYS_DCACHE_OFF
	#define CONFIG_CMD_CACHE
#endif

#include <linux/sizes.h>

#define CONFIG_SYS_GENERIC_BOARD

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		   (10 * 1024)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE           UART4_BASE

//#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
	#define CONFIG_MXC_OCOTP
#endif

/* MMC Configs */
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR      0
#define CONFIG_SYS_FSL_USDHC_NUM	   3

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
//#define CONFIG_CMD_FAT
//#define CONFIG_CMD_EXT2
//#define CONFIG_CMD_EXT4
//#define CONFIG_DOS_PARTITION

#define CONFIG_CMD_FS_GENERIC

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX               1
#define CONFIG_BAUDRATE                 115200

#define CONFIG_CMD_BMODE

#define CONFIG_LOADADDR                0x10800000
//#define CONFIG_RD_LOADADDR             0x11000000
//#define CONFIG_DTB_LOADADDR            0x10F00000

#define CONFIG_BOOTARGS         "console=ttymxc0,115200"
#define CONFIG_BOOTCOMMAND      ""

/* Miscellaneous configurable options */
#define CONFIG_SYS_PROMPT		       "RTX-VALOR111 MX6DL U-Boot > "
//#define CONFIG_SYS_HUSH_PARSER
//#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE              512

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE              (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS             16
#define CONFIG_SYS_BARGSIZE            CONFIG_SYS_CBSIZE

#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

#define CONFIG_SYS_LOAD_ADDR           CONFIG_LOADADDR

#define CONFIG_CMDLINE_EDITING

#define CONFIG_STACKSIZE               (6 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS           1
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR
#define PHYS_SDRAM_SIZE		           (1u * 1024 * 1024 * 1024)

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH
#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_ENV_OFFSET      (6 * 64 * 1024)
#define CONFIG_ENV_SIZE        (8 * 1024)

#endif                         /* __RTX_VALOR_MX6DL_CONFIG_H */
