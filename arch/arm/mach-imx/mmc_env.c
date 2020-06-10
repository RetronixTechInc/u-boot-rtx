/*
 * Copyright (C) 2017 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/sys_proto.h>
#include <asm/io.h>
#include <asm/mach-imx/boot_mode.h>

__weak int board_mmc_get_env_dev(int devno)
{
	return CONFIG_SYS_MMC_ENV_DEV;
}

int mmc_get_env_dev(void)
{
	struct bootrom_sw_info **p =
		(struct bootrom_sw_info **)(ulong)ROM_SW_INFO_ADDR;
	int devno = (*p)->boot_dev_instance;
	u8 boot_type = (*p)->boot_dev_type;

	/* If not boot from sd/mmc, use default value */
	if ((boot_type != BOOT_TYPE_SD) && (boot_type != BOOT_TYPE_MMC))
		return env_get_ulong("mmcdev", 10, CONFIG_SYS_MMC_ENV_DEV);

	return board_mmc_get_env_dev(devno);
#if 0
	u32 soc_sbmr1 = readl(SRC_BASE_ADDR + 0x58);
	u32 dev_no;
	u32 bootsel;

	bootsel = (soc_sbmr1 & 0x00007000) >> 12 ;

	/* If not boot from sd/mmc, use default value */
	if ((bootsel != 1) && (bootsel != 2))
		return CONFIG_SYS_MMC_ENV_DEV;

	/* BOOT_CFG[11] and BOOT_CFG[10] */
	dev_no = (soc_sbmr1 & 0x00000C00) >> 10;

	/* need ubstract 1 to map to the mmc device id
	 * see the comments in board_mmc_init function
	 */

	dev_no--;

	return dev_no;
#endif
}
