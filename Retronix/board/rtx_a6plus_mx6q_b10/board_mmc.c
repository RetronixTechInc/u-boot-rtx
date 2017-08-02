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
#include <asm/arch/mx6-pins.h>
#include <linux/errno.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>

#ifdef CONFIG_FSL_ESDHC

#include "board_iomux.h"

struct fsl_esdhc_cfg usdhc_cfg[2] = {
	{USDHC4_BASE_ADDR},
	{USDHC3_BASE_ADDR},
};

int board_mmc_get_env_dev(int devno)
{
	int const iDevNoTable[] = {
		/* 0 1 2 3 4 5 6 7*/
		0 , 0 , 1 , 0 , 0 , 0 , 0 , 0
	} ;
	
	debug("mmc num: %d\n",devno);
	
	if ( devno >= 8 )
	{
		return ( 0 ) ;
	}
	if ( devno < 0 )
	{
		return ( 0 ) ;
	}
	
	debug("trans mmc num: %d\n",iDevNoTable[devno]);
	
	return ( iDevNoTable[devno] ) ;
}

int board_mmc_getcd(struct mmc *mmc)
{
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;

	switch (cfg->esdhc_base) 
	{
		case USDHC3_BASE_ADDR:
			#if defined(BOARD_PAD_SD3_CD)
				ret = !gpio_get_value(BOARD_PAD_SD3_CD);
			#else
				ret = 1;
			#endif
			break;
		case USDHC4_BASE_ADDR:
			ret = 1; /* eMMC/uSDHC4 is always present */
			break;
	}

	return ret;
}

int board_mmc_init(bd_t *bis)
{
	int ret = 0;
		
	usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC4_CLK);
	usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);

	#if defined(BOARD_PAD_SD3_CD)
		gpio_direction_input(BOARD_PAD_SD3_CD);
	#endif
	
	ret = fsl_esdhc_initialize(bis, &usdhc_cfg[0]) ;
	ret|= fsl_esdhc_initialize(bis, &usdhc_cfg[1]) ;
	
	return ret;
}


#endif
