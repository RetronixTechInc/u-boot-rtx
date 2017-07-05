
#ifndef __RTX_PITX_MX6Q_CONFIG_H
	#define __RTX_PITX_MX6Q_CONFIG_H

	#ifdef CONFIG_SPL
		#define CONFIG_SPL_LIBCOMMON_SUPPORT
		#define CONFIG_SPL_MMC_SUPPORT

		#define CONFIG_SPL_FRAMEWORK

		/*
		 * see Figure 8-3 in IMX6DQ/IMX6SDL Reference manuals:
		 *  - IMX6SDL OCRAM (IRAM) is from 0x00907000 to 0x0091FFFF
		 *  - IMX6DQ has 2x IRAM of IMX6SDL but we intend to support IMX6SDL as well
		 *  - BOOT ROM stack is at 0x0091FFB8
		 *  - if icache/dcache is enabled (eFuse/strapping controlled) then the
		 *    IMX BOOT ROM will setup MMU table at 0x00918000, therefore we need to
		 *    fit between 0x00907000 and 0x00918000.
		 *  - Additionally the BOOT ROM loads what they consider the firmware image
		 *    which consists of a 4K header in front of us that contains the IVT, DCD
		 *    and some padding thus 'our' max size is really 0x00908000 - 0x00918000
		 *    or 64KB
		 */
		#define CONFIG_SYS_THUMB_BUILD
		#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv7/omap-common/u-boot-spl.lds"
		#define CONFIG_SPL_TEXT_BASE	0x00908000
		#define CONFIG_SPL_MAX_SIZE		0x10000
		#define CONFIG_SPL_STACK		0x0091FFB8
		#define CONFIG_SPL_LIBCOMMON_SUPPORT
		#define CONFIG_SPL_LIBGENERIC_SUPPORT
		#define CONFIG_SPL_SERIAL_SUPPORT
		#define CONFIG_SPL_I2C_SUPPORT
		#define CONFIG_SPL_GPIO_SUPPORT
		#define CONFIG_SPL_WATCHDOG_SUPPORT

		/* NAND support */
		#if defined(CONFIG_SPL_NAND_SUPPORT)
			#define CONFIG_SPL_NAND_MXS
			#define CONFIG_SPL_DMA_SUPPORT
		#endif

		/* MMC support */
		#if defined(CONFIG_SPL_MMC_SUPPORT)
			#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR		138 /* offset 69KB */
			#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS			800 /* 400 KB */
			#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION			1
			#define CONFIG_SYS_MONITOR_LEN 						(CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS/2*1024)
		#endif

		/* SATA support */
		#if defined(CONFIG_SPL_SATA_SUPPORT)
			#define CONFIG_SPL_SATA_BOOT_DEVICE				0
			#define CONFIG_SYS_SATA_FAT_BOOT_PARTITION		1
		#endif

		/* Define the payload for FAT/EXT support */
		#if defined(CONFIG_SPL_FAT_SUPPORT) || defined(CONFIG_SPL_EXT_SUPPORT)
			#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME		"u-boot.img"
			#define CONFIG_SPL_LIBDISK_SUPPORT
		#endif

		#if defined(CONFIG_MX6SX) || defined(CONFIG_MX6UL) || defined(CONFIG_MX6SL)
			#define CONFIG_SPL_BSS_START_ADDR		0x88200000
			#define CONFIG_SPL_BSS_MAX_SIZE			0x100000		/* 1 MB */
			#define CONFIG_SYS_SPL_MALLOC_START		0x88300000
			#define CONFIG_SYS_SPL_MALLOC_SIZE		0x100000		/* 1 MB */
			#define CONFIG_SYS_TEXT_BASE			0x87800000
		#else
			#define CONFIG_SPL_BSS_START_ADDR		0x18200000
			#define CONFIG_SPL_BSS_MAX_SIZE			0x100000	/* 1 MB */
			#define CONFIG_SYS_SPL_MALLOC_START		0x18300000
			#define CONFIG_SYS_SPL_MALLOC_SIZE		0x100000	/* 1 MB */
			#define CONFIG_SYS_TEXT_BASE			0x17800000
		#endif
	#endif

	#define CONFIG_RTX_EFM32
	#ifdef CONFIG_RTX_EFM32
		#define CONFIG_MCU_WDOG_BUS	2
	#endif

	#define CONFIG_MACH_TYPE		3980
	#define CONFIG_MXC_UART_BASE	UART2_BASE
	#define CONFIG_CONSOLE_DEV		"ttymxc1"
	#define CONFIG_MMCROOT			"/dev/mmcblk2p2"  /* SDHC3 */

	#if defined(CONFIG_MX6QP)
		#define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024)
	#elif defined(CONFIG_MX6Q)
		#define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024)
	#elif defined(CONFIG_MX6DL)
		#define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024)
	#elif defined(CONFIG_MX6SOLO)
		#define PHYS_SDRAM_SIZE		(512u * 1024 * 1024)
	#endif

	#ifndef CONFIG_MX6UL
		#define CONFIG_ARM_ERRATA_743622
		#if (defined(CONFIG_MX6QP) || defined(CONFIG_MX6Q) || defined(CONFIG_MX6DL)) && !defined(CONFIG_MX6SOLO)
			#define CONFIG_ARM_ERRATA_751472
			#define CONFIG_ARM_ERRATA_794072
			#define CONFIG_ARM_ERRATA_761320
			#define CONFIG_ARM_ERRATA_845369
		#endif

		#ifndef CONFIG_SYS_L2CACHE_OFF
			#define CONFIG_SYS_L2_PL310
			#define CONFIG_SYS_PL310_BASE	L2_PL310_BASE
		#endif

		#define CONFIG_MP
		#define CONFIG_GPT_TIMER
	#else
		#define CONFIG_SYSCOUNTER_TIMER
		#define CONFIG_SC_TIMER_CLK 8000000 /* 8Mhz */
	#endif
	
	#define CONFIG_BOARD_POSTCLK_INIT
	#define CONFIG_MXC_GPT_HCLK

	#ifdef CONFIG_MX6QP
		#define CONFIG_MX6Q
	#endif

	#ifdef CONFIG_MX6SOLO
		#define CONFIG_MX6DL
	#endif

	#define CONFIG_SYS_NO_FLASH

	#define CONFIG_SYS_BOOTM_LEN	0x1000000

	#include <linux/sizes.h>
	#include <asm/arch/imx-regs.h>
	#include <asm/imx-common/gpio.h>

	#ifndef CONFIG_MX6
		#define CONFIG_MX6
	#endif

	#define CONFIG_DISPLAY_BOARDINFO
	#define CONFIG_DISPLAY_CPUINFO
	#define CONFIG_SYS_FSL_CLK

	/* ATAGs */
	#define CONFIG_CMDLINE_TAG
	#define CONFIG_SETUP_MEMORY_TAGS
	#define CONFIG_INITRD_TAG
	#define CONFIG_REVISION_TAG

	/* Boot options */
	#if (defined(CONFIG_MX6SX) || defined(CONFIG_MX6SL) || defined(CONFIG_MX6UL) || defined(CONFIG_MX6SLL))
		#define CONFIG_LOADADDR		0x80800000
		#ifndef CONFIG_SYS_TEXT_BASE
			#define CONFIG_SYS_TEXT_BASE	0x87800000
		#endif
	#else
		#define CONFIG_LOADADDR		0x12000000
		#ifndef CONFIG_SYS_TEXT_BASE
			#define CONFIG_SYS_TEXT_BASE	0x17800000
		#endif
	#endif
	#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

	#ifndef CONFIG_BOOTDELAY
		#define CONFIG_BOOTDELAY	3
	#endif

	/* allow to overwrite serial and ethaddr */
	#define CONFIG_ENV_OVERWRITE
	#define CONFIG_CONS_INDEX       1
	#define CONFIG_BAUDRATE         115200

	/* Filesystems and image support */
	#define CONFIG_OF_LIBFDT
	#define CONFIG_CMD_BOOTZ
	#define CONFIG_SUPPORT_RAW_INITRD
	#define CONFIG_CMD_FS_GENERIC
	#define CONFIG_DOS_PARTITION
	#define CONFIG_CMD_EXT2
	#define CONFIG_CMD_EXT4
	#define CONFIG_CMD_EXT4_WRITE
	#define CONFIG_CMD_FAT

	/* Miscellaneous configurable options */
	#undef CONFIG_CMD_IMLS
	#define CONFIG_SYS_LONGHELP
	#define CONFIG_SYS_HUSH_PARSER
	#define CONFIG_CMDLINE_EDITING
	#define CONFIG_AUTO_COMPLETE
	#define CONFIG_SYS_CBSIZE	1024
	#define CONFIG_SYS_MAXARGS	256
	#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE

	#ifndef CONFIG_SYS_DCACHE_OFF
		#define CONFIG_CMD_CACHE
	#endif

	/* GPIO */
	#define CONFIG_MXC_GPIO

	/* MMC */
	#define CONFIG_MMC
	#define CONFIG_CMD_MMC
	#define CONFIG_GENERIC_MMC
	#define CONFIG_BOUNCE_BUFFER
	#define CONFIG_FSL_ESDHC
	#define CONFIG_FSL_USDHC
	#define CONFIG_SUPPORT_EMMC_BOOT

	/* Fuses */
	#define CONFIG_CMD_FUSE
	#define CONFIG_MXC_OCOTP

	/* LDO Bypass */
	#ifndef CONFIG_MX6SLL
		#define CONFIG_LDO_BYPASS_CHECK
	#endif

	/* uncomment for PLUGIN mode support */
	/* #define CONFIG_USE_PLUGIN */

	/* uncomment for SECURE mode support */
	/* #define CONFIG_SECURE_BOOT */

	#ifdef CONFIG_SECURE_BOOT
		#ifndef CONFIG_CSF_SIZE
			#define CONFIG_CSF_SIZE 0x4000
		#endif
	#endif

	#define CONFIG_IMX_THERMAL

	/* Size of malloc() pool */
	#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

	#define CONFIG_BOARD_EARLY_INIT_F
	#define CONFIG_BOARD_LATE_INIT

	#define CONFIG_MXC_UART

	/* MMC Configs */
	#define CONFIG_SYS_FSL_ESDHC_ADDR      0

	#define CONFIG_CMD_PING
	#define CONFIG_CMD_DHCP
	#define CONFIG_CMD_MII
	#define CONFIG_FEC_MXC
	#define CONFIG_MII
	#define IMX_FEC_BASE			ENET_BASE_ADDR
	#define CONFIG_FEC_XCV_TYPE		RGMII
	#define CONFIG_ETHPRIME			"FEC"
	#define CONFIG_FEC_MXC_PHYADDR		1

	#define CONFIG_PHYLIB
	#define CONFIG_PHY_ATHEROS

	/* Command definition */
	#define CONFIG_CMD_BMODE

	#ifdef CONFIG_SYS_BOOT_NAND
		#define CONFIG_MFG_NAND_PARTITION "mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),1m(misc),-(rootfs) "
	#else
		#define CONFIG_MFG_NAND_PARTITION ""
	#endif

	#define CONFIG_MFG_ENV_SETTINGS \
		"mfgtool_args=setenv bootargs console=" CONFIG_CONSOLE_DEV ",115200 " \
		"rdinit=/linuxrc " \
		"g_mass_storage.stall=0 g_mass_storage.removable=1 " \
		"g_mass_storage.file=/fat g_mass_storage.ro=1 " \
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
		"g_mass_storage.iSerialNumber=\"\" "\
		"enable_wait_mode=off "\
		CONFIG_MFG_NAND_PARTITION \
		"\0" \
		"initrd_addr=0x12C00000\0" \
		"initrd_high=0xffffffff\0" \
		"bootcmd_mfg=run mfgtool_args;bootz ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \


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

	#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

	#if defined(CONFIG_SYS_BOOT_NAND)
		/*
		 * The dts also enables the WEIN NOR which is mtd0.
		 * So the partions' layout for NAND is:
		 *     mtd1: 16M      (uboot)
		 *     mtd2: 16M      (kernel)
		 *     mtd3: 16M      (dtb)
		 *     mtd4: left     (rootfs)
		*/
	#define CONFIG_EXTRA_ENV_SETTINGS \
		CONFIG_MFG_ENV_SETTINGS \
		"fdt_addr=0x18000000\0" \
		"fdt_high=0xffffffff\0"	  \
		"bootargs=console=" CONFIG_CONSOLE_DEV ",115200 ubi.mtd=5 "  \
			"root=ubi0:rootfs rootfstype=ubifs "		     \
			"mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),1m(misc),-(rootfs)\0"\
		"bootcmd=nand read ${loadaddr} 0x4000000 0x800000;"\
			"nand read ${fdt_addr} 0x5000000 0x100000;"\
			"bootz ${loadaddr} - ${fdt_addr}\0"

	#elif defined(CONFIG_SYS_BOOT_SATA)

		#define CONFIG_EXTRA_ENV_SETTINGS \
			CONFIG_MFG_ENV_SETTINGS \
			"fdt_addr=0x18000000\0" \
			"fdt_high=0xffffffff\0"   \
			"bootargs=console=" CONFIG_CONSOLE_DEV ",115200 \0"\
			"bootargs_sata=setenv bootargs ${bootargs} " \
				"root=/dev/sda1 rootwait rw \0" \
			"bootcmd_sata=run bootargs_sata; sata init; " \
				"sata read ${loadaddr} 0x800  0x4000; " \
				"sata read ${fdt_addr} 0x8000 0x800; " \
				"bootz ${loadaddr} - ${fdt_addr} \0" \
			"bootcmd=run bootcmd_sata \0"

	#else

		#define CONFIG_EXTRA_ENV_SETTINGS \
			CONFIG_MFG_ENV_SETTINGS \
			"epdc_waveform=epdc_splash.bin\0" \
			"script=boot.scr\0" \
			"image=zImage\0" \
			"fdt_file=undefined\0" \
			"fdt_addr=0x18000000\0" \
			"boot_fdt=try\0" \
			"ip_dyn=yes\0" \
			"console=" CONFIG_CONSOLE_DEV "\0" \
			"dfuspi=dfu 0 sf 0:0:10000000:0\0" \
			"dfu_alt_info_spl=spl raw 0x400\0" \
			"dfu_alt_info_img=u-boot raw 0x10000\0" \
			"dfu_alt_info=spl raw 0x400\0" \
			"fdt_high=0xffffffff\0"	  \
			"initrd_high=0xffffffff\0" \
			"mmcdev=" __stringify(CONFIG_SYS_MMC_ENV_DEV) "\0" \
			"mmcpart=1\0" \
			"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
			"mmcautodetect=yes\0" \
			"update_sd_firmware=" \
				"if test ${ip_dyn} = yes; then " \
					"setenv get_cmd dhcp; " \
				"else " \
					"setenv get_cmd tftp; " \
				"fi; " \
				"if mmc dev ${mmcdev}; then "	\
					"if ${get_cmd} ${update_sd_firmware_filename}; then " \
						"setexpr fw_sz ${filesize} / 0x200; " \
						"setexpr fw_sz ${fw_sz} + 1; "	\
						"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
					"fi; "	\
				"fi\0" \
			EMMC_ENV	  \
			"smp=" CONFIG_SYS_NOSMP "\0"\
			"mmcargs=setenv bootargs console=${console},${baudrate} ${smp} " \
				"root=${mmcroot}\0" \
			"loadbootscript=" \
				"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
			"bootscript=echo Running bootscript from mmc ...; " \
				"source\0" \
			"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
			"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
			"mmcboot=echo Booting from mmc ...; " \
				"run mmcargs; " \
				"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
					"if run loadfdt; then " \
						"bootz ${loadaddr} - ${fdt_addr}; " \
					"else " \
						"if test ${boot_fdt} = try; then " \
							"bootz; " \
					"else " \
						"echo WARN: Cannot load the DT; " \
					"fi; " \
				"fi; " \
			"else " \
				"bootz; " \
			"fi;\0" \
			"netargs=setenv bootargs console=${console},${baudrate} ${smp} " \
				"root=/dev/nfs " \
				"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
			"netboot=echo Booting from net ...; " \
				"run netargs; " \
				"if test ${ip_dyn} = yes; then " \
					"setenv get_cmd dhcp; " \
				"else " \
					"setenv get_cmd tftp; " \
				"fi; " \
				"${get_cmd} ${image}; " \
				"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
					"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
						"bootz ${loadaddr} - ${fdt_addr}; " \
					"else " \
						"if test ${boot_fdt} = try; then " \
							"bootz; " \
						"else " \
							"echo WARN: Cannot load the DT; " \
						"fi; " \
					"fi; " \
				"else " \
					"bootz; " \
				"fi;\0" \
				"findfdt="\
					"if test $fdt_file = undefined; then " \
						"if test $board_name = SABREAUTO && test $board_rev = MX6QP; then " \
							"setenv fdt_file imx6qp-sabreauto.dtb; fi; " \
						"if test $board_name = SABREAUTO && test $board_rev = MX6Q; then " \
							"setenv fdt_file imx6q-sabreauto.dtb; fi; " \
						"if test $board_name = SABREAUTO && test $board_rev = MX6DL; then " \
							"setenv fdt_file imx6dl-sabreauto.dtb; fi; " \
						"if test $board_name = SABRESD && test $board_rev = MX6QP; then " \
							"setenv fdt_file imx6qp-sabresd.dtb; fi; " \
						"if test $board_name = SABRESD && test $board_rev = MX6Q; then " \
							"setenv fdt_file imx6q-sabresd.dtb; fi; " \
						"if test $board_name = SABRESD && test $board_rev = MX6DL; then " \
							"setenv fdt_file imx6dl-sabresd.dtb; fi; " \
						"if test $fdt_file = undefined; then " \
							"echo WARNING: Could not determine dtb to use; fi; " \
					"fi;\0" \


		#define CONFIG_BOOTCOMMAND \
			"run findfdt;" \
			"mmc dev ${mmcdev};" \
			"if mmc rescan; then " \
				"if run loadbootscript; then " \
				"run bootscript; " \
				"else " \
					"if run loadimage; then " \
						"run mmcboot; " \
					"else run netboot; " \
					"fi; " \
				"fi; " \
			"else run netboot; fi"
	#endif

	#define CONFIG_ARP_TIMEOUT     200UL

	#define CONFIG_CMD_MEMTEST
	#define CONFIG_SYS_MEMTEST_START       0x10000000
	#define CONFIG_SYS_MEMTEST_END         0x10010000
	#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

	#define CONFIG_STACKSIZE               (128 * 1024)

	/* Physical Memory Map */
	#define CONFIG_NR_DRAM_BANKS           1
	#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

	#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
	#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
	#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

	#define CONFIG_SYS_INIT_SP_OFFSET \
		(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
	#define CONFIG_SYS_INIT_SP_ADDR \
		(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

	/* Environment organization */
	#define CONFIG_ENV_SIZE			(8 * 1024)

	#ifndef CONFIG_SYS_NOSMP
		#define CONFIG_SYS_NOSMP
	#endif

	#if defined CONFIG_SYS_BOOT_SPINOR
		#define CONFIG_SYS_USE_SPINOR
		#define CONFIG_ENV_IS_IN_SPI_FLASH
	#elif defined CONFIG_SYS_BOOT_EIMNOR
		#define CONFIG_SYS_USE_EIMNOR
		#define CONFIG_ENV_IS_IN_FLASH
	#elif defined CONFIG_SYS_BOOT_NAND
		#define CONFIG_SYS_USE_NAND
		#define CONFIG_ENV_IS_IN_NAND
	#elif defined CONFIG_SYS_BOOT_SATA
		#define CONFIG_ENV_IS_IN_SATA
		#define CONFIG_CMD_SATA
	#else
		#define CONFIG_ENV_IS_IN_MMC
	#endif

	#ifdef CONFIG_CMD_SATA
		#define CONFIG_DWC_AHSATA
		#define CONFIG_SYS_SATA_MAX_DEVICE	1
		#define CONFIG_DWC_AHSATA_PORT_ID	0
		#define CONFIG_DWC_AHSATA_BASE_ADDR	SATA_ARB_BASE_ADDR
		#define CONFIG_LBA48
		#define CONFIG_LIBATA
	#endif

	#ifdef CONFIG_SYS_USE_SPINOR
		#define CONFIG_CMD_SF
		#define CONFIG_SPI_FLASH
		#define CONFIG_SPI_FLASH_STMICRO
		#define CONFIG_MXC_SPI
		#define CONFIG_SF_DEFAULT_BUS  0
		#define CONFIG_SF_DEFAULT_SPEED 20000000
		#define CONFIG_SF_DEFAULT_MODE (SPI_MODE_0)
	#endif

	#ifdef CONFIG_SYS_USE_EIMNOR
		#undef CONFIG_SYS_NO_FLASH
		#define CONFIG_SYS_FLASH_BASE           WEIM_ARB_BASE_ADDR
		#define CONFIG_SYS_FLASH_SECT_SIZE      (128 * 1024)
		#define CONFIG_SYS_MAX_FLASH_BANKS 1    /* max number of memory banks */
		#define CONFIG_SYS_MAX_FLASH_SECT 256   /* max number of sectors on one chip */
		#define CONFIG_SYS_FLASH_CFI            /* Flash memory is CFI compliant */
		#define CONFIG_FLASH_CFI_DRIVER         /* Use drivers/cfi_flash.c */
		#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE /* Use buffered writes*/
		#define CONFIG_SYS_FLASH_EMPTY_INFO
	#endif

	#ifdef CONFIG_SYS_USE_NAND
		/* NAND flash command */
		#define CONFIG_CMD_NAND
		#define CONFIG_CMD_NAND_TRIMFFS

		/* NAND stuff */
		#define CONFIG_NAND_MXS
		#define CONFIG_SYS_MAX_NAND_DEVICE     1
		#define CONFIG_SYS_NAND_BASE           0x40000000
		#define CONFIG_SYS_NAND_5_ADDR_CYCLE
		#define CONFIG_SYS_NAND_ONFI_DETECTION

		/* DMA stuff, needed for GPMI/MXS NAND support */
		#define CONFIG_APBH_DMA
		#define CONFIG_APBH_DMA_BURST
		#define CONFIG_APBH_DMA_BURST8
	#endif


	#if defined(CONFIG_ENV_IS_IN_MMC)
		#define CONFIG_ENV_OFFSET		(12288 * 1024)
	#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
		#define CONFIG_ENV_OFFSET              (768 * 1024)
		#define CONFIG_ENV_SECT_SIZE           (64 * 1024)
		#define CONFIG_ENV_SPI_BUS             CONFIG_SF_DEFAULT_BUS
		#define CONFIG_ENV_SPI_CS              CONFIG_SF_DEFAULT_CS
		#define CONFIG_ENV_SPI_MODE            CONFIG_SF_DEFAULT_MODE
		#define CONFIG_ENV_SPI_MAX_HZ          CONFIG_SF_DEFAULT_SPEED
	#elif defined(CONFIG_ENV_IS_IN_FLASH)
		#undef CONFIG_ENV_SIZE
		#define CONFIG_ENV_SIZE                        CONFIG_SYS_FLASH_SECT_SIZE
		#define CONFIG_ENV_SECT_SIZE           CONFIG_SYS_FLASH_SECT_SIZE
		#define CONFIG_ENV_OFFSET              (6 * CONFIG_SYS_FLASH_SECT_SIZE)
	#elif defined(CONFIG_ENV_IS_IN_NAND)
		#undef CONFIG_ENV_SIZE
		#define CONFIG_ENV_OFFSET              (60 << 20)
		#define CONFIG_ENV_SECT_SIZE           (128 << 10)
		#define CONFIG_ENV_SIZE                        CONFIG_ENV_SECT_SIZE
	#elif defined(CONFIG_ENV_IS_IN_SATA)
		#define CONFIG_ENV_OFFSET		(768 * 1024)
		#define CONFIG_SATA_ENV_DEV		0
		#define CONFIG_SYS_DCACHE_OFF /* remove when sata driver support cache */
	#endif

	/* I2C Configs */
	#define CONFIG_CMD_I2C
	#define CONFIG_SYS_I2C
	#define CONFIG_SYS_I2C_MXC
	#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
	#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
	#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
	#define CONFIG_SYS_I2C_SPEED		100000

	/* PMIC */
	#define CONFIG_POWER
	#define CONFIG_POWER_I2C
	#define CONFIG_POWER_PFUZE100
	#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08

	/* Framebuffer */
	/*
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
		#define CONFIG_IPUV3_CLK 198000000
	#else
		#define CONFIG_IPUV3_CLK 264000000
	#endif
	#define CONFIG_IMX_HDMI
	#define CONFIG_IMX_VIDEO_SKIP
	*/
	
	#if defined(CONFIG_ANDROID_SUPPORT)
		#define CONFIG_CI_UDC
		#define CONFIG_USBD_HS
		#define CONFIG_USB_GADGET_DUALSPEED

		#define CONFIG_USB_GADGET
		#define CONFIG_CMD_USB_MASS_STORAGE
		#define CONFIG_USB_FUNCTION_MASS_STORAGE
		#define CONFIG_USB_GADGET_DOWNLOAD
		#define CONFIG_USB_GADGET_VBUS_DRAW	2

		#define CONFIG_G_DNL_VENDOR_NUM		0x18d1
		#define CONFIG_G_DNL_PRODUCT_NUM	0x0d02
		#define CONFIG_G_DNL_MANUFACTURER	"FSL"

		#define CONFIG_USB_FUNCTION_FASTBOOT
		#define CONFIG_CMD_FASTBOOT
		#define CONFIG_ANDROID_BOOT_IMAGE
		#define CONFIG_FASTBOOT_FLASH

		#define CONFIG_FSL_FASTBOOT
		#define CONFIG_ANDROID_RECOVERY

		#if defined CONFIG_SYS_BOOT_NAND
			#define CONFIG_FASTBOOT_STORAGE_NAND
		#elif defined CONFIG_SYS_BOOT_SATA
			#define CONFIG_FASTBOOT_STORAGE_SATA
			#define CONFIG_FASTBOOT_SATA_NO 0
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

		#undef CONFIG_EXTRA_ENV_SETTINGS
		#undef CONFIG_BOOTCOMMAND

		#define CONFIG_EXTRA_ENV_SETTINGS					\
			"splashpos=m,m\0"	  \
			"fdt_high=0xffffffff\0"	  \
			"initrd_high=0xffffffff\0" \

		#define CONFIG_FASTBOOT_BUF_ADDR   CONFIG_SYS_LOAD_ADDR
		#define CONFIG_FASTBOOT_BUF_SIZE   0x19000000		
	#else

		#ifndef CONFIG_SPL
			#define CONFIG_CI_UDC
			#define CONFIG_USBD_HS
			#define CONFIG_USB_GADGET_DUALSPEED

			#define CONFIG_USB_GADGET
			#define CONFIG_CMD_USB_MASS_STORAGE
			#define CONFIG_USB_FUNCTION_MASS_STORAGE
			#define CONFIG_USB_GADGET_DOWNLOAD
			#define CONFIG_USB_GADGET_VBUS_DRAW	2

			#define CONFIG_G_DNL_VENDOR_NUM		0x0525
			#define CONFIG_G_DNL_PRODUCT_NUM	0xa4a5
			#define CONFIG_G_DNL_MANUFACTURER	"FSL"

			/* USB Device Firmware Update support */
			#define CONFIG_CMD_DFU
			#define CONFIG_USB_FUNCTION_DFU
			#define CONFIG_DFU_MMC
			#if defined(CONFIG_SYS_USE_SPINOR)
				#define CONFIG_DFU_SF
			#endif
		#endif
	#endif

	#define CONFIG_SYS_FSL_USDHC_NUM	2
	#define CONFIG_SYS_MMC_ENV_DEV		1	/* SDHC3 */
	#define CONFIG_SYS_MMC_ENV_PART     0   /* user partition */

	#ifdef CONFIG_SYS_USE_SPINOR
		#define CONFIG_SF_DEFAULT_CS    0
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
	/* #define CONFIG_CMD_PCI */
	#ifdef CONFIG_CMD_PCI
		#define CONFIG_PCI
		#define CONFIG_PCI_PNP
		#define CONFIG_PCI_SCAN_SHOW
		#define CONFIG_PCIE_IMX
		#define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(7, 12)
		#define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(3, 19)
	#endif

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
	#endif /* CONFIG_SPLASH_SCREEN && CONFIG_MXC_EPDC */

#endif
