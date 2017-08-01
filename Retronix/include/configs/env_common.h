/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ENV_COMMON_CONFIG_H
	#define __ENV_COMMON_CONFIG_H
	
	/*
	#define CONFIG_BOOTARGS		"console=" CONSOLE_DEV "," __stringify(CONFIG_BAUDRATE) 
	*/

	#if defined(CONFIG_RTX_BOOT_ARGS_ROOTFS_MMC_NUM) && defined(CONFIG_RTX_BOOT_ARGS_ROOTFS_PARTITION)
		#define ENV_COMMON_ROOTFS_STORAGE "mmcrootpath=/dev/mmcblk" __stringify(CONFIG_RTX_BOOT_ARGS_ROOTFS_MMC_NUM)  "p" __stringify(CONFIG_RTX_BOOT_ARGS_ROOTFS_PARTITION)
	#endif
	
	#if defined(CONSOLE_DEV) && defined(CONFIG_BAUDRATE)
		#define ENV_COMMON_CONSOLE "console=" CONSOLE_DEV "," __stringify(CONFIG_BAUDRATE) 
	#else
		#define ENV_COMMON_CONSOLE ""
	#endif
	
	#if defined(ENV_COMMON_ROOTFS_STORAGE) 
		#define ENV_COMMON_ROOTFS "root=${mmcrootpath} rootwait rw"
	#else
		#define ENV_COMMON_ROOTFS ""
	#endif

	#if defined(CONFIG_RTX_LOADADDR)
		#define ENV_COMMON_LOADADDR "loadaddr=" __stringify(CONFIG_RTX_LOADADDR)
	#else
		#define ENV_COMMON_LOADADDR ""
	#endif
	
	#if defined(CONFIG_RTX_LOADADDR_DTB)
		#define ENV_COMMON_LOADADDR_DTB "dtb_loadaddr=" __stringify(CONFIG_RTX_LOADADDR_DTB)
	#else
		#define ENV_COMMON_LOADADDR_DTB ""
	#endif

	#if defined(CONFIG_RTX_LOADADDR_RD)
		#define ENV_COMMON_LOADADDR_RD "rd_loadaddr=" __stringify(CONFIG_RTX_LOADADDR_RD)
	#else
		#define ENV_COMMON_LOADADDR_RD ""
	#endif
	
	#if defined(CONFIG_RTX_BOOT_ARGS_DEFAULT_MAC01)
		#define ENV_COMMON_ETH_MAC01 "mac1_val=" CONFIG_RTX_BOOT_ARGS_DEFAULT_MAC01
	#else
		#define ENV_COMMON_ETH_MAC01 ""
	#endif

	#if defined(CONFIG_RTX_BOOT_ARGS_DEFAULT_MAC02)
		#define ENV_COMMON_ETH_MAC02 "mac2_val=" CONFIG_RTX_BOOT_ARGS_DEFAULT_MAC02
	#else
		#define ENV_COMMON_ETH_MAC02 ""
	#endif
	
	#if defined(CONFIG_RTX_PARTITION_0_KERNEL_OFFSET) && defined(CONFIG_RTX_PARTITION_0_KERNEL_SIZE)
		#define ENV_COMMON_READ_KERNEL "r_kernel=mmc read ${loadaddr} " __stringify(CONFIG_RTX_PARTITION_0_KERNEL_OFFSET) " " __stringify(CONFIG_RTX_PARTITION_0_KERNEL_SIZE)
	#else
		#define ENV_COMMON_READ_KERNEL ""
	#endif

	#if defined(CONFIG_RTX_PARTITION_0_KERNEL_DTB_OFFSET) && defined(CONFIG_RTX_PARTITION_0_KERNEL_DTB_SIZE)
		#define ENV_COMMON_READ_DTB "r_dtb=mmc read ${dtb_loadaddr} " __stringify(CONFIG_RTX_PARTITION_0_KERNEL_DTB_OFFSET) " " __stringify(CONFIG_RTX_PARTITION_0_KERNEL_DTB_SIZE)
	#else
		#define ENV_COMMON_READ_DTB ""
	#endif

	#if defined(CONFIG_RTX_PARTITION_0_RAM_DISK2_OFFSET) && defined(CONFIG_RTX_PARTITION_0_RAM_DISK2_SIZE)
		#define ENV_COMMON_READ_RAMDISK "r_ramdisk=mmc read ${rd_loadaddr} " __stringify(CONFIG_RTX_PARTITION_0_RAM_DISK2_OFFSET) " " __stringify(CONFIG_RTX_PARTITION_0_RAM_DISK2_SIZE)
	#else
		#define ENV_COMMON_READ_RAMDISK ""
	#endif

	#if defined(CONFIG_RTX_BOOT_ARGS_ROOTFS_MMC_NUM)
		#define ENV_COMMON_MMC_NUMBER "mmc_num=" __stringify(CONFIG_RTX_BOOT_ARGS_ROOTFS_MMC_NUM)
	#else
		#define ENV_COMMON_MMC_NUMBER ""
	#endif
	
	#if defined(CONFIG_RTX_BOOT_ARGS_VMALLOC)
		#if defined(CONFIG_RTX_BOOT_ARGS_GPU_MEM)
			#define ENV_COMMON_SET_MEM "set_mem=setenv bootargs ${bootargs} gpumem=" __stringify(CONFIG_RTX_BOOT_ARGS_GPU_MEM) "M vmalloc=" __stringify(CONFIG_RTX_BOOT_ARGS_VMALLOC) "M"
		#else
			#define ENV_COMMON_SET_MEM "set_mem=setenv bootargs ${bootargs} vmalloc=" __stringify(CONFIG_RTX_BOOT_ARGS_VMALLOC) "M"
		#endif
	#else
		#if defined(CONFIG_RTX_BOOT_ARGS_GPU_MEM)
			#define ENV_COMMON_SET_MEM "set_mem=setenv bootargs ${bootargs} gpumem=" __stringify(CONFIG_RTX_BOOT_ARGS_GPU_MEM) "M"
		#else
			#define ENV_COMMON_SET_MEM ""
		#endif
	#endif

	#if defined(CONFIG_RTX_BOOT_ARGS_VGA_EN) && defined(CONFIG_RTX_BOOT_ARGS_VGA)
		#define ENV_COMMON_DISPLAY_VGA "vga=setenv bootargs ${bootargs} video=mxcfb0:dev=lcd," CONFIG_RTX_BOOT_ARGS_VGA " video=mxcfb1:off video=mxcfb2:off"
	#else
		#define ENV_COMMON_DISPLAY_VGA ""
	#endif
	
	#if defined(CONFIG_RTX_BOOT_ARGS_HDMI_EN) && defined(CONFIG_RTX_BOOT_ARGS_HDMI)
		#define ENV_COMMON_DISPLAY_HDMI "hdmi=setenv bootargs ${bootargs} video=mxcfb0:dev=hdmi," CONFIG_RTX_BOOT_ARGS_HDMI " video=mxcfb1:off video=mxcfb2:off"
	#else
		#define ENV_COMMON_DISPLAY_HDMI ""
	#endif

	#if defined(CONFIG_RTX_BOOT_ARGS_LVDS0_EN) && defined(CONFIG_RTX_BOOT_ARGS_LVDS0)
		#define ENV_COMMON_DISPLAY_LVDS0 "lvds0=setenv bootargs ${bootargs} video=mxcfb0:dev=ldb," CONFIG_RTX_BOOT_ARGS_LVDS0 " video=mxcfb1:off video=mxcfb2:off"
	#else
		#define ENV_COMMON_DISPLAY_LVDS0 ""
	#endif

	#if defined(CONFIG_RTX_BOOT_ARGS_LVDS1_EN) && defined(CONFIG_RTX_BOOT_ARGS_LVDS1)
		#define ENV_COMMON_DISPLAY_LVDS1 "lvds0=setenv bootargs ${bootargs} video=mxcfb0:dev=ldb," CONFIG_RTX_BOOT_ARGS_LVDS1 " video=mxcfb1:off video=mxcfb2:off"
	#else
		#define ENV_COMMON_DISPLAY_LVDS1 ""
	#endif
	
	#if defined(CONFIG_RTX_BOOT_ARGS_VIDEO1)
		#if CONFIG_RTX_BOOT_ARGS_VIDEO1 == 1
			#define ENV_COMMON_VIDEO1_SELECT "set_display=run vga" 
		#elif CONFIG_RTX_BOOT_ARGS_VIDEO1 == 2
			#define ENV_COMMON_VIDEO1_SELECT "set_display=run hdmi" 
		#elif CONFIG_RTX_BOOT_ARGS_VIDEO1 == 3
			#define ENV_COMMON_VIDEO1_SELECT "set_display=run lvds0"
		#elif CONFIG_RTX_BOOT_ARGS_VIDEO1 == 4		
			#define ENV_COMMON_VIDEO1_SELECT "set_display=run lvds1"
		#else
			#define ENV_COMMON_VIDEO1_SELECT "set_display=run hdmi"
		#endif
	#endif

	/* ============================================================== */
	
	#define	CONFIG_EXTRA_ENV_SETTINGS \
		"bootargs_base=setenv bootargs no_console_suspend\0" \
		"bootargs_console=setenv bootargs ${bootargs} " ENV_COMMON_CONSOLE "\0" \
		"bootargs_gen=setenv bootargs ${bootargs} init=/sbin/init " ENV_COMMON_ROOTFS "\0" \
		"bootcmd=run bootcmd_gen\0" \
		"bootcmd_gen=run bootargs_base set_display set_mem bootargs_console bootargs_gen ;run storage r_kernel r_dtb r_ramdisk; bootm ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}\0" \
		"bootdelay=1\0" \
		"ethact=FEC\0" \
		"ethprime=FEC\0" \
		"silent=1\0" \
		"splashpos=m,m\0" \
		"storage=mmc dev ${mmc_num}\0" \
		ENV_COMMON_LOADADDR "\0" \
		ENV_COMMON_LOADADDR_DTB "\0" \
		ENV_COMMON_LOADADDR_RD "\0" \
		ENV_COMMON_ETH_MAC01 "\0" \
		ENV_COMMON_ETH_MAC02 "\0" \
		ENV_COMMON_READ_KERNEL "\0" \
		ENV_COMMON_READ_DTB "\0" \
		ENV_COMMON_READ_RAMDISK "\0" \
		ENV_COMMON_MMC_NUMBER "\0" \
		ENV_COMMON_ROOTFS_STORAGE "\0" \
		ENV_COMMON_SET_MEM "\0" \
		ENV_COMMON_DISPLAY_VGA "\0" \
		ENV_COMMON_DISPLAY_HDMI "\0" \
		ENV_COMMON_DISPLAY_LVDS0 "\0" \
		ENV_COMMON_DISPLAY_LVDS1 "\0" \
		ENV_COMMON_VIDEO1_SELECT "\0" \


	/*
	#define CONFIG_BOOTCOMMAND "bootcmd=run bootcmd_gen"
	*/

#endif
