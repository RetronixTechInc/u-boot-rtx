/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Author: Fabio Estevam <fabio.estevam@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <linux/errno.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/boot_mode.h>
#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>

#include "board_iomux.h"
#include "board_i2c.h"
#include "board_display.h"
#include "board_usb.h"

#ifdef CONFIG_RTX_EFM32
	#include "../../include/efm32.h"
#endif

#ifdef CONFIG_RTX_BOOT_SYSTEM
	#include "../../include/bootsystem.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	gd->ram_size = imx_ddr_size();
	return 0;
}

/*
 * Do not overwrite the console
 * Use always serial for U-Boot console
 */
int overwrite_console(void)
{
	return 1;
}

int board_early_init_f(void)
{
	board_setup_pads();
	
	board_setup_i2cs( ) ;
			
	#if defined(CONFIG_VIDEO_IPUV3)
		board_setup_display();
	#endif

	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;
	
	#ifdef CONFIG_USB_EHCI_MX6
		board_setup_usb();
	#endif
	
	return 0;
}

int board_late_init(void)
{
	#ifdef CONFIG_RTX_BOOT_SYSTEM
		vBootMain_Init() ;
	#endif

	#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
		setenv("board_name", "PICO-B21");

		if (is_mx6dqp())
			setenv("board_rev", "MX6QP");
		else if (is_mx6dq())
			setenv("board_rev", "MX6Q");
		else if (is_mx6sdl())
			setenv("board_rev", "MX6DL");
	#endif
	
	return 0;
}

int checkboard(void)
{
	puts("Board: RTX-PICO-B21\n");
	return 0;
}

