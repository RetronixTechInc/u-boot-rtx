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

#define CONFIG_MACH_TYPE	MACH_TYPE_MX53_SMD

#include <asm/arch/imx-regs.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#define CONFIG_SYS_GENERIC_BOARD

#define CONFIG_BISHOP_MAGIC_PACKAGE
#define CONFIG_BOOT_SYSTEM
/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(10 * 1024 * 1024)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO
#define CONFIG_REVISION_TAG

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE	UART4_BASE_ADDR

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

#define CONFIG_LOADADDR		0x70800000	/* loadaddr env var */
#define CONFIG_RD_LOADADDR      0x70E00000
#define CONFIG_DTB_LOADADDR     0x70D00000
#define CONFIG_SYS_TEXT_BASE    0x77800000

//#define CONFIG_BOOTARGS         "console=ttymxc3,115200 rootfs=/dev/ram0 rdinit=/init rootwait video=off"
//#define CONFIG_BOOTCOMMAND      "bootm 0x70800000 0x70E00000 0x70D00000"

//#define CONFIG_BOOTARGS         "console=ttymxc3,115200 root=/dev/sda1 init=/sbin/init rootwait rw video=mx3fb:bpp=32 //fec_mac=fa:3a:65:c7:14:ea"
#define CONFIG_BOOTARGS         "console=ttymxc3,115200 root=/dev/mmcblk0p1 init=/sbin/init rootwait rw video=mx3fb:bpp=32 fec_mac=fa:3a:65:c7:14:ea"
#define CONFIG_BOOTCOMMAND      "mmc dev 1;mmc read ${loadaddr} 0x6800 0x3000; mmc read ${dtb_loadaddr} 0xA000 0x1000;bootm 0x70800000 - 0x70D00000"

#define CONFIG_ARP_TIMEOUT	200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_PROMPT		"RTX-BISHOP MX53 U-Boot > "
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
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1			CSD0_BASE_ADDR
#define PHYS_SDRAM_1_SIZE		(gd->bd->bi_dram[0].size)
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

#define CONFIG_ENV_SIZE        (8 * 1024)

#define CONFIG_ENV_IS_IN_MMC

#if defined(CONFIG_ENV_IS_IN_MMC)
	#define CONFIG_ENV_OFFSET		   (12288 * 1024)
	//#define CONFIG_DYNAMIC_MMC_DEVNO
	#define CONFIG_SYS_MMC_ENV_DEV		1	/* SDHC4 */
#endif

#define CONFIG_OF_LIBFDT

#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_MXC_I2C1_SPEED	100000	/* 100 kHz */
#define CONFIG_SYS_MXC_I2C1_SLAVE	0x0
#define CONFIG_SYS_MXC_I2C2_SPEED	100000	/* 100 kHz */
#define CONFIG_SYS_MXC_I2C2_SLAVE	0x0
#define CONFIG_SYS_MXC_I2C3_SPEED	100000	/* 100 kHz */
#define CONFIG_SYS_MXC_I2C3_SLAVE	0x0

#define CONFIG_RTX_EFM32
#define CONFIG_RTX_SET_TIMEOUT


/* USB Configs */
#define CONFIG_CMD_USB
#define CONFIG_CMD_FAT
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX5
#define CONFIG_USB_STORAGE
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_ETHER_MCS7830
#define CONFIG_USB_ETHER_SMSC95XX
#define CONFIG_MXC_USB_PORT	1
#define CONFIG_MXC_USB_PORTSC	(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS	0

#define CONFIG_BOOT_SYSTEM
#ifdef CONFIG_BOOT_SYSTEM
	#define CONFIG_BOOT_SYSTEM_PASSWORD
	//#define CONFIG_BOOT_SYSTEM_SHOW_SETTING_INFO
	#define CONFIG_BOOT_CMD_RESET_ENV
	#define CONFIG_BOOT_VIDEO_BG_LOGO
	
	#define CONFIG_BOOT_SYSTEM_MAX_EXTSD                  4
	
	#define CONFIG_BOOT_SYSTEM_SETTING_OFFSET             0x600
	#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET     0x800
	#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE       0x2800
	#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET 0x2F00
	#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE   0x100
	#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET         0x3000
	#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE           0x3000
	
	#define CONFIG_BOOT_SYSTEM_LOGO_OFFSET                0xA000
	#define CONFIG_BOOT_SYSTEM_LOGO_SIZE                  0x3000

#endif
#endif				/* __CONFIG_H */
