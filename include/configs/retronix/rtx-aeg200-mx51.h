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
#define CONFIG_VERSION_STRING "rtx-aeg200-mx51"

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_SYS_TEXT_BASE	0x97800000

#include <asm/arch/imx-regs.h>

#define CONFIG_CMDLINE_TAG			/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SYS_GENERIC_BOARD

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(10 * 1024 * 1024)

#define CONFIG_BOARD_LATE_INIT

/*
 * Hardware drivers
 */
#define CONFIG_FSL_IIM
#define CONFIG_CMD_FUSE

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONFIG_MXC_GPIO

/*
 * SPI Configs
 * */
#define CONFIG_CMD_SPI

#define CONFIG_MXC_SPI

/* PMIC Controller */
#define CONFIG_POWER
#define CONFIG_POWER_SPI
#define CONFIG_POWER_FSL
#define CONFIG_FSL_PMIC_BUS	0
#define CONFIG_FSL_PMIC_CS	0
#define CONFIG_FSL_PMIC_CLK	2500000
#define CONFIG_FSL_PMIC_MODE	(SPI_MODE_0 | SPI_CS_HIGH)
#define CONFIG_FSL_PMIC_BITLEN	32
#define CONFIG_RTC_MC13XXX

/*
 * MMC Configs
 * */
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_ESDHC_NUM	2

#define CONFIG_MMC

#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION

/*
 * Eth Configs
 */
#define CONFIG_MII

#define CONFIG_FEC_MXC
#define IMX_FEC_BASE	FEC_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR	0x1F

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET

/* USB Configs */
#define CONFIG_CMD_USB
#define CONFIG_CMD_FAT
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX5
#define CONFIG_USB_STORAGE
//#define CONFIG_USB_HOST_ETHER
//#define CONFIG_USB_ETHER_ASIX
//#define CONFIG_USB_ETHER_SMSC95XX
#define CONFIG_MXC_USB_PORT	1
#define CONFIG_MXC_USB_PORTSC	PORT_PTS_ULPI
#define CONFIG_MXC_USB_FLAGS	MXC_EHCI_POWER_PINS_ENABLED


/* Framebuffer and LCD */
/*
#define CONFIG_PREBOOT
#define CONFIG_VIDEO
#define CONFIG_VIDEO_IPUV3
#define CONFIG_CFB_CONSOLE
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_IPUV3_CLK	133000000
*/

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

/***********************************************************
 * Command definition
 ***********************************************************/

#include <config_cmd_default.h>
#define CONFIG_CMD_BMODE
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_SETEXPR
#undef  CONFIG_CMD_IMLS

#define CONFIG_CMD_DATE

#define CONFIG_BOOTDELAY	1

#define CONFIG_ETHPRIME		"FEC0"

#define CONFIG_LOADADDR		0x90800000	/* loadaddr env var */
#define CONFIG_RD_LOADADDR	0x91000000
#define CONFIG_DTB_LOADADDR 0x90F00000

#define CONFIG_EXTRA_ENV_SETTINGS \
		"hdmi=setenv bootargs ${bootargs} video=off\0" \
		"set_mem=setenv bootargs ${bootargs} gpu_nommu gpu_memory=4M\0" \
		"fecmac_val=fec_mac=fa:3a:65:c7:14:ea\0" \
		"set_display=run hdmi\0" \
		"bootargs_base=setenv bootargs console=ttymxc0,115200 ${fecmac_val}\0"\
		"bootargs_gen=setenv bootargs ${bootargs} ip=off ${root_loc} rootfstype=ext4 rootwait rw\0"\
		"r_kernel=mmc read ${loadaddr} 0x6800 0x3000\0" \
		"r_ramdisk=mmc read ${rd_loadaddr} 0x3000 0x3000\0" \
		"r_dtb=mmc read ${dtb_loadaddr} 0x9000 0x600\0" \
		"storage=mmc dev 2\0" \
		"root_loc=root=/dev/mmcblk0p1\0" \
		"bootargs_ramdisk=setenv bootargs ${bootargs} root=/dev/ram0 rootwait rw rdinit=/sbin/init\0"	\
		"bootcmd_ramdisk=run bootargs_base bootargs_ramdisk set_display set_mem ;run storage r_kernel r_ramdisk;bootm ${loadaddr} ${rd_loadaddr}\0" \
		"bootcmd_gen=run bootargs_base bootargs_gen set_display set_mem ;run storage r_kernel r_dtb; bootm ${loadaddr} - ${dtb_loadaddr}\0"	\
		"bootcmd=run bootcmd_gen\0"	\
		"version=" CONFIG_VERSION_STRING "\0"

#define CONFIG_ARP_TIMEOUT	200UL

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP				/* undef to save memory */
#define CONFIG_SYS_PROMPT		       "RTX-AEG200 MX51 U-Boot > "
#define CONFIG_SYS_HUSH_PARSER			/* use "hush" command parser */
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE				512	/* Console I/O Buffer Size */
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

#define CONFIG_SYS_DDR_CLKSEL			0
#define CONFIG_SYS_CLKTL_CBCDR			0x59E35100
#define CONFIG_SYS_MAIN_PWR_ON

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE			       (8 * 1024)

#define CONFIG_ENV_IS_IN_MMC

#if defined(CONFIG_ENV_IS_IN_MMC)
	#define CONFIG_ENV_OFFSET		   (12288 * 1024)
	#define CONFIG_DYNAMIC_MMC_DEVNO
	#define CONFIG_SYS_MMC_ENV_DEV		0	/* SDHC4 */
#endif

#define CONFIG_OF_LIBFDT

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

#endif
