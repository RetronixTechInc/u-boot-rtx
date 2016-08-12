
#ifndef __RTX_A6_MX6Q_CONFIG_H
	#define __RTX_A6_MX6Q_CONFIG_H

	#include <asm/arch/imx-regs.h>
	#include <asm/imx-common/gpio.h>
	#include <linux/sizes.h>

	#ifdef CONFIG_SPL
	#define CONFIG_SPL_LIBCOMMON_SUPPORT
	#define CONFIG_SPL_MMC_SUPPORT
	#include "imx6_spl.h"
	#endif

	/*-----------------------------------------------------------------------
	 *bootsel.c 
	 */
	#define CONFIG_BOOT_SYSTEM
	#ifdef CONFIG_BOOT_SYSTEM
		#define CONFIG_BOOT_SYSTEM_SHOW_SETTING_INFO
		#define CONFIG_BOOT_CMD_RESET_ENV

		#define CONFIG_BOOT_SYSTEM_SETTING_OFFSET             0x600
		#define CONFIG_BOOT_SYSTEM_SETTING_SIZE               0x2
		#define CONFIG_BOOT_SYSTEM_MAX_EXTSD                  4
		#define DEF_BOOTSEL_FUNC_DEFAULT (DEF_BOOTSEL_FUNC_UD_EXTSD|DEF_BOOTSEL_FUNC_MENU|DEF_BOOTSEL_FUNC_CHG_STORAGE)
	#endif

		#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET     	0x800
		#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE       	0x5000
		#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET 	0x5C00
		#define CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE   	0x400
		#define CONFIG_BOOT_SYSTEM_KERNEL_OFFSET     			0x6800
		#define CONFIG_BOOT_SYSTEM_KERNEL_SIZE       			0x5000
		#define CONFIG_BOOT_SYSTEM_KERNEL_DTB_OFFSET 			0x6400
		#define CONFIG_BOOT_SYSTEM_KERNEL_DTB_SIZE   			0x400
		#define CONFIG_BOOT_SYSTEM_URAMDISK_FS_OFFSET         	0xC000
		#define CONFIG_BOOT_SYSTEM_URAMDISK_FS_SIZE           	0x1000
		#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET         	0xD000
		#define CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE           	0x2000
		#define CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET         	0xF000
		#define CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE           	0x5000
		#define CONFIG_BOOT_SYSTEM_LOGO_OFFSET                	0x1E000
		#define CONFIG_BOOT_SYSTEM_LOGO_SIZE                  	0x1000
	
	/*-----------------------------------------------------------------------
	 * MCU watch dog
	 */
	#define CONFIG_RTX_EFM32
	#ifdef CONFIG_RTX_EFM32
		#define CONFIG_MCU_WDOG_BUS	2
	#endif
	
	#define CONFIG_MACH_TYPE					3980
	#define CONFIG_MXC_UART_BASE				UART2_BASE
	#define CONFIG_CONSOLE_DEV					"ttymxc1"

	#define CONFIG_DEFAULT_FDT_FILE				"imx6q-sabresd.dtb"

	#if defined(CONFIG_MX6_DDR_2G)
		#define PHYS_SDRAM_SIZE				(2u * 1024 * 1024 * 1024)
	#else 
		#define PHYS_SDRAM_SIZE				(1u * 1024 * 1024 * 1024)
	#endif

	#define CONFIG_MX6

	/* uncomment for PLUGIN mode support */
	/* #define CONFIG_USE_PLUGIN */

	/* uncomment for SECURE mode support */
	/* #define CONFIG_SECURE_BOOT */

	#ifdef CONFIG_SECURE_BOOT
	#ifndef CONFIG_CSF_SIZE
	#define CONFIG_CSF_SIZE 0x4000
	#endif
	#endif

	#define CONFIG_ARM_ERRATA_743622
	#define CONFIG_ARM_ERRATA_751472
	#define CONFIG_ARM_ERRATA_794072
	#define CONFIG_ARM_ERRATA_761320
	#define CONFIG_ARM_ERRATA_845369

	#ifndef CONFIG_SYS_L2CACHE_OFF
		#define CONFIG_SYS_L2_PL310
		#define CONFIG_SYS_PL310_BASE				L2_PL310_BASE
	#endif

	#define CONFIG_MP
	#define CONFIG_GPT_TIMER

	#define CONFIG_BOARD_POSTCLK_INIT
	#define CONFIG_LDO_BYPASS_CHECK
	#define CONFIG_MXC_GPT_HCLK

	#ifndef CONFIG_MX6Q
		#define CONFIG_MX6Q
	#endif

	#define CONFIG_IMX_THERMAL

	#define CONFIG_DISPLAY_CPUINFO
	#define CONFIG_DISPLAY_BOARDINFO

	#define CONFIG_CMDLINE_TAG
	#define CONFIG_SETUP_MEMORY_TAGS
	#define CONFIG_INITRD_TAG
	#define CONFIG_REVISION_TAG

	#define CONFIG_SYS_GENERIC_BOARD

	/* Size of malloc() pool */
	#define CONFIG_SYS_MALLOC_LEN				(16 * SZ_1M)

	#define CONFIG_BOARD_EARLY_INIT_F
	#define CONFIG_BOARD_LATE_INIT
	#define CONFIG_MXC_GPIO

	#define CONFIG_MXC_UART

	#define CONFIG_CMD_FUSE
	#ifdef CONFIG_CMD_FUSE
		#define CONFIG_MXC_OCOTP
	#endif

	/* MMC Configs */
	#define CONFIG_FSL_ESDHC
	#define CONFIG_FSL_USDHC
	#define CONFIG_SYS_FSL_ESDHC_ADDR      		0

	#define CONFIG_MMC
	#define CONFIG_CMD_MMC
	#define CONFIG_GENERIC_MMC
	#define CONFIG_BOUNCE_BUFFER
	#define CONFIG_CMD_EXT2
	#define CONFIG_CMD_EXT4
	#define CONFIG_CMD_EXT4_WRITE
	#define CONFIG_CMD_FAT
	#define CONFIG_DOS_PARTITION

	#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */

	#define CONFIG_CMD_PING
	#define CONFIG_CMD_DHCP
	#define CONFIG_CMD_MII
	#define CONFIG_CMD_NET
	#define CONFIG_FEC_MXC
	#define CONFIG_MII
	#define IMX_FEC_BASE						ENET_BASE_ADDR
	#define CONFIG_FEC_XCV_TYPE					RGMII
	#define CONFIG_ETHPRIME						"FEC"
	#define CONFIG_FEC_MXC_PHYADDR				1

	#define CONFIG_PHYLIB
	#define CONFIG_PHY_ATHEROS

	/* allow to overwrite serial and ethaddr */
	#define CONFIG_ENV_OVERWRITE
	#define CONFIG_CONS_INDEX              		1
	#define CONFIG_BAUDRATE                     115200

	/* Command definition */
	#include <config_cmd_default.h>

	#define CONFIG_CMD_BMODE
	#define CONFIG_CMD_BOOTZ
	#define CONFIG_CMD_SETEXPR
	#undef CONFIG_CMD_IMLS

	#define CONFIG_BOOTDELAY               		1

	#define CONFIG_LOADADDR   CONFIG_DEF_LOADADDR
	/* define by configs/vendor_***_defconfig.h
	#define CONFIG_DTB_LOADADDR            0x11300000
	#define CONFIG_RD_LOADADDR             0x11400000
	#define CONFIG_SYS_TEXT_BASE           0x27800000
	*/

	#define CONFIG_SYS_MMC_IMG_LOAD_PART		1

	#define CONFIG_MFG_NAND_PARTITION ""

	#ifdef CONFIG_SUPPORT_EMMC_BOOT
		#define EMMC_ENV \
			"emmcdev=2\0" \
			"update_emmc_firmware=" \
			"if test ${ip_dyn} = yes; then " \
				"setenv get_cmd dhcp; " \
			"else " \
				"setenv get_cmd tftp; " \
			"fi; " \
			"if ${get_cmd} ${update_sd_firmware_filename}; then " \
				"if mmc dev ${emmcdev} 1; then "	\
					"setexpr fw_sz ${filesize} / 0x200; " \
					"setexpr fw_sz ${fw_sz} + 1; "	\
					"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
				"fi; "	\
			"fi\0"
	#else
		#define EMMC_ENV ""
	#endif

	#define CONFIG_VERSION_STRING "rtx-a6plus-mx6q"

	/*-----------------------------------------------------------------------
	 * update and recovery parameter
	 */
	/* usb or sd card */
	#define CONFIG_ENG_BOOTARGS \
		"setenv bootargs ${bootargs} root=/dev/ram0 rdinit=/sbin/init rdisk_option=${roption} storage=${rstorage} mmcroot=" CONFIG_UPDATEROOT
	#define CONFIG_ENG_BOOTCMD  \
		"run bootargs_base set_display set_mem bootargs_console ext_args; bootm ${loadaddr} ${rd_loadaddr}"
	#define CONFIG_ENG_DTB_BOOTCMD  \
		"run bootargs_base set_display set_mem bootargs_console ext_args; bootm ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}"

	/* recovery mode parameter 'r' or 'R' key*/
	#define CONFIG_ANDROID_RECOVERY_BOOTARGS \
		"setenv bootargs ${bootargs} init=/init"
	#ifdef CONFIG_EXTRA_ENV_USE_DTB
		#define CONFIG_ANDROID_RECOVERY_BOOTCMD  \
			"run bootargs_base set_display set_mem bootargs_console ext_args;"			\
			"mmc dev ${mmc_num};mmc read ${loadaddr} "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET) " " __stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE) ";"\
			"mmc read ${dtb_loadaddr}  "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET) " " __stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE) ";" \
			"mmc read ${rd_loadaddr}  "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET) " " __stringify(CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE) ";" \
			"bootm ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}"
	#else
		#define CONFIG_ANDROID_RECOVERY_BOOTCMD  \
			"run bootargs_base set_display set_mem bootargs_console ext_args;"			\
			"mmc dev ${mmc_num};mmc read ${loadaddr} "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET) " " __stringify(CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE) ";"\
			"mmc read ${rd_loadaddr}  "__stringify(CONFIG_BOOT_SYSTEM_RECOVERY_FS_OFFSET) " " __stringify(CONFIG_BOOT_SYSTEM_RECOVERY_FS_SIZE) ";" \
			"bootm ${loadaddr} ${rd_loadaddr}"
	#endif

	#ifdef CONFIG_EXTRA_ENV_USE_DTB
		#define CONFIG_EXTRA_ENV_BOOTCMD_GEN "bootcmd_gen=run bootargs_base set_display set_mem bootargs_console bootargs_gen ; bootm ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}\0"
		#define CONFIG_EXTRA_ENV_BOOTCMD_MFG "bootcmd_mfg=run bootargs_base set_display set_mem bootargs_console bootargs_gen ; bootm ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}\0"
	#else
		#define CONFIG_EXTRA_ENV_BOOTCMD_GEN "bootcmd_gen=run bootargs_base set_display set_mem bootargs_console bootargs_gen ; bootm ${loadaddr} ${rd_loadaddr}\0"
		#define CONFIG_EXTRA_ENV_BOOTCMD_MFG "bootcmd_mfg=run bootargs_base set_display set_mem bootargs_console bootargs_gen ; bootm ${loadaddr} ${rd_loadaddr}\0"
	#endif

	#define	CONFIG_EXTRA_ENV_SETTINGS \
		"bootcmd=run bootcmd_gen\0"	\
		"bootargs_base=setenv bootargs androidboot.hardware=freescale no_console_suspend\0" \
		"set_display=run " CONFIG_GUIPORT "\0" \
		"set_mem=setenv bootargs ${bootargs} " CONFIG_BOOTARGS_GUIMEM "\0" \
		"bootargs_console=setenv bootargs ${bootargs} console=" CONFIG_CONSOLE_DEV "," __stringify(CONFIG_BAUDRATE) " androidboot.console=" CONFIG_CONSOLE_DEV "\0"	\
		"bootargs_gen=setenv bootargs ${bootargs} " CONFIG_BOOTARGS_GEN "\0"	\
		"hdmi=setenv bootargs ${bootargs} " CONFIG_BOOTARGS_HDMI "\0" \
		"vga=setenv bootargs ${bootargs} " CONFIG_BOOTARGS_VGA "\0" \
		"dual-hdmi=setenv bootargs ${bootargs} " CONFIG_BOOTARGS_DUAL_HDMI "\0" \
		"mmc_num=" CONFIG_UBOOT_MMCNUM "\0"	  \
		"storage=mmc dev ${mmc_num}\0" \
		"fecmac_val=" CONFIG_DEFAULT_MAC01 "\0" \
		"mmcrootpath=" CONFIG_MMCROOTPATH "\0" \
		"r_kernel=mmc read ${loadaddr} "__stringify(CONFIG_BOOT_SYSTEM_KERNEL_OFFSET) " " __stringify(CONFIG_BOOT_SYSTEM_KERNEL_SIZE) "\0" \
		"r_dtb=mmc read ${dtb_loadaddr} " __stringify(CONFIG_BOOT_SYSTEM_KERNEL_DTB_OFFSET) " " __stringify(CONFIG_BOOT_SYSTEM_KERNEL_DTB_SIZE) "\0"\
		"r_ramdisk=mmc read ${rd_loadaddr} " __stringify(CONFIG_BOOT_SYSTEM_URAMDISK_FS_OFFSET) " " __stringify(CONFIG_BOOT_SYSTEM_URAMDISK_FS_SIZE) "\0" \
		CONFIG_EXTRA_ENV_BOOTCMD_MFG	\
		CONFIG_EXTRA_ENV_BOOTCMD_GEN	\
		"splashpos=m,m\0"	  \
		"def_video=" CONFIG_VGA_VIDEO "\0" \
		"loadaddr=" __stringify(CONFIG_LOADADDR) "\0" \
		"dtb_loadaddr=" __stringify(CONFIG_DTB_LOADADDR) "\0" \
		"rd_loadaddr=" __stringify(CONFIG_RD_LOADADDR) "\0" \
		"version=" CONFIG_VERSION_STRING "\0"

	#define CONFIG_ARP_TIMEOUT     					200UL

	/* Miscellaneous configurable options */
	#define CONFIG_SYS_LONGHELP
	#define CONFIG_SYS_HUSH_PARSER
	#define CONFIG_SYS_PROMPT_HUSH_PS2     			"> "
	#define CONFIG_AUTO_COMPLETE
	#define CONFIG_SYS_CBSIZE              			1024

	/* Print Buffer Size */
	#define CONFIG_SYS_PBSIZE 						(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
	#define CONFIG_SYS_MAXARGS             			256
	#define CONFIG_SYS_BARGSIZE 					CONFIG_SYS_CBSIZE

	#define CONFIG_CMD_MEMTEST
	#define CONFIG_SYS_MEMTEST_START       			0x10000000
	#define CONFIG_SYS_MEMTEST_END         			0x10010000
	#define CONFIG_SYS_MEMTEST_SCRATCH     			0x10800000

	#define CONFIG_SYS_LOAD_ADDR           			CONFIG_LOADADDR

	#define CONFIG_CMDLINE_EDITING
	#define CONFIG_STACKSIZE               			(128 * 1024)

	/* Physical Memory Map */
	#define CONFIG_NR_DRAM_BANKS           			1
	#define PHYS_SDRAM                     			MMDC0_ARB_BASE_ADDR

	#define CONFIG_SYS_SDRAM_BASE          			PHYS_SDRAM
	#define CONFIG_SYS_INIT_RAM_ADDR       			IRAM_BASE_ADDR
	#define CONFIG_SYS_INIT_RAM_SIZE       			IRAM_SIZE

	#define CONFIG_SYS_INIT_SP_OFFSET 				(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
	#define CONFIG_SYS_INIT_SP_ADDR 				(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

	/* FLASH and environment organization */
	#define CONFIG_SYS_NO_FLASH

	#define CONFIG_ENV_SIZE							(8 * 1024)

	#ifndef CONFIG_SYS_NOSMP
		#define CONFIG_SYS_NOSMP
	#endif

	#define CONFIG_ENV_IS_IN_MMC


	#define CONFIG_ENV_OFFSET						(12288 * 1024)
	
	#define CONFIG_OF_LIBFDT

	#ifndef CONFIG_SYS_DCACHE_OFF
		#define CONFIG_CMD_CACHE
	#endif

	/* I2C Configs */
	#define CONFIG_CMD_I2C
	#define CONFIG_SYS_I2C
	#define CONFIG_SYS_I2C_MXC
	#define CONFIG_SYS_I2C_SPEED		  			100000

	/* Framebuffer */
	#define CONFIG_VIDEO
	#define CONFIG_VIDEO_IPUV3
	#define CONFIG_CFB_CONSOLE
	#define CONFIG_VGA_AS_SINGLE_DEVICE
	#define CONFIG_SYS_CONSOLE_IS_IN_ENV
	#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
	#define CONFIG_VIDEO_BMP_RLE8
	#define CONFIG_SPLASH_SCREEN
	#define CONFIG_SPLASH_SCREEN_ALIGN
	#define CONFIG_BMP_16BPP
	#define CONFIG_VIDEO_LOGO
	#define CONFIG_VIDEO_BMP_LOGO
	#ifdef CONFIG_MX6DL                                                             
		#define CONFIG_IPUV3_CLK 						198000000
	#else
		#define CONFIG_IPUV3_CLK 						264000000
	#endif
	#define CONFIG_IMX_HDMI
	#define CONFIG_IMX_VIDEO_SKIP

	/* support android */
	#if defined(CONFIG_ANDROID_SUPPORT)
		#define CONFIG_CI_UDC
		#define CONFIG_USBD_HS
		#define CONFIG_USB_GADGET_DUALSPEED

		#define CONFIG_USB_GADGET
		#define CONFIG_CMD_USB_MASS_STORAGE
		#define CONFIG_USB_GADGET_MASS_STORAGE
		#define CONFIG_USBDOWNLOAD_GADGET
		#define CONFIG_USB_GADGET_VBUS_DRAW	2

		#define CONFIG_G_DNL_VENDOR_NUM		0x18d1
		#define CONFIG_G_DNL_PRODUCT_NUM	0x0d02
		#define CONFIG_G_DNL_MANUFACTURER	"FSL"

		#define CONFIG_CMD_FASTBOOT
		#define CONFIG_ANDROID_BOOT_IMAGE
		#define CONFIG_FASTBOOT_FLASH

		#define CONFIG_FSL_FASTBOOT
		#define CONFIG_ANDROID_RECOVERY

		#if defined CONFIG_SYS_BOOT_NAND
		#define CONFIG_FASTBOOT_STORAGE_NAND
		#elif defined CONFIG_SYS_BOOT_SATA
		#define CONFIG_FASTBOOT_STORAGE_SATA
		#else
		#define CONFIG_FASTBOOT_STORAGE_MMC
		#endif

		#define CONFIG_ANDROID_MAIN_MMC_BUS 2
		#define CONFIG_ANDROID_BOOT_PARTITION_MMC 1
		#define CONFIG_ANDROID_SYSTEM_PARTITION_MMC 5
		#define CONFIG_ANDROID_RECOVERY_PARTITION_MMC 2
		#define CONFIG_ANDROID_CACHE_PARTITION_MMC 6
		#define CONFIG_ANDROID_DATA_PARTITION_MMC 4

		#if defined(CONFIG_FASTBOOT_STORAGE_NAND)
		#define ANDROID_FASTBOOT_NAND_PARTS "16m@64m(boot) 16m@80m(recovery) 810m@96m(android_root)ubifs"
		#endif

		#define CONFIG_CMD_BOOTA
		#define CONFIG_SUPPORT_RAW_INITRD
		#define CONFIG_SERIAL_TAG

		#undef CONFIG_BOOTCOMMAND

		#define CONFIG_USB_FASTBOOT_BUF_ADDR   CONFIG_SYS_LOAD_ADDR
		#define CONFIG_USB_FASTBOOT_BUF_SIZE   0x19000000
	#endif

	#define CONFIG_SYS_FSL_USDHC_NUM					3
	#define CONFIG_DYNAMIC_MMC_DEVNO
	#define CONFIG_SYS_MMC_ENV_DEV						1	/* SDHC3 */
	#define CONFIG_SYS_MMC_ENV_PART                		0       /* user partition */

	/* PMIC */
	#define CONFIG_POWER
	#define CONFIG_POWER_I2C
	#define CONFIG_POWER_PFUZE100
	#define CONFIG_POWER_PFUZE100_I2C_ADDR				0x08

	/* USB Configs */
	#define CONFIG_CMD_USB
	#ifdef CONFIG_CMD_USB
		#define CONFIG_USB_EHCI
		#define CONFIG_USB_EHCI_MX6
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

		#define CONFIG_WAVEFORM_BUF_SIZE		0x200000
	#endif

#endif
