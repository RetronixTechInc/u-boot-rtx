/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __RTX_PICO_MX6Q_B10_CONFIG_H
	#define __RTX_PICO_MX6Q_B10_CONFIG_H

	#ifdef CONFIG_SPL
		#include "imx6_spl.h"
	#endif

	#define CONFIG_MACH_TYPE				3980

	#if defined(CONFIG_RTX_DEBUG_UART_PORT)
		#if CONFIG_RTX_DEBUG_UART_PORT == 0
			#define CONFIG_MXC_UART_BASE			UART1_BASE
			#define CONSOLE_DEV						"ttymxc0"
		#elif CONFIG_RTX_DEBUG_UART_PORT == 1
			#define CONFIG_MXC_UART_BASE			UART2_BASE
			#define CONSOLE_DEV						"ttymxc1"
		#elif CONFIG_RTX_DEBUG_UART_PORT == 2
			#define CONFIG_MXC_UART_BASE			UART3_BASE
			#define CONSOLE_DEV						"ttymxc2"
		#elif CONFIG_RTX_DEBUG_UART_PORT == 3
			#define CONFIG_MXC_UART_BASE			UART4_BASE
			#define CONSOLE_DEV						"ttymxc3"
		#elif CONFIG_RTX_DEBUG_UART_PORT == 4
			#define CONFIG_MXC_UART_BASE			UART5_BASE
			#define CONSOLE_DEV						"ttymxc4"
		#else
			#define CONFIG_MXC_UART_BASE			UART1_BASE
			#define CONSOLE_DEV						"ttymxc0"
		#endif
	#else
		#define CONFIG_MXC_UART_BASE			UART1_BASE
		#define CONSOLE_DEV						"ttymxc0"
	#endif

	/* */
	#if defined(CONFIG_RTX_LOADADDR)
		#define CONFIG_LOADADDR					CONFIG_RTX_LOADADDR
	#else
		#define CONFIG_LOADADDR					0x10800000
	#endif

	/* Retronix common define */
	#include "rtx_common.h"
	
	#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */

	#include "mx6_common.h"

	#define CONFIG_IMX_THERMAL

	/* Size of malloc() pool */
	#define CONFIG_SYS_MALLOC_LEN			(10 * SZ_1M)

	#define CONFIG_MXC_UART

	/* MMC Configs */
	#define CONFIG_SYS_FSL_ESDHC_ADDR		0

	#define CONFIG_FEC_MXC
	#define CONFIG_MII
	#define IMX_FEC_BASE					ENET_BASE_ADDR
	#define CONFIG_FEC_XCV_TYPE				RGMII
	#define CONFIG_ETHPRIME					"FEC"
	#define CONFIG_FEC_MXC_PHYADDR			1

	#define CONFIG_PHYLIB
	#define CONFIG_PHY_ATHEROS

	#ifdef CONFIG_CMD_SF
		#define CONFIG_MXC_SPI
		#define CONFIG_SF_DEFAULT_BUS		0
		#define CONFIG_SF_DEFAULT_CS		0
		#define CONFIG_SF_DEFAULT_SPEED		20000000
		#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
	#endif

	#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

	#include "env_common.h"
	
	#define CONFIG_ARP_TIMEOUT					200UL

	#define CONFIG_SYS_MEMTEST_START			0x10000000
	#define CONFIG_SYS_MEMTEST_END				0x10010000
	#define CONFIG_SYS_MEMTEST_SCRATCH			0x10800000

	/* Physical Memory Map */
	#define CONFIG_NR_DRAM_BANKS				1
	#define PHYS_SDRAM							MMDC0_ARB_BASE_ADDR

	#define CONFIG_SYS_SDRAM_BASE				PHYS_SDRAM
	#define CONFIG_SYS_INIT_RAM_ADDR			IRAM_BASE_ADDR
	#define CONFIG_SYS_INIT_RAM_SIZE			IRAM_SIZE

	#define CONFIG_SYS_INIT_SP_OFFSET \
		(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
	#define CONFIG_SYS_INIT_SP_ADDR \
		(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

	/* Environment organization */
	#define CONFIG_ENV_SIZE						(8 * 1024)

	#define CONFIG_ENV_IS_IN_MMC

	#if defined(CONFIG_ENV_IS_IN_MMC)
		#if defined(CONFIG_RTX_PARTITION_0_UBOOT_ARGUMENT_OFFSET) && defined(CONFIG_RTX_PARTITION_BLOCK_SIZE)
			#define CONFIG_ENV_OFFSET				(CONFIG_RTX_PARTITION_0_UBOOT_ARGUMENT_OFFSET*CONFIG_RTX_PARTITION_BLOCK_SIZE)
		#else
			#define CONFIG_ENV_OFFSET				(12288 * 1024)
		#endif
	#endif

	/* Framebuffer */
	#ifndef CONFIG_SPL_BUILD
		#ifdef CONFIG_VIDEO
			#define CONFIG_VIDEO_IPUV3
			#define CONFIG_VIDEO_BMP_RLE8
			#define CONFIG_SPLASH_SCREEN
			#define CONFIG_SPLASH_SCREEN_ALIGN
			#define CONFIG_BMP_16BPP
			#define CONFIG_VIDEO_LOGO
			#define CONFIG_VIDEO_BMP_LOGO
			#ifdef CONFIG_MX6DL
				#define CONFIG_IPUV3_CLK 				198000000
			#else
				#define CONFIG_IPUV3_CLK 				264000000
			#endif
			#define CONFIG_IMX_HDMI
			#define CONFIG_IMX_VIDEO_SKIP
		#endif
	#endif

	#ifndef CONFIG_SPL
		#define CONFIG_USBD_HS

		#define CONFIG_USB_FUNCTION_MASS_STORAGE

		#define CONFIG_USB_FUNCTION_FASTBOOT
		#define CONFIG_CMD_FASTBOOT
		#define CONFIG_ANDROID_BOOT_IMAGE
		#define CONFIG_FASTBOOT_BUF_ADDR   				CONFIG_SYS_LOAD_ADDR
		#define CONFIG_FASTBOOT_BUF_SIZE   				0x07000000
	#endif

	/* Falcon Mode */
	#define CONFIG_SPL_FS_LOAD_ARGS_NAME				"args"
	#define CONFIG_SPL_FS_LOAD_KERNEL_NAME				"uImage"
	#define CONFIG_CMD_SPL
	#define CONFIG_SYS_SPL_ARGS_ADDR					0x18000000
	#define CONFIG_CMD_SPL_WRITE_SIZE					(128 * SZ_1K)

	/* Falcon Mode - MMC support: args@1MB kernel@2MB */
	#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR  		0x800   /* 1MB */
	#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS 		(CONFIG_CMD_SPL_WRITE_SIZE / 512)
	#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR 	0x1000  /* 2MB */

	/* USDHC */
	#define CONFIG_SYS_FSL_USDHC_NUM		2
	#if defined(CONFIG_ENV_IS_IN_MMC)
		#define CONFIG_SYS_MMC_ENV_DEV		2	/* SDHC4 */
	#endif

	#define CONFIG_CMD_PCI
	#ifdef CONFIG_CMD_PCI
		#define CONFIG_PCI_SCAN_SHOW
		#define CONFIG_PCIE_IMX
		#define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(7, 12)
		/*
		#define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(3, 19)
		*/
	#endif

	/* I2C Configs */
	#define CONFIG_SYS_I2C
	#define CONFIG_SYS_I2C_MXC
	#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
	#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
	#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
	#define CONFIG_SYS_I2C_SPEED		  100000

	/* PMIC */
	#define CONFIG_POWER
	#define CONFIG_POWER_I2C
	#define CONFIG_POWER_PFUZE100
	#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08

	/* USB Configs */
	#ifdef CONFIG_CMD_USB
		#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
		#define CONFIG_USB_HOST_ETHER
		#define CONFIG_USB_ETHER_ASIX
		#define CONFIG_MXC_USB_PORTSC				(PORT_PTS_UTMI | PORT_PTS_PTW)
		#define CONFIG_MXC_USB_FLAGS				0
		#define CONFIG_USB_MAX_CONTROLLER_COUNT		2 /* Enabled USB controller number */
	#endif

#endif
