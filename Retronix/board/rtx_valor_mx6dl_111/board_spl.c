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

extern struct mx6_ddr3_cfg rtx_ddr_chip_info ;
extern struct mx6_ddr_sysinfo rtx_ddr_sysinfo ;

#ifdef CONFIG_SPL_OS_BOOT
	int spl_start_uboot(void)
	{
		return 1 ;
	}
#endif

void board_boot_order(u32 *spl_boot_list)
{
	switch (spl_boot_device()) {
	case BOOT_DEVICE_MMC2:
	case BOOT_DEVICE_MMC1:
		spl_boot_list[0] = BOOT_DEVICE_MMC2;
		spl_boot_list[1] = BOOT_DEVICE_MMC1;
		break;
	}
}

/* configure MX6SOLO/DUALLITE mmdc DDR io registers */
const struct mx6sdl_iomux_ddr_regs mx6sdl_ddr_ioregs = {
	/* SDCLK[0:1], CAS, RAS, Reset: Differential input, 40ohm */
	.dram_sdclk_0 = 0x00020030,
	.dram_sdclk_1 = 0x00020030,
	.dram_cas = 0x00020030,
	.dram_ras = 0x00020030,
	.dram_reset = 0x00020030,
	/* SDCKE[0:1]: 100k pull-up */
	.dram_sdcke0 = 0x00003000,
	.dram_sdcke1 = 0x00003000,
	/* SDBA2: pull-up disabled */
	.dram_sdba2 = 0x00000000,
	/* SDODT[0:1]: 100k pull-up, 40 ohm */
	.dram_sdodt0 = 0x00003030,
	.dram_sdodt1 = 0x00003030,
	/* SDQS[0:7]: Differential input, 40 ohm */
	.dram_sdqs0 = 0x00000030,
	.dram_sdqs1 = 0x00000030,
	.dram_sdqs2 = 0x00000030,
	.dram_sdqs3 = 0x00000030,
	.dram_sdqs4 = 0x00000030,
	.dram_sdqs5 = 0x00000030,
	.dram_sdqs6 = 0x00000030,
	.dram_sdqs7 = 0x00000030,

	/* DQM[0:7]: Differential input, 40 ohm */
	.dram_dqm0 = 0x00020030,
	.dram_dqm1 = 0x00020030,
	.dram_dqm2 = 0x00020030,
	.dram_dqm3 = 0x00020030,
	.dram_dqm4 = 0x00020030,
	.dram_dqm5 = 0x00020030,
	.dram_dqm6 = 0x00020030,
	.dram_dqm7 = 0x00020030,
};

/* configure MX6SOLO/DUALLITE mmdc GRP io registers */
struct mx6sdl_iomux_grp_regs mx6sdl_grp_ioregs = {
	/* DDR3 */
	.grp_ddr_type = 0x000c0000,
	/* SDQS[0:7]: Differential input, 40 ohm */
	.grp_ddrmode_ctl = 0x00020000,
	/* disable DDR pullups */
	.grp_ddrpke = 0x00000000,
	/* ADDR[00:16], SDBA[0:1]: 40 ohm */
	.grp_addds = 0x00000030,
	/* CS0/CS1/SDBA2/CKE0/CKE1/SDWE: 40 ohm */
	.grp_ctlds = 0x00000030,
	/* DATA[00:63]: Differential input, 40 ohm */
	.grp_ddrmode = 0x00020000,
	.grp_b0ds = 0x00000030,
	.grp_b1ds = 0x00000030,
	.grp_b2ds = 0x00000030,
	.grp_b3ds = 0x00000030,
	.grp_b4ds = 0x00000030,
	.grp_b5ds = 0x00000030,
	.grp_b6ds = 0x00000030,
	.grp_b7ds = 0x00000030,
};

const struct mx6_mmdc_calibration mx6_mmcd_calib = {
	.p0_mpwldectrl0  =  0x001F001F,
	.p0_mpwldectrl1  =  0x001F001F,
	.p1_mpwldectrl0  =  0x00440044,
	.p1_mpwldectrl1  =  0x00440044,
	.p0_mpdgctrl0    =  0x434B0350,
	.p0_mpdgctrl1    =  0x034C0359,
	.p1_mpdgctrl0    =  0x434B0350,
	.p1_mpdgctrl1    =  0x03650348,
	.p0_mprddlctl    =  0x4436383B,
	.p1_mprddlctl    =  0x39393341,
	.p0_mpwrdlctl    =  0x35373933,
	.p1_mpwrdlctl    =  0x48254A36,
};

static void ccgr_init(void)
{
	struct mxc_ccm_reg *ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	writel(0x00C03F3F, &ccm->CCGR0);
	writel(0x0030FC03, &ccm->CCGR1);
	writel(0x0FFFC000, &ccm->CCGR2);
	writel(0x3FF00000, &ccm->CCGR3);
	writel(0x00FFF300, &ccm->CCGR4);
	writel(0x0F0000C3, &ccm->CCGR5);
	writel(0x00000FFF, &ccm->CCGR6);
}

static void gpr_init(void)
{
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;

	/* enable AXI cache for VDOA/VPU/IPU */
	writel(0xF00000CF, &iomux->gpr[4]);
	/* set IPU AXI-id0 Qos=0xf(bypass) AXI-id1 Qos=0x7 */
	writel(0x007F007F, &iomux->gpr[6]);
	writel(0x007F007F, &iomux->gpr[7]);
}

static void spl_dram_init(void)
{
	#if defined(CONFIG_MX6QDL) || defined(CONFIG_MX6Q) || defined(CONFIG_MX6D)
		#error "Can't support these SOC"
	#endif

	#if defined(CONFIG_RTX_DDR_DATA_WIDTH_16)
		#error "Can't support the DDR Data Bus Width(16)"
	#else
	#if defined(CONFIG_RTX_DDR_DATA_WIDTH_32)
		mx6sdl_dram_iocfg(32, &mx6sdl_ddr_ioregs, &mx6sdl_grp_ioregs);
	#else
	#if defined(CONFIG_RTX_DDR_DATA_WIDTH_64)
		#error "Can't support the DDR Data Bus Width(64)"
	#else
		#error "Can't find the DDR Data Bus Width setting"
	#endif
	#endif
	#endif
	

	mx6_dram_cfg(&rtx_ddr_sysinfo, &mx6_mmcd_calib, &rtx_ddr_chip_info);	

	/* Perform DDR DRAM calibration */
	udelay(100);
	mmdc_do_write_level_calibration(&rtx_ddr_sysinfo);
	mmdc_do_dqs_calibration(&rtx_ddr_sysinfo);
}

void board_init_f(ulong dummy)
{
	/* setup AIPS and disable watchdog */
	arch_cpu_init();

	ccgr_init();
	gpr_init();

	/* iomux and setup of i2c */
	board_early_init_f();
	
	/* setup GP timer */
	timer_init();

	/* UART clocks enabled and gd valid - init serial console */
	preloader_console_init();

	/* DDR initialization */
	spl_dram_init();

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);
	
	/* load/boot image from boot device */
	board_init_r(NULL, 0);
}


#endif
