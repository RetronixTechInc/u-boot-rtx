/*
 * Copyright (C) 2011 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the MX53SMD Freescale board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
    #define __CONFIG_H

    #define CONFIG_MX53

    #define CONFIG_DISPLAY_CPUINFO
    #define CONFIG_DISPLAY_BOARDINFO

    #define CONFIG_MACH_TYPE	MACH_TYPE_MX53_SMD

    #include <asm/arch/imx-regs.h>

    #define CONFIG_CMDLINE_TAG			/* enable passing of ATAGs */
    #define CONFIG_SETUP_MEMORY_TAGS
    #define CONFIG_INITRD_TAG
    #define CONFIG_REVISION_TAG

    #define CONFIG_SYS_GENERIC_BOARD

    /* Size of malloc() pool */
    #define CONFIG_SYS_MALLOC_LEN		(10 * 1024 * 1024)

    #define CONFIG_BOARD_EARLY_INIT_F
    #define CONFIG_MXC_GPIO

    #define CONFIG_MXC_UART
    #define CONFIG_MXC_UART_BASE	UART4_BASE

    /* I2C Configs */
    #define CONFIG_CMD_I2C
    #define CONFIG_SYS_I2C
    #define CONFIG_SYS_I2C_MXC

    /* MMC Configs */
    #define CONFIG_FSL_ESDHC
    #define CONFIG_SYS_FSL_ESDHC_ADDR	0
    #define CONFIG_SYS_FSL_ESDHC_NUM	1

    #define CONFIG_MMC
    #define CONFIG_CMD_MMC
    #define CONFIG_GENERIC_MMC
    #define CONFIG_CMD_FAT
    #define CONFIG_DOS_PARTITION
    #define CONFIG_CMD_EXT2
    #define CONFIG_CMD_EXT4


    /* Eth Configs */
    #define CONFIG_HAS_ETH1
    #define CONFIG_MII

    #define CONFIG_FEC_MXC
    #define IMX_FEC_BASE	FEC_BASE_ADDR
    #define CONFIG_FEC_MXC_PHYADDR	0x1F

    #define CONFIG_CMD_PING
    #define CONFIG_CMD_DHCP
    #define CONFIG_CMD_MII
    #define CONFIG_CMD_NET

    /* allow to overwrite serial and ethaddr */
    #define CONFIG_ENV_OVERWRITE
    #define CONFIG_CONS_INDEX		1
    #define CONFIG_BAUDRATE			115200

    /* Command definition */
    #include <config_cmd_default.h>

    #undef CONFIG_CMD_IMLS

    #define CONFIG_BOOTDELAY	3

    #define CONFIG_ETHPRIME		"FEC0"

    #define CONFIG_LOADADDR		    0x70800000	/* loadaddr env var */
    #define CONFIG_RD_LOADADDR      0x71000000
    #define CONFIG_DTB_LOADADDR     0x70F00000
    #define CONFIG_SYS_TEXT_BASE    0x77800000

    #define CONFIG_VERSION_STRING "rtx-bishop-mx53"

    #define	CONFIG_EXTRA_ENV_SETTINGS					\
            "netdev=eth0\0"						\
            "ethprime=FEC0\0"					\
            "bootfile=uImage\0"					\
            "loadaddr=0x70800000\0"					\
            "rd_loadaddr=0x70F00000\0"				\
            "bootcmd=run bootcmd_nand\0"				\
            "bootcmd_nand=run bootargs_nand;mmc dev 1;mmc read ${loadaddr} 0x6800 0x2000;bootm\0" 			\
            "bootargs_nand=setenv bootargs console=ttymxc3 video=mxcdi0fb:RGB24,1024x768@60 root=/dev/mmcblk0p1 rootwait rw gpu_nommu gpu_memory=64M\0"						\
            "bootcmd_ramdisk=run bootargs_ramdisk;mmc dev 1;mmc read ${loadaddr} 0x800 0x2000;mmc read ${rd_loadaddr} 0x3000 0x2000;bootm ${loadaddr} ${rd_loadaddr}\0"				\
            "bootargs_ramdisk=setenv bootargs console=ttymxc3 root=/dev/ram0 rootwait rw rdinit=/sbin/init\0"	\


    #define CONFIG_ARP_TIMEOUT	200UL

    /* Miscellaneous configurable options */
    #define CONFIG_SYS_LONGHELP		/* undef to save memory */
    #define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
    #define CONFIG_SYS_PROMPT_HUSH_PS2     	"> "
    #define CONFIG_SYS_PROMPT               "BI-SHOP(MX53) > "
    #define CONFIG_AUTO_COMPLETE
    #define CONFIG_SYS_CBSIZE		        1024	/* Console I/O Buffer Size */

    /* Print Buffer Size */
    #define CONFIG_SYS_PBSIZE               (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
    #define CONFIG_SYS_MAXARGS	            16	/* max number of command args */
    #define CONFIG_SYS_BARGSIZE             CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */

    #define CONFIG_SYS_MEMTEST_START       0x70000000
    #define CONFIG_SYS_MEMTEST_END         0x70010000

    #define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

    #define CONFIG_CMDLINE_EDITING

    /* Physical Memory Map */
    #define CONFIG_NR_DRAM_BANKS	2
    #define PHYS_SDRAM_1		CSD0_BASE_ADDR
    #define PHYS_SDRAM_1_SIZE	(512 * 1024 * 1024)
    #define PHYS_SDRAM_2		0
    #define PHYS_SDRAM_2_SIZE	0
    #define PHYS_SDRAM_SIZE         (PHYS_SDRAM_1_SIZE + PHYS_SDRAM_2_SIZE)

    #define CONFIG_SYS_SDRAM_BASE		(PHYS_SDRAM_1)
    #define CONFIG_SYS_INIT_RAM_ADDR	(IRAM_BASE_ADDR)
    #define CONFIG_SYS_INIT_RAM_SIZE	(IRAM_SIZE)

    #define CONFIG_SYS_INIT_SP_OFFSET \
        (CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
    #define CONFIG_SYS_INIT_SP_ADDR \
        (CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

    /* FLASH and environment organization */
    #define CONFIG_SYS_NO_FLASH

    #define CONFIG_ENV_OFFSET      (12288 * 1024)
    #define CONFIG_ENV_SIZE        (8 * 1024)
    #define CONFIG_ENV_IS_IN_MMC
    #define CONFIG_SYS_MMC_ENV_DEV 0

    #define CONFIG_OF_LIBFDT

	/* USB Configs */
	#define CONFIG_CMD_USB
    #ifdef CONFIG_CMD_USB
        #define CONFIG_USB_EHCI
        #define CONFIG_USB_EHCI_MX5
        #define CONFIG_USB_STORAGE
        #define CONFIG_USB_HOST_ETHER
        #define CONFIG_USB_ETHER_ASIX
        #define CONFIG_USB_ETHER_MCS7830
        #define CONFIG_USB_ETHER_SMSC95XX
        #define CONFIG_MXC_USB_PORT		1
        #define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
        #define CONFIG_MXC_USB_FLAGS		0
    #endif
    
    #define CONFIG_RTX_EFM32
    #define CONFIG_RTX_SET_TIMEOUT
    #define CONFIG_RTX_EFM32_BISHOP

#endif				/* __CONFIG_H */
