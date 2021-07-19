/*
 * Copyright 2017 NXP
 */

#ifndef __MX_ANDROID_COMMON_H
#define __MX_ANDROID_COMMON_H

#define CONFIG_CI_UDC
#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_DUALSPEED

#define CONFIG_BCB_SUPPORT
#define CONFIG_CMD_READ

#define CONFIG_USB_GADGET
#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_USB_GADGET_DOWNLOAD
#define CONFIG_USB_GADGET_VBUS_DRAW     2

#define CONFIG_USB_GADGET_VENDOR_NUM        0x18d1
#define CONFIG_USB_GADGET_PRODUCT_NUM       0x0d02
#define CONFIG_USB_GADGET_MANUFACTURER	    "FSL"

#define CONFIG_USB_FUNCTION_FASTBOOT
#define CONFIG_FASTBOOT_FLASH

#define CONFIG_FSL_FASTBOOT
#define CONFIG_ANDROID_RECOVERY

#if defined CONFIG_NAND_BOOT
#define CONFIG_FASTBOOT_STORAGE_NAND
#elif defined CONFIG_SATA_BOOT
#define CONFIG_FASTBOOT_STORAGE_SATA
#else
#define CONFIG_FASTBOOT_STORAGE_MMC
#endif

#define CONFIG_CMD_BOOTA
#define CONFIG_SUPPORT_RAW_INITRD
#define CONFIG_SERIAL_TAG

#undef CONFIG_EXTRA_ENV_SETTINGS
#undef CONFIG_BOOTCOMMAND

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"bootcmd=run bootargs_base; setenv ethaddr ${mac1_val}; setenv fec_addr ${mac1_val}; boota ${fastboot_dev}\0"	\
	"bootargs_base=setenv bootargs console=ttymxc1,115200 init=/init video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 video=mxcfb1:off video=mxcfb2:off video=mxcfb3:off vmalloc=128M androidboot.console=ttymxc1 consoleblank=0 androidboot.hardware=freescale cma=320M galcore.contiguousSize=33554432 loop.max_part=7 hpddis smsc_mac=${mac2_val}\0" \
	"splashpos=m,m\0"	\
	"fdt_high=0xffffffff\0"	\
	"initrd_high=0xffffffff\0" \
	"mac1_val="CONFIG_DEFAULT_MAC01"\0" \
	"mac2_val="CONFIG_DEFAULT_MAC02"\0" \


#define CONFIG_FASTBOOT_BUF_ADDR   CONFIG_SYS_LOAD_ADDR
#define CONFIG_FASTBOOT_BUF_SIZE   0x19000000
#define CONFIG_APPEND_BOOTARGS

#endif /*MX_ANDROID_COMMON_H*/
