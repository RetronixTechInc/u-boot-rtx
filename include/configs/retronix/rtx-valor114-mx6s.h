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
#define CONFIG_VERSION_STRING "TRIPAS-mx6dl"
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

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

#define CONFIG_SYS_GENERIC_BOARD

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		   (10 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE           UART4_BASE

#define CONFIG_CMD_FUSE
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
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_DOS_PARTITION

#define CONFIG_CMD_FS_GENERIC

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX               1
#define CONFIG_BAUDRATE                 115200

/* Command definition */
#include <config_cmd_default.h>

#define CONFIG_CMD_BMODE
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_SETEXPR
#undef  CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY               1

#define CONFIG_LOADADDR                0x10800000
#define CONFIG_RD_LOADADDR             0x11000000
#define CONFIG_DTB_LOADADDR            0x10F00000
#define CONFIG_SYS_TEXT_BASE           0x27800000

#define CONFIG_EXTRA_ENV_SETTINGS \
		"no_video=setenv bootargs ${bootargs} video=off\0" \
		"set_mem=setenv bootargs ${bootargs} gpu_nommu gpumem=16M\0" \
		"set_display=run no_video\0" \
		"bootargs_base=setenv bootargs console=ttymxc3,115200\0"\
		"bootargs_gen=setenv bootargs ${bootargs} ip=off ${root_loc} rootfstype=ext4 rootwait rw\0"\
		"r_kernel=mmc read ${loadaddr} 0x6800 0x3000\0" \
		"r_ramdisk=mmc read ${rd_loadaddr} 0x3000 0x3000\0" \
		"r_dtb=mmc read ${dtb_loadaddr} 0x9000 0x600\0" \
		"storage=mmc dev 2\0" \
		"root_loc=root=/dev/mmcblk0p1\0" \
		"bootargs_ramdisk=setenv bootargs ${bootargs} root=/dev/ram0 rootwait rw rdinit=/sbin/init\0"	\
		"bootcmd_ramdisk=run bootargs_base bootargs_ramdisk set_display set_mem ;run storage r_kernel r_ramdisk;bootm ${loadaddr} ${rd_loadaddr}\0" \
		"bootcmd_gen=run bootargs_base bootargs_gen set_display set_mem ;run storage r_kernel; bootm ${loadaddr}\0"	\
		"bootcmd=run bootcmd_gen\0"	\
		"version=" CONFIG_VERSION_STRING "\0"

#define CONFIG_ARP_TIMEOUT             200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT		       "TRIPAS MX6DL U-Boot > "
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_AUTO_COMPLETE
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

#define CONFIG_STACKSIZE               (128 * 1024)

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

#define CONFIG_ENV_SIZE			       (8 * 1024)

#define CONFIG_ENV_IS_IN_MMC

#if defined(CONFIG_ENV_IS_IN_MMC)
	#define CONFIG_ENV_OFFSET		   (12288 * 1024)
	#define CONFIG_DYNAMIC_MMC_DEVNO
	#define CONFIG_SYS_MMC_ENV_DEV		2	/* SDHC4 */
#endif

#define CONFIG_OF_LIBFDT

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		  100000

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08

/* USB Configs */
#define CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET	/* For OTG port */
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

#endif                         /* __RTX_VALOR_MX6DL_CONFIG_H */
