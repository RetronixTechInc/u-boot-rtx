/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 */

#ifndef __RTX_PITXP_MX6Q_CONFIG_H
#define __RTX_PITXP_MX6Q_CONFIG_H

#include <linux/sizes.h>
#include <asm/arch/imx-regs.h>
#include <asm/mach-imx/gpio.h>

#ifdef CONFIG_SPL
	#define CONFIG_SPL_LIBCOMMON_SUPPORT
	#define CONFIG_SPL_MMC_SUPPORT
	#include "imx6_spl.h"
#endif

#define CONFIG_BOOT_SYSTEM
#ifdef CONFIG_BOOT_SYSTEM
	#define CONFIG_BOOT_SYSTEM_SHOW_SETTING_INFO
	#define CONFIG_BOOT_CMD_RESET_ENV
	#define CONFIG_BOOT_CMD_RESET_SETTING

	#define CONFIG_BOOT_SYSTEM_SETTING_OFFSET             0x600
	#define CONFIG_BOOT_SYSTEM_SETTING_SIZE               0x2
	#define CONFIG_BOOT_SYSTEM_MAX_EXTSD                  4
#endif

#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET		0x800
#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE       	0x5400
#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET 	0x5C00
#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE   	0x400
#define CONFIG_BOOT_SYSTEM_KERNEL_DTB_OFFSET 			0x6400
#define CONFIG_BOOT_SYSTEM_KERNEL_DTB_SIZE   			0x400
#define CONFIG_BOOT_SYSTEM_KERNEL_OFFSET     			0x6800
#define CONFIG_BOOT_SYSTEM_KERNEL_SIZE       			0x5800
#define CONFIG_BOOT_SYSTEM_URAMDISK_FS_OFFSET         	0xC000
#define CONFIG_BOOT_SYSTEM_URAMDISK_FS_SIZE           	0x1000
#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET         	0xD000
#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE           	0x2000
#define CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET			0xF000
#define CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE			0x10000
#define CONFIG_BOOT_SYSTEM_LOGO_OFFSET                	0x1F000
#define CONFIG_BOOT_SYSTEM_LOGO_SIZE                  	0x1000
	
/*-----------------------------------------------------------------------
 * MCU watch dog
----------------------------------------------------------------------- */
#define CONFIG_RTX_EFM32
#ifdef CONFIG_RTX_EFM32
	#define CONFIG_MCU_WDOG_BUS	2
#endif

#define CONFIG_MACH_TYPE		3980
#define CONFIG_MXC_UART_BASE		UART2_BASE
#define CONFIG_CONSOLE_DEV		"ttymxc1"
#define CONFIG_MMCROOT			"/dev/mmcblk1p1"  /* SDHC3 */

#define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024)

#ifndef CONFIG_SYS_L2CACHE_OFF
	#define CONFIG_SYS_L2_PL310
	#define CONFIG_SYS_PL310_BASE	L2_PL310_BASE
#endif

#define CONFIG_BOARD_POSTCLK_INIT
#define CONFIG_MXC_GPT_HCLK

#define CONFIG_SYS_BOOTM_LEN		0x1000000

#ifndef CONFIG_MX6
	#define CONFIG_MX6
#endif

#define CONFIG_SYS_FSL_CLK

/* ATAGs */
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

/* Boot options */
#define CONFIG_LOADADDR		CONFIG_DEF_LOADADDR
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Miscellaneous configurable options */
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_MAXARGS		32
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

/* NET PHY */
#define PHY_ANEG_TIMEOUT 20000

#ifdef CONFIG_SPL_BUILD
	#define CONFIG_SPL_DRIVERS_MISC_SUPPORT
#endif

#ifdef CONFIG_IMX_OPTEE
	#define TEE_ENV "tee=yes\0"
#else
	#define TEE_ENV "tee=no\0"
#endif

#define MFG_BOOT_CMD "bootz "

#ifdef CONFIG_USB_PORT_AUTO
    #define FASTBOOT_CMD "echo \"Run fastboot ...\"; fastboot auto; "
#else
    #define FASTBOOT_CMD "echo \"Run fastboot ...\"; fastboot 0; "
#endif

/* define the nandfit partiton environment for uuu */
#define MFG_NAND_FIT_PARTITION ""

#define CONFIG_MFG_ENV_SETTINGS_DEFAULT	\
	"mfgtool_args=setenv bootargs console=${console},${baudrate} " \
	"rdinit=/linuxrc " \
	"clk_ignore_unused "\
	"\0" \
	"kboot="MFG_BOOT_CMD"\0"\
	"bootcmd_mfg=run mfgtool_args;" \
        "if iminfo ${initrd_addr}; then " \
            "if test ${tee} = yes; then " \
                "bootm ${tee_addr} ${initrd_addr} ${fdt_addr}; " \
            "else " \
                MFG_BOOT_CMD "${loadaddr} ${initrd_addr} ${fdt_addr}; " \
            "fi; " \
        "else " \
		FASTBOOT_CMD  \
        "fi;\0" \
	MFG_NAND_FIT_PARTITION \

#define CONFIG_IMX_THERMAL

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_MXC_UART

/* MMC Configs */
//#define CONFIG_FSL_ESDHC
//#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR      0

#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */

#define CONFIG_FEC_MXC
#define CONFIG_FEC_XCV_TYPE		RGMII
#define CONFIG_ETHPRIME		"eth0"

#define CONFIG_PHY_ATHEROS

#define SYS_NOSMP

#ifdef CONFIG_NAND_BOOT
	#define MFG_NAND_PARTITION "mtdparts=8000000.nor:1m(boot),-(rootfs)\\;gpmi-nand:64m(nandboot),16m(nandkernel),16m(nanddtb),16m(nandtee),-(nandrootfs)"
#else
	#define MFG_NAND_PARTITION ""
#endif

#define CONFIG_CMD_READ
#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV 0

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x12C00000\0" \
	"initrd_high=0xffffffff\0" \
	"emmc_dev=3\0"\
	"sd_dev=2\0" \
	"weim_uboot=0x08001000\0"\
	"weim_base=0x08000000\0"\
	"spi_bus=0\0"\
	"spi_uboot=0x400\0" \
	"mtdparts=" MFG_NAND_PARTITION \
	"\0"\

#ifdef CONFIG_SUPPORT_EMMC_BOOT
	#define EMMC_ENV \
		"updtb-check=test -n ${update-dtb} && ext4load mmc ${mmc_num}:${bootimage_num} ${dtb_loadaddr} /${update-dtb}\0" \
		"upkernel-check=test -n ${update-kernel} && ext4load mmc ${mmc_num}:${bootimage_num} ${loadaddr} /${update-kernel}\0" \
		"update-check=run upkernel-check && run updtb-check\0" \
		"nrdtb-check=test -n ${normal-dtb} && ext4load mmc ${mmc_num}:${bootimage_num} ${dtb_loadaddr} /${normal-dtb}\0" \
		"nrkernel-check=test -n ${normal-kernel} && ext4load mmc ${mmc_num}:${bootimage_num} ${loadaddr} /${normal-kernel}\0" \
		"normal-check=run nrkernel-check && run nrdtb-check\0" \
		"loadimage=" \
		"if run update-check; then " \
			"echo start update image; " \
			"setenv update-kernel; " \
			"setenv update-dtb; " \
			"saveenv; " \
			"setenv bootargs ${bootargs} startmode=update; " \
		"else " \
			"if run normal-check; then " \
				"echo start normal image; " \
				"setenv bootargs ${bootargs} startmode=normal; " \
			"else " \
				"run storage r_kernel r_dtb; " \
				"echo start backup image; " \
				"setenv bootargs ${bootargs} startmode=backup; " \
			"fi; " \
		"fi;\0"
#else
	#define EMMC_ENV ""
#endif

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_VERSION_STRING "rtx-pitxP-mx6q"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootcmd=run bootcmd_gen\0"	\
	"bootargs_base=setenv bootargs no_console_suspend\0" \
	"set_display=run "CONFIG_GUIPORT"\0" \
	"set_mem=setenv bootargs ${bootargs} "CONFIG_BOOTARGS_GUIMEM"\0" \
	"bootargs_console=setenv bootargs ${bootargs} console="CONFIG_CONSOLE_DEV","__stringify(CONFIG_BAUDRATE)" androidboot.console="CONFIG_CONSOLE_DEV"\0"	\
	"bootargs_gen=setenv bootargs ${bootargs} "CONFIG_BOOTARGS_GEN"\0"	\
	"hdmi=setenv bootargs ${bootargs} "CONFIG_BOOTARGS_HDMI"\0" \
	"vga=setenv bootargs ${bootargs} "CONFIG_BOOTARGS_VGA"\0" \
	"dual-hdmi=setenv bootargs ${bootargs} " CONFIG_BOOTARGS_DUAL_HDMI "\0" \
	CONFIG_BOOTARGS_UI_VAR01 "\0" \
	CONFIG_BOOTARGS_UI_VAR02 "\0" \
	CONFIG_BOOTARGS_UI_VAR03 "\0" \
	CONFIG_BOOTARGS_UI_VAR04 "\0" \
	CONFIG_BOOTARGS_UI_VAR05 "\0" \
	"mmc_num="CONFIG_UBOOT_MMCNUM"\0"	  \
	"storage=mmc dev ${mmc_num}\0" \
	"set_mac1=setenv ethaddr ${mac1_val}; setenv fec_addr ${mac1_val}\0" \
	"mac1_val="CONFIG_DEFAULT_MAC01"\0" \
	"mmcrootpath="CONFIG_MMCROOTPATH"\0" \
	"r_kernel=mmc read ${loadaddr} "__stringify(CONFIG_BOOT_SYSTEM_KERNEL_OFFSET)" "__stringify(CONFIG_BOOT_SYSTEM_KERNEL_SIZE)"\0" \
	"r_dtb=mmc read ${dtb_loadaddr} "__stringify(CONFIG_BOOT_SYSTEM_KERNEL_DTB_OFFSET)" "__stringify(CONFIG_BOOT_SYSTEM_KERNEL_DTB_SIZE)"\0"\
	"r_ramdisk=mmc read ${rd_loadaddr} "__stringify(CONFIG_BOOT_SYSTEM_URAMDISK_FS_OFFSET)" "__stringify(CONFIG_BOOT_SYSTEM_URAMDISK_FS_SIZE)"\0" \
	"bootcmd_gen=run bootargs_base set_display set_mem set_mac1 bootargs_console bootargs_android bootargs_gen;"CONFIG_EXTRA_ENV_BOOTCMD_GEN"\0" \
	"bootargs_android=setenv bootargs ${bootargs} "CONFIG_EXTRA_ENV_BOOTARGS_ANDROID"\0" \
	"splashpos=m,m\0" \
	"def_video="CONFIG_VGA_VIDEO"\0" \
	"loadaddr="__stringify(CONFIG_LOADADDR)"\0" \
	"dtb_loadaddr="__stringify(CONFIG_DTB_LOADADDR)"\0" \
	"rd_loadaddr="__stringify(CONFIG_RD_LOADADDR)"\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"startinit=init=/sbin/init\0" \
	"version="CONFIG_VERSION_STRING"\0" \
	EMMC_ENV \

#define CONFIG_ANDROID_RECOVERY_BOOTARGS \
	"setenv bootargs ${bootargs} init=/init"
#ifdef CONFIG_EXTRA_ENV_USE_DTB
	#define CONFIG_ANDROID_RECOVERY_BOOTCMD  \
		"run bootargs_base set_display set_mem bootargs_console ext_args;" \
		"mmc dev ${mmc_num};mmc read ${loadaddr} "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET)" "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE)";" \
		"mmc read ${dtb_loadaddr}  "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET)" "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE)";" \
		"mmc read ${rd_loadaddr}  "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET)" "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE)";" \
		"bootm ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}"
#else
	#define CONFIG_ANDROID_RECOVERY_BOOTCMD  \
		"run bootargs_base set_display set_mem bootargs_console ext_args;"			\
		"mmc dev ${mmc_num};mmc read ${loadaddr} "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET)" "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE) ";"\
		"mmc read ${rd_loadaddr}  "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET)" "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE)";" \
		"bootm ${loadaddr} ${rd_loadaddr}"
#endif

#define CONFIG_ENG_BOOTARGS \
	"setenv bootargs ${bootargs} root=/dev/ram0 rdinit=/sbin/init rdisk_option=${roption} storage=${rstorage} pcba_version=${version} mmcroot=" CONFIG_UPDATEROOT
#define CONFIG_ENG_BOOTCMD  \
	"run bootargs_base set_display set_mem bootargs_console ext_args; bootm "__stringify(CONFIG_LOADADDR)" " __stringify(CONFIG_RD_LOADADDR)
#define CONFIG_ENG_DTB_BOOTCMD  \
	"run bootargs_base set_display set_mem bootargs_console ext_args; bootm "__stringify(CONFIG_LOADADDR)" " __stringify(CONFIG_RD_LOADADDR)" "  __stringify(CONFIG_DTB_LOADADDR)

#define CONFIG_ARP_TIMEOUT     200UL

#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

/* Physical Memory Map */
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#ifdef CONFIG_MTD_NOR_FLASH
	#define CONFIG_SYS_FLASH_BASE           WEIM_ARB_BASE_ADDR
	#define CONFIG_SYS_FLASH_SECT_SIZE      (128 * 1024)
	#define CONFIG_SYS_MAX_FLASH_BANKS 1    /* max number of memory banks */
	#define CONFIG_SYS_MAX_FLASH_SECT 256   /* max number of sectors on one chip */
	#define CONFIG_SYS_FLASH_CFI            /* Flash memory is CFI compliant */
	#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE /* Use buffered writes*/
	#define CONFIG_SYS_FLASH_EMPTY_INFO
	#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
#endif

#ifdef CONFIG_NAND_MXS
	#define CONFIG_SYS_MAX_NAND_DEVICE     1
	#define CONFIG_SYS_NAND_BASE           0x40000000
	#define CONFIG_SYS_NAND_5_ADDR_CYCLE
	#define CONFIG_SYS_NAND_ONFI_DETECTION
	#define CONFIG_SYS_NAND_USE_FLASH_BBT
	/* DMA stuff, needed for GPMI/MXS NAND support */
#endif

#if defined(CONFIG_ENV_IS_IN_MMC)
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
	#define CONFIG_ENV_SPI_BUS             CONFIG_SF_DEFAULT_BUS
	#define CONFIG_ENV_SPI_CS              CONFIG_SF_DEFAULT_CS
	#define CONFIG_ENV_SPI_MODE            CONFIG_SF_DEFAULT_MODE
	#define CONFIG_ENV_SPI_MAX_HZ          CONFIG_SF_DEFAULT_SPEED
#elif defined(CONFIG_ENV_IS_IN_FLASH)
#elif defined(CONFIG_ENV_IS_IN_NAND)
#elif defined(CONFIG_ENV_IS_IN_SATA)
	#define CONFIG_SYS_SATA_ENV_DEV		0
#endif

/* I2C Configs */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
#define CONFIG_SYS_I2C_SPEED		  100000

/* PMIC */
#ifndef CONFIG_DM_PMIC
	#define CONFIG_POWER
	#define CONFIG_POWER_I2C
	#define CONFIG_POWER_PFUZE100
	#define CONFIG_POWER_PFUZE100_I2C_ADDR 0x08
#endif

/* Framebuffer */
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_CMD_BMP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP

#if defined(CONFIG_ANDROID_SUPPORT)
	#include "mx6sabreandroid_common.h"
#else
	#define CONFIG_USBD_HS
#endif /* CONFIG_ANDROID_SUPPORT */

/* Falcon Mode */
#define CONFIG_SPL_FS_LOAD_ARGS_NAME		"args"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME	"uImage"
#define CONFIG_SYS_SPL_ARGS_ADDR		0x18000000

/* Falcon Mode - MMC support: args@1MB kernel@2MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR  0x800   /* 1MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS (CONFIG_CMD_SPL_WRITE_SIZE / 512)
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR        0x1000  /* 2MB */

#define CONFIG_SYS_FSL_USDHC_NUM		3
#define CONFIG_DYNAMIC_MMC_DEVNO
#define CONFIG_SYS_MMC_ENV_DEV		1	/* SDHC3 */
#define CONFIG_SYS_MMC_ENV_PART		0       /* user partition */

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
/* #define CONFIG_CMD_PCI */
#ifdef CONFIG_CMD_PCI
	#define CONFIG_PCI_SCAN_SHOW
	#define CONFIG_PCIE_IMX
	#ifndef CONFIG_DM_PCI
		#define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(7, 12)
		#define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(3, 19)
	#endif
#endif

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08

/* USB Configs */
#define CONFIG_CMD_USB
#ifdef CONFIG_CMD_USB
	#define CONFIG_USB_EHCI
	#define CONFIG_USB_STORAGE
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
	#define CONFIG_LCD
	#define CONFIG_SYS_CONSOLE_IS_IN_ENV
	#undef LCD_TEST_PATTERN
	/* #define CONFIG_SPLASH_IS_IN_MMC			1 */
	#define LCD_BPP					LCD_MONOCHROME
	/* #define CONFIG_SPLASH_SCREEN_ALIGN		1 */

	#define CONFIG_WAVEFORM_BUF_SIZE		0x400000
#endif /* CONFIG_SPLASH_SCREEN && CONFIG_MXC_EPDC */

#endif /* __RTX_PITXP_MX6Q_CONFIG_H */
