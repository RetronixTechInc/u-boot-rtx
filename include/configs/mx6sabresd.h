/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX6SABRESD_CONFIG_H
#define __MX6SABRESD_CONFIG_H

#ifdef CONFIG_SPL
#include "imx6_spl.h"
#endif

#define CONFIG_BOOT_SYSTEM
#ifdef CONFIG_BOOT_SYSTEM
	#define CONFIG_BOOT_SYSTEM_SHOW_SETTING_INFO

	#define CONFIG_BOOT_SYSTEM_SETTING_OFFSET             0x600
	#define CONFIG_BOOT_SYSTEM_SETTING_SIZE               0x2
	#define CONFIG_BOOT_SYSTEM_MAX_EXTSD                  4
#endif

#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET	0x800
#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE       	0x5400
#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET 	0x5C00
#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE   	0x400
#define CONFIG_BOOT_SYSTEM_KERNEL_DTB_OFFSET 		0x6400
#define CONFIG_BOOT_SYSTEM_KERNEL_DTB_SIZE   		0x400
#define CONFIG_BOOT_SYSTEM_KERNEL_OFFSET     		0x6800
#define CONFIG_BOOT_SYSTEM_KERNEL_SIZE       		0x5800
#define CONFIG_BOOT_SYSTEM_URAMDISK_FS_OFFSET         	0xC000
#define CONFIG_BOOT_SYSTEM_URAMDISK_FS_SIZE           	0x1000
#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET         	0xD000
#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE           	0x2000
#define CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET		0xF000
#define CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE		0x10000
#define CONFIG_BOOT_SYSTEM_LOGO_OFFSET                	0x1F000
#define CONFIG_BOOT_SYSTEM_LOGO_SIZE                  	0x1000

#ifndef CONFIG_DM_I2C
#define CONFIG_MCU_WDOG_BUS	2
#endif

#define CONFIG_MACH_TYPE	3980
#define CONFIG_MXC_UART_BASE	UART2_BASE
#define CONSOLE_DEV		"ttymxc1"
#define CONFIG_MMCROOT			"/dev/mmcblk2p2"  /* SDHC3 */

#if defined(CONFIG_MX6Q)
#define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024)
#elif defined(CONFIG_MX6DL)
#define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024)
#elif defined(CONFIG_MX6S)
#define PHYS_SDRAM_SIZE		(512u * 1024 * 1024)
#endif

#include "mx6sabre_common.h"

/* Falcon Mode */
#define CONFIG_SPL_FS_LOAD_ARGS_NAME	"args"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME	"uImage"
#define CONFIG_SYS_SPL_ARGS_ADDR       0x18000000

/* Falcon Mode - MMC support: args@1MB kernel@2MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR  0x800   /* 1MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS (CONFIG_CMD_SPL_WRITE_SIZE / 512)
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR        0x1000  /* 2MB */

#define CONFIG_SYS_FSL_USDHC_NUM	3
#if defined(CONFIG_ENV_IS_IN_MMC)
	#define CONFIG_SYS_MMC_ENV_DEV		1	/* SDHC3 */
	#ifndef CONFIG_SYS_MMC_ENV_PART
		#define CONFIG_SYS_MMC_ENV_PART                0       /* user partition */
	#endif
#endif

#ifdef CONFIG_CMD_SF
#define CONFIG_SF_DEFAULT_CS   0
#endif

/*
 * imx6 q/dl/solo pcie would be failed to work properly in kernel, if
 * the pcie module is iniialized/enumerated both in uboot and linux
 * kernel.
 * rootcause:imx6 q/dl/solo pcie don't have the reset mechanism.
 * it is only be RESET by the POR. So, the pcie module only be
 * initialized/enumerated once in one POR.
 * Set to use pcie in kernel defaultly, mask the pcie config here.
 * Remove the mask freely, if the uboot pcie functions, rather than
 * the kernel's, are required.
 */
#ifdef CONFIG_CMD_PCI
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_PCIE_IMX
#define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(7, 12)
#define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(3, 19)
#endif

/* USB Configs */
#ifdef CONFIG_CMD_USB
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	1 /* Enabled USB controller number */
#endif

/*#define CONFIG_SPLASH_SCREEN*/
/*#define CONFIG_MXC_EPDC*/

/*
 * SPLASH SCREEN Configs
 */
#if defined(CONFIG_SPLASH_SCREEN) && defined(CONFIG_MXC_EPDC)
	/*
	 * Framebuffer and LCD
	 */
	#define CONFIG_CMD_BMP
	#undef LCD_TEST_PATTERN
	/* #define CONFIG_SPLASH_IS_IN_MMC			1 */
	#define LCD_BPP					LCD_MONOCHROME
	/* #define CONFIG_SPLASH_SCREEN_ALIGN		1 */

	#define CONFIG_WAVEFORM_BUF_SIZE		0x400000
#endif /* CONFIG_SPLASH_SCREEN && CONFIG_MXC_EPDC */

#endif                         /* __MX6SABRESD_CONFIG_H */
