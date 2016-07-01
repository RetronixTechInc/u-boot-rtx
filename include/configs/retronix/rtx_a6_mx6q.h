
#ifndef __RTX_A6_MX6Q_CONFIG_H
	#define __RTX_A6_MX6Q_CONFIG_H

	#include <asm/arch/imx-regs.h>
	#include <asm/imx-common/gpio.h>
	#include <linux/sizes.h>

	#define CONFIG_TARGET_RTX_A6_MX6Q_DISABLE_EFM32_WATCHDOG

	#define CONFIG_MACH_TYPE					3980
	#define CONFIG_MXC_UART_BASE				UART1_BASE
	#define CONFIG_CONSOLE_DEV					"ttymxc0"

	#define CONFIG_MMCROOT						"/dev/mmcblk2p2"  /* SDHC3 */
	#define CONFIG_DEFAULT_FDT_FILE				"imx6q-sabresd.dtb"
	#define PHYS_SDRAM_SIZE						(1u * 1024 * 1024 * 1024)

	#define CONFIG_MX6

	/* uncomment for PLUGIN mode support */
	/* #define CONFIG_USE_PLUGIN */

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

	#define CONFIG_LOADADDR                     0x12000000
	#define CONFIG_RD_LOADADDR             		0x13000000
	#define CONFIG_DTB_LOADADDR            		0x12F00000
	#define CONFIG_SYS_TEXT_BASE           		0x17800000
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

	#define CONFIG_VERSION_STRING "rtx-a6-mx6q"

	#define CONFIG_EXTRA_ENV_USE_DTB

	#ifdef CONFIG_EXTRA_ENV_USE_DTB
		#define CONFIG_EXTRA_ENV_BOOTCMD_GEN "bootcmd_gen=run bootargs_base bootargs_gen set_display set_mem ;run storage r_kernel r_dtb; bootm ${loadaddr} - ${dtb_loadaddr}\0"
	#else
		#define CONFIG_EXTRA_ENV_BOOTCMD_GEN "bootcmd_gen=run bootargs_base bootargs_gen set_display set_mem ;run storage r_kernel; bootm\0"
	#endif

	#ifdef CONFIG_EXTRA_ENV_USE_DTB
		#define CONFIG_EXTRA_ENV_SETTINGS \
			"hdmi=setenv bootargs ${bootargs} video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32\0" \
			"set_mem=setenv bootargs ${bootargs}\0" \
			"fecmac_val=fec_mac=fa:3a:65:c7:14:eb\0" \
			"set_display=run hdmi\0" \
			"bootargs_base=setenv bootargs console=ttymxc0,115200 ${fecmac_val}\0"\
			"bootargs_gen=setenv bootargs ${bootargs} ip=off ${root_loc} rootfstype=ext4 rootwait rw\0"\
			"r_kernel=mmc read ${loadaddr} 0x6800 0x3000\0" \
			"r_ramdisk=mmc read ${rd_loadaddr} 0x3000 0x3000\0" \
			"r_dtb=mmc read ${dtb_loadaddr} 0x9800 0x600\0" \
			"storage=mmc dev 2\0" \
			"root_loc=root=/dev/mmcblk0p1\0" \
			"bootargs_ramdisk=setenv bootargs ${bootargs} root=/dev/ram0 rootwait rw rdinit=/sbin/init\0"	\
			"bootcmd_ramdisk=run bootargs_base bootargs_ramdisk set_display set_mem ;run storage r_kernel r_ramdisk r_dtb ;bootm ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}\0" \
			CONFIG_EXTRA_ENV_BOOTCMD_GEN	\
			"bootcmd=run bootcmd_gen\0"	\
			"version=" CONFIG_VERSION_STRING "\0"

	#else
		#define CONFIG_EXTRA_ENV_SETTINGS \
			"hdmi=setenv bootargs ${bootargs} video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32\0" \
			"set_mem=setenv bootargs ${bootargs} fbmem=28M,28M,28M gpu_nommu gpu_memory=256M\0" \
			"fecmac_val=fec_mac=fa:3a:65:c7:14:eb\0" \
			"set_display=run hdmi\0" \
			"bootargs_base=setenv bootargs console=ttymxc0,115200 ${fecmac_val}\0"\
			"bootargs_gen=setenv bootargs ${bootargs} ip=off ${root_loc} rootfstype=ext4 rootwait rw\0"\
			"r_kernel=mmc read ${loadaddr} 0x6800 0x3000\0" \
			"r_ramdisk=mmc read ${rd_loadaddr} 0x3000 0x3000\0" \
			"r_dtb=mmc read ${dtb_loadaddr} 0x9800 0x600\0" \
			"storage=mmc dev 2\0" \
			"root_loc=root=/dev/mmcblk0p1\0" \
			"bootargs_ramdisk=setenv bootargs ${bootargs} root=/dev/ram0 rootwait rw rdinit=/sbin/init\0"	\
			"bootcmd_ramdisk=run bootargs_base bootargs_ramdisk set_display set_mem ;run storage r_kernel r_ramdisk r_dtb ;bootm ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}\0" \
			CONFIG_EXTRA_ENV_BOOTCMD_GEN	\
			"bootcmd=run bootcmd_gen\0"	\
			"version=" CONFIG_VERSION_STRING "\0"

	#endif

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
