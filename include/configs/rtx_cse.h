/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __RTX_CSE_H
#define __RTX_CSE_H

#include <linux/sizes.h>
#include <asm/arch/imx-regs.h>

#include "imx_env.h"

#ifdef CONFIG_SECURE_BOOT
#define CONFIG_CSF_SIZE			0x2000 /* 8K region */
#endif

/*-----------------------------------------------------------------------
 *bootsel.c 
 */

#define CONFIG_BOOT_SYSTEM
#ifdef CONFIG_BOOT_SYSTEM
	#define CONFIG_BOOT_SYSTEM_SHOW_SETTING_INFO
	#define CONFIG_BOOT_CMD_RESET_ENV
	#define CONFIG_BOOT_CMD_RESET_SETTING

	#define CONFIG_BOOT_SYSTEM_SETTING_OFFSET             0xC00
	#define CONFIG_BOOT_SYSTEM_SETTING_SIZE               0x2
	#define CONFIG_BOOT_SYSTEM_MAX_EXTSD                  4
	#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET     	0x1800
	#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE       	0xC800
	#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET 	0x1400
	#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE   	0x400
	#define CONFIG_BOOT_SYSTEM_URAMDISK_FS_OFFSET         	0xE000
	#define CONFIG_BOOT_SYSTEM_URAMDISK_FS_SIZE           	0x8000

	#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET         	0x16000
	#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE           	0x2000
	#define CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET         	0x18000
	#define CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE           	0x5000
	#define CONFIG_BOOT_SYSTEM_LOGO_OFFSET                	0x1D000
	#define CONFIG_BOOT_SYSTEM_LOGO_SIZE                  	0x1000
#endif
/*-----------------------------------------------------------------------
 * MCU watch dog
 */
#define CONFIG_RTX_EFM32
#ifdef CONFIG_RTX_EFM32
	#define CONFIG_MCU_WDOG_BUS	0
#endif

#define CONFIG_SPL_MAX_SIZE		(148 * 1024)
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION	1
#define CONFIG_SYS_UBOOT_BASE		(QSPI0_AMBA_BASE + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)

#ifdef CONFIG_SPL_BUILD
/*#define CONFIG_ENABLE_DDR_TRAINING_DEBUG*/
#define CONFIG_SPL_WATCHDOG_SUPPORT
#define CONFIG_SPL_POWER_SUPPORT
#define CONFIG_SPL_DRIVERS_MISC_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv8/u-boot-spl.lds"
#define CONFIG_SPL_STACK		0x91fff0
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_BSS_START_ADDR      0x00910000
#define CONFIG_SPL_BSS_MAX_SIZE        0x2000	/* 8 KB */
#define CONFIG_SYS_SPL_MALLOC_START    0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE     0x80000	/* 512 KB */
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF

#define CONFIG_MALLOC_F_ADDR		0x912000 /* malloc f used before GD_FLG_FULL_MALLOC_INIT set */

#define CONFIG_SPL_ABORT_ON_RAW_IMAGE /* For RAW image gives a error info not panic */

#undef CONFIG_DM_MMC
#undef CONFIG_DM_PMIC
#undef CONFIG_DM_PMIC_PFUZE100

#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_BD71837

#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#if defined(CONFIG_NAND_BOOT)
#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_DMA_SUPPORT
#define CONFIG_SPL_NAND_MXS
#define CONFIG_SYS_NAND_U_BOOT_OFFS 	0x4000000 /* Put the FIT out of first 64MB boot area */

/* Set a redundant offset in nand FIT mtdpart. The new uuu will burn full boot image (not only FIT part) to the mtdpart, so we check both two offsets */
#define CONFIG_SYS_NAND_U_BOOT_OFFS_REDUND \
	(CONFIG_SYS_NAND_U_BOOT_OFFS + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512 - 0x8400)
#endif

#endif

#define CONFIG_CMD_READ
#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV 0

#define CONFIG_REMAKE_ELF

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_POSTCLK_INIT
#define CONFIG_BOARD_LATE_INIT

/* Flat Device Tree Definitions */
#define CONFIG_OF_BOARD_SETUP

#undef CONFIG_CMD_EXPORTENV
#undef CONFIG_CMD_IMPORTENV
#undef CONFIG_CMD_IMLS

#undef CONFIG_CMD_CRC32
#undef CONFIG_BOOTM_NETBSD

/* ENET Config */
/* ENET1 */
#if defined(CONFIG_CMD_NET)
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_MII
#define CONFIG_ETHPRIME                 "FEC"

#define CONFIG_FEC_MXC
#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_FEC_MXC_PHYADDR          0
#define FEC_QUIRK_ENET_MAC

#define CONFIG_PHY_GIGE
#define IMX_FEC_BASE			0x30BE0000

#define CONFIG_PHYLIB
#define CONFIG_PHY_ATHEROS
#endif

/*
 * Another approach is add the clocks for inmates into clks_init_on
 * in clk-imx8mm.c, then clk_ingore_unused could be removed.
 */
#define JAILHOUSE_ENV \
	"jh_clk= \0 " \
	"jh_mmcboot=mw 0x303d0518 0xff; setenv fdt_file fsl-imx8mm-evk-root.dtb;" \
		"setenv jh_clk clk_ignore_unused; " \
			   "if run loadimage; then " \
				   "run mmcboot; " \
			   "else run jh_netboot; fi; \0" \
	"jh_netboot=mw 0x303d0518 0xff; setenv fdt_file fsl-imx8mm-evk-root.dtb; setenv jh_clk clk_ignore_unused; run netboot; \0 "

#ifdef CONFIG_NAND_BOOT
#define MFG_NAND_PARTITION "mtdparts=gpmi-nand:64m(nandboot),16m(nandfit),32m(nandkernel),16m(nanddtb),8m(nandtee),-(nandrootfs) "
#endif

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x43800000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	"emmc_dev=1\0"\
	"sd_dev=0\0" \

/* Initial environment variables */
#if defined(CONFIG_NAND_BOOT)
#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	"fdt_addr=0x43000000\0"			\
	"fdt_high=0xffffffffffffffff\0" \
	"mtdparts=" MFG_NAND_PARTITION "\0" \
	"console=ttymxc1,115200 earlycon=ec_imx6q,0x30890000,115200\0" \
	"bootargs=console=ttymxc1,115200 earlycon=ec_imx6q,0x30890000,115200 ubi.mtd=5 "  \
		"root=ubi0:nandrootfs rootfstype=ubifs "		     \
		MFG_NAND_PARTITION \
		"\0" \
	"bootcmd=nand read ${loadaddr} 0x5000000 0x2000000;"\
		"nand read ${fdt_addr} 0x7000000 0x100000;"\
		"booti ${loadaddr} - ${fdt_addr}"

#else
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"script=boot.scr\0" \
	"image=Image\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"boot_fdt=try\0" \
	"version=rtx-imx8mm-cse\0" \
	"count=0\0" \
	"count1=2\0" \
	"count2=5\0" \
	"panel=MIPI2HDMI\0" \
	"console=ttymxc1,115200 earlycon=ec_imx6q,0x30890000,115200\0" \
	"fdt_addr=0x43000000\0"			\
	"fdt_high=0xffffffffffffffff\0"		\
	"initrd_addr=0x43800000\0"		\
	"initrd_high=0xffffffffffffffff\0" \
	"emmc_dev=1\0"\
	"sd_dev=0\0"\
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"bootargs_gen=setenv bootargs ${bootargs} " CONFIG_BOOTARGS_GEN "\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcautodetect=yes\0" \
	"mmcargs=setenv bootargs ${jh_clk} console=${console}; run bootargs_gen\0 " \
	"ramargs=setenv bootargs ${jh_clk} console=${console} root=/dev/ram0 rootwait rw rdinit=/sbin/init\0 " \
	"loadm4fw=fatload mmc ${mmcdev}:${mmcpart} 0x7e0000 m4fw.bin;\0" \
	"loadbootscript=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"loadbootcount=fatload mmc ${mmcdev}:${mmcpart} 0x80002000 boot.count;\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"bootscript=echo Running bootscript from mmc ...; source\0" \
	"mmcboot=echo Booting from mmc ...; run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then booti ${loadaddr} - ${fdt_addr}; " \
			"else echo WARN: Cannot load the DT; fi; " \
		"else echo wait for boot; fi;\0" \
	"r_dtb=mmc read ${fdt_addr} 0x1400 0x400\0" \
	"r_kernel=mmc read ${loadaddr} 0x1800 0xc800\0" \
	"r_ramdisk=mmc read ${initrd_addr} 0xE000 0x4000\0" \
	"mmcboot2=echo Booting from recovery ...; run mmcargs; " \
		"run r_kernel r_dtb; booti ${loadaddr} - ${fdt_addr};\0" \
	"ramboot=echo Booting to ramdisk ...; run ramargs; " \
		"run r_kernel r_dtb r_ramdisk; booti ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \
	"checkcount=" \
		"if run loadbootcount; then echo Check count ... ${count}; "\
			"if test ${count} > ${count2}; then run ramboot; fi; " \
			"if test ${count} > ${count1}; then run mmcboot2; fi; " \
			"if run loadimage; then setexpr count ${count} + 1; saveenv; run mmcboot; " \
			"else run mmcboot2; fi; " \
		"else " \
			"if run loadimage; then run mmcboot; " \
			"else run mmcboot2; fi; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs;  " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${loadaddr} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"booti ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"else " \
			"booti; " \
		"fi;\0"

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev}; " \
	"if mmc rescan; then " \
		"if run loadm4fw; then run loadm4fw; bootaux 0x7e0000; fi; " \
		"if run loadbootscript; then run bootscript; " \
		"else run checkcount; fi; " \
	"else echo Cannot find the mmcdev; fi;"
#endif

/* Link Definitions */
#define CONFIG_LOADADDR			0x40480000

#define CONFIG_SYS_LOAD_ADDR           CONFIG_LOADADDR

#define CONFIG_SYS_INIT_RAM_ADDR        0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE        0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
        (CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
        (CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_ENV_OVERWRITE
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET               (32 * SZ_64K)
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET		(2 * 1024 * 1024)
#define CONFIG_ENV_SECT_SIZE		(64 * 1024)
#define CONFIG_ENV_SPI_BUS		CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS		CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE		CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
#elif defined(CONFIG_ENV_IS_IN_NAND)
#define CONFIG_ENV_OFFSET       (30 << 20)
#endif
#define CONFIG_ENV_SIZE			0x2000
#define CONFIG_SYS_MMC_ENV_DEV		1   /* USDHC2(SD) = 0, USDHC3(MMC) = 1 */

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		((CONFIG_ENV_SIZE + (12*1024) + (16*1024)) * 1024)

#define CONFIG_SYS_SDRAM_BASE           0x40000000
#define PHYS_SDRAM                      0x40000000
#define PHYS_SDRAM_SIZE			0x80000000 /* 2GB DDR */
#define CONFIG_NR_DRAM_BANKS		1

#define CONFIG_SYS_MEMTEST_START    PHYS_SDRAM
#define CONFIG_SYS_MEMTEST_END      (CONFIG_SYS_MEMTEST_START + (PHYS_SDRAM_SIZE >> 1))

#define CONFIG_BAUDRATE			115200

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART2_BASE_ADDR
/*#define CONFIG_MXC_UART_BASE		UART4_BASE_ADDR		*/

/* Monitor Command Prompt */
#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT		"u-boot=> "
#define CONFIG_SYS_PROMPT_HUSH_PS2     "> "
#define CONFIG_SYS_CBSIZE              2048
#define CONFIG_SYS_MAXARGS             64
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_IMX_BOOTAUX


/*-----------------------------------------------------------------------
 * update and recovery parameter
 */
/* usb or sd card */
#define CONFIG_ENG_BOOTARGS \
	"setenv bootargs ${bootargs} root=/dev/ram0 rdinit=/sbin/init rdisk_option=${roption} storage=${rstorage} pcba_version=${version} mmcroot=" CONFIG_UPDATEROOT
#define CONFIG_ENG_BOOTCMD  \
	"setenv bootargs console=${console}; run ext_args; booti " __stringify(CONFIG_LOADADDR) " " __stringify(CONFIG_RD_LOADADDR)
#define CONFIG_ENG_DTB_BOOTCMD  \
	"setenv bootargs console=${console}; run ext_args; booti " __stringify(CONFIG_LOADADDR) " " __stringify(CONFIG_RD_LOADADDR) " "  __stringify(CONFIG_DTB_LOADADDR)


/* USDHC */
#define CONFIG_CMD_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC

#ifdef CONFIG_TARGET_IMX8MM_DDR4_EVK
#define CONFIG_SYS_FSL_USDHC_NUM	1
#else
#define CONFIG_SYS_FSL_USDHC_NUM	2
#endif
#define CONFIG_DYNAMIC_MMC_DEVNO
#define CONFIG_SYS_MMC_ENV_PART		0       /* user partition */
#define CONFIG_SYS_FSL_ESDHC_ADDR       0

#define CONFIG_SUPPORT_EMMC_BOOT	/* eMMC specific */
#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#ifdef CONFIG_FSL_FSPI
#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_SPEED		40000000
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#define FSL_FSPI_FLASH_SIZE		SZ_32M
#define FSL_FSPI_FLASH_NUM		1
#define FSPI0_BASE_ADDR			0x30bb0000
#define FSPI0_AMBA_BASE			0x0
#define	CONFIG_SPI_FLASH_BAR
#define CONFIG_FSPI_QUAD_SUPPORT
#define CONFIG_SYS_FSL_FSPI_AHB
#endif

/* Enable SPI */
#ifndef CONFIG_NAND_MXS
#ifndef CONFIG_FSL_FSPI
#ifdef CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_MXC_SPI
#define CONFIG_SF_DEFAULT_BUS  0
#define CONFIG_SF_DEFAULT_SPEED 20000000
#define CONFIG_SF_DEFAULT_MODE (SPI_MODE_0)
#endif
#endif
#endif

#ifdef CONFIG_CMD_NAND
#define CONFIG_NAND_MXS
#define CONFIG_CMD_NAND_TRIMFFS

/* NAND stuff */
#define CONFIG_SYS_MAX_NAND_DEVICE     1
#define CONFIG_SYS_NAND_BASE           0x20000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION

/* DMA stuff, needed for GPMI/MXS NAND support */
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8

#ifdef CONFIG_CMD_UBI
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#endif
#endif /* CONFIG_CMD_NAND */


#define CONFIG_MXC_GPIO

#define CONFIG_MXC_OCOTP
#define CONFIG_CMD_FUSE

#ifndef CONFIG_DM_I2C
#define CONFIG_SYS_I2C
#endif
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
#define CONFIG_SYS_I2C_SPEED		100000

/* USB configs */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#define CONFIG_USBD_HS

#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USB_FUNCTION_MASS_STORAGE

#endif

#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET_VBUS_DRAW 2

#define CONFIG_CI_UDC

#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_USB_MAX_CONTROLLER_COUNT         2

#define CONFIG_USE_PREBOOT
#define CONFIG_PREBOOT "setenv stdin serial,usbkbd; setenv stdout serial,vga; usb start;"
#define CONFIG_Enable_KEYFUNC

#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_MXS
#define CONFIG_VIDEO_LOGO
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_VIDEO_SKIP
#define CONFIG_RM67191
#endif

#define CONFIG_OF_SYSTEM_SETUP

#endif
