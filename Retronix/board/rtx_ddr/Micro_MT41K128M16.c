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


/* Micron MT41K128M16JT */
struct mx6_ddr3_cfg rtx_ddr_chip_info = {
	.mem_speed = 1600,
	.density   = 2,
	.width     = 16,
	.banks     = 8,
	.rowaddr   = 14,
	.coladdr   = 10,
	.pagesz    = 2,
	.trcd      = 1375,
	.trcmin    = 4875,
	.trasmin   = 3500,
};

/* Micron MT41K256M16JT */
struct mx6_ddr3_cfg rtx_ddr_MT41K256M16JT = {
	.mem_speed = 1600,
	.density   = 4,
	.width     = 16,
	.banks     = 8,
	.rowaddr   = 15,
	.coladdr   = 10,
	.pagesz    = 2,
	.trcd      = 1375,
	.trcmin    = 4875,
	.trasmin   = 3500,
};

/* Micron MT41K256M16JT */
struct mx6_ddr3_cfg rtx_ddr_MT41K512M16JT = {
	.mem_speed = 1600,
	.density   = 8,
	.width     = 16,
	.banks     = 8,
	.rowaddr   = 15,
	.coladdr   = 10,
	.pagesz    = 1,
	.trcd      = 1375,
	.trcmin    = 5062,
	.trasmin   = 3750,
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
	.ddr_type = DDR_TYPE_DDR3,
};


#endif
