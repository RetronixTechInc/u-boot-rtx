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
#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>

#ifdef CONFIG_MXC_SPI


int board_spi_cs_gpio(unsigned bus, unsigned cs)
{
	#if defined(BOARD_PAD_SPI4_CS0)
		return (bus == 0 && cs == 0) ? (BOARD_PAD_SPI4_CS0) : -1;
	#else
		return -1 ;
	#endif
}


#endif

