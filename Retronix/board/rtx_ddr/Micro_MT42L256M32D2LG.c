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
#include <asm/imx-common/mxc_i2c.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/boot_mode.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>

#ifdef CONFIG_SPL_BUILD


#include <asm/arch/mx6-ddr.h>
#include <spl.h>
#include <libfdt.h>


/* Micron MT42L256M32D2LG */
struct mx6_lpddr2_cfg rtx_ddr_chip_info = {
	.mem_speed = 800,
	.density   = 4,
	.width     = 32,
	.banks     = 8,
	.rowaddr   = 14,
	.coladdr   = 10,
	.trcd_lp   = 2000,
	.trppb_lp  = 2000,
	.trpab_lp  = 2250,
	.trasmin   = 4200,
};

struct mx6_ddr_sysinfo rtx_ddr_sysinfo = {
	/* width of data bus:0=16,1=32,2=64 */
#if defined(CONFIG_RTX_DDR_DATA_WIDTH_16)
	.dsize = 0,
#else
#if defined(CONFIG_RTX_DDR_DATA_WIDTH_32)
	.dsize = 1,
#else
#if defined(CONFIG_RTX_DDR_DATA_WIDTH_64)
	.dsize = 2,
#else
	.dsize = 2,
#endif
#endif
	/* config for full 4GB range so that get_mem_size() works */
	.cs_density = 32, 		/* 32Gb per CS */
	/* single chip select */
	.ncs = 1,
	.cs1_mirror = 0,
	.rtt_wr = 1, 			/*DDR3_RTT_60_OHM*/	/* RTT_Wr = RZQ/4 */
	.rtt_nom = 1, 			/*DDR3_RTT_60_OHM*/	/* RTT_Nom = RZQ/4 */
	.walat = 1,				/* Write additional latency */
	.ralat = 5,				/* Read additional latency */
	.mif3_mode = 3,			/* Command prediction working mode */
	.bi_on = 1,				/* Bank interleaving enabled */
	.sde_to_rst = 0x10,		/* 14 cycles, 200us (JEDEC default) */
	.rst_to_cke = 0x23,		/* 33 cycles, 500us (JEDEC default) */
	.ddr_type = DDR_TYPE_LPDDR2,
};


#endif
