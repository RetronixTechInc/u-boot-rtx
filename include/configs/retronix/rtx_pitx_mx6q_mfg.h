
#ifndef __RTX_PITX_MX6Q_CONFIG_H
	#define __RTX_PITX_MX6Q_CONFIG_H

	#include <asm/arch/imx-regs.h>
	#include <asm/imx-common/gpio.h>
	#include <linux/sizes.h>

	#define CONFIG_MACH_TYPE					3980
	#define CONFIG_MXC_UART_BASE				UART2_BASE
	#define CONFIG_CONSOLE_DEV					"ttymxc1"

	#define CONFIG_MMCROOT						"/dev/mmcblk3p2"  /* SDHC3 */
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
	/* #define CONFIG_LDO_BYPASS_CHECK */
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

	/*
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
	*/
	
	/* allow to overwrite serial and ethaddr */
	#define CONFIG_ENV_OVERWRITE
	#define CONFIG_CONS_INDEX              		1
	#define CONFIG_BAUDRATE                     115200

	/* Command definition */
	#define CONFIG_CMD_BDI		/* bdinfo			*/
	#define CONFIG_CMD_BOOTD	/* bootd			*/
	#define CONFIG_CMD_CONSOLE	/* coninfo			*/
	#define CONFIG_CMD_ECHO		/* echo arguments		*/
	#define CONFIG_CMD_EDITENV	/* editenv			*/
	#define CONFIG_CMD_ENV_EXISTS	/* query whether env variables exists */
	#define CONFIG_CMD_FPGA		/* FPGA configuration Support	*/
	#define CONFIG_CMD_IMI		/* iminfo			*/
	#define CONFIG_CMD_ITEST	/* Integer (and string) test	*/
	#define CONFIG_CMD_LOADB	/* loadb			*/
	#define CONFIG_CMD_LOADS	/* loads			*/
	#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop */
	#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
	#define CONFIG_CMD_RUN		/* run command in env variable	*/
	#define CONFIG_CMD_SAVEENV	/* saveenv			*/
	#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
	#define CONFIG_CMD_SOURCE	/* "source" command support	*/
	#define CONFIG_CMD_XIMG		/* Load part of Multi Image	*/

	#define CONFIG_CMD_BMODE
	#define CONFIG_CMD_BOOTZ
	#define CONFIG_CMD_SETEXPR

	#define CONFIG_BOOTDELAY               		1

	#define CONFIG_LOADADDR                     0x10800000
	#define CONFIG_RD_LOADADDR             		0x11000000
	#define CONFIG_DTB_LOADADDR            		0x10F00000
	#define CONFIG_SYS_TEXT_BASE           		0x27800000
	#define CONFIG_SYS_MMC_IMG_LOAD_PART		1

	#define CONFIG_MFG_NAND_PARTITION ""
	#define EMMC_ENV ""
	
	#define CONFIG_DISABLE_MFG_AUTOBOOT_FROM_USB
	/* ramdisk */

	#ifdef CONFIG_BOOTARGS_HDMI
		/*hdmi*/
		#define CONFIG_BOOTARGS         			"console=ttymxc1,115200 rootfs=/dev/ram0 rdinit=/init rootwait video=mxcfb0:dev=hdmi,if=RGB24,fbpix=RGB24 fec_mac=00:03:2d:29:5f:24"
	#else
		/*ldb*/
		#define CONFIG_BOOTARGS         			"console=ttymxc1,115200 rootfs=/dev/ram0 rdinit=/init rootwait video=mxcfb0:dev=ldb,if=RGB666,fbpix=RGB666 fec_mac=00:03:2d:29:5f:24"
	#endif
	
	#define CONFIG_BOOTCOMMAND      			"bootm 0x10800000 0x11000000 0x10F00000"
	#define CONFIG_EXTRA_ENV_SETTINGS			"bootcmd_mfg=bootm 0x10800000 0x11000000 0x10F00000\0"


	#define CONFIG_ARP_TIMEOUT     					200UL

	/* Miscellaneous configurable options */
	#define CONFIG_SYS_LONGHELP
	#define CONFIG_SYS_PROMPT		                "RTX-PITX MX6D MFG U-Boot > "
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
	#define CONFIG_ENV_IS_NOWHERE
	
	#define CONFIG_ENV_SIZE							(8 * 1024)

	#ifndef CONFIG_SYS_NOSMP
		#define CONFIG_SYS_NOSMP
	#endif
	
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

	#define CONFIG_SYS_FSL_USDHC_NUM					3
	#define CONFIG_DYNAMIC_MMC_DEVNO
	#define CONFIG_SYS_MMC_ENV_DEV						1	/* SDHC3 */
	#define CONFIG_SYS_MMC_ENV_PART                		0       /* user partition */

	/* PMIC */
	#define CONFIG_POWER
	#define CONFIG_POWER_I2C
	#define CONFIG_POWER_PFUZE100
	#define CONFIG_POWER_PFUZE100_I2C_ADDR				0x08


#endif
