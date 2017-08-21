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
#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>


#define I2C_PAD MUX_PAD_CTRL(0x1b8b1)

static struct i2c_pads_info board_i2c_pad_info0 = {
	.scl = {
		.i2c_mode  = IOMUX_PADS(PAD_CSI0_DAT9__I2C1_SCL   | I2C_PAD) ,
		.gpio_mode = IOMUX_PADS(PAD_CSI0_DAT9__GPIO5_IO27 | I2C_PAD) ,
		.gp        = IMX_GPIO_NR(5, 27)
	},
	.sda = {
		.i2c_mode  = IOMUX_PADS(PAD_CSI0_DAT8__I2C1_SDA   | I2C_PAD ),
		.gpio_mode = IOMUX_PADS(PAD_CSI0_DAT8__GPIO5_IO26 | I2C_PAD ),
		.gp        = IMX_GPIO_NR(5, 26)
	}
};

static struct i2c_pads_info board_i2c_pad_info1 = {
	.scl = {
		.i2c_mode  = IOMUX_PADS(PAD_KEY_COL3__I2C2_SCL    | I2C_PAD ),
		.gpio_mode = IOMUX_PADS(PAD_KEY_COL3__GPIO4_IO12  | I2C_PAD ),
		.gp        = IMX_GPIO_NR(4, 12)
	},
	.sda = {
		.i2c_mode  = IOMUX_PADS(PAD_KEY_ROW3__I2C2_SDA    | I2C_PAD ),
		.gpio_mode = IOMUX_PADS(PAD_KEY_ROW3__GPIO4_IO13  | I2C_PAD ),
		.gp        = IMX_GPIO_NR(4, 13)
	}
};

static struct i2c_pads_info board_i2c_pad_info2 = {
	.scl = {
		.i2c_mode  = IOMUX_PADS(PAD_GPIO_3__I2C3_SCL      | I2C_PAD ),
		.gpio_mode = IOMUX_PADS(PAD_GPIO_3__GPIO1_IO03    | I2C_PAD ),
		.gp        = IMX_GPIO_NR(1, 3)
	},
	.sda = {
		.i2c_mode  = IOMUX_PADS(PAD_GPIO_6__I2C3_SDA      | I2C_PAD ),
		.gpio_mode = IOMUX_PADS(PAD_GPIO_6__GPIO1_IO06    | I2C_PAD ),
		.gp        = IMX_GPIO_NR(1, 6)
	}
};

static struct i2c_pads_info board_i2c_pad_info3 = {
	.scl = {
		.i2c_mode  = IOMUX_PADS(PAD_GPIO_7__I2C4_SCL      | I2C_PAD ),
		.gpio_mode = IOMUX_PADS(PAD_GPIO_7__GPIO1_IO07    | I2C_PAD ),
		.gp        = IMX_GPIO_NR(1, 7)
	},
	.sda = {
		.i2c_mode  = IOMUX_PADS(PAD_GPIO_8__I2C4_SDA      | I2C_PAD ),
		.gpio_mode = IOMUX_PADS(PAD_GPIO_8__GPIO1_IO08    | I2C_PAD ),
		.gp        = IMX_GPIO_NR(1, 8)
	}
};

void board_setup_i2cs(void)
{
	setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x7f, &board_i2c_pad_info0);
	setup_i2c(1, CONFIG_SYS_I2C_SPEED, 0x7f, &board_i2c_pad_info1);
	setup_i2c(2, CONFIG_SYS_I2C_SPEED, 0x7f, &board_i2c_pad_info2);
	setup_i2c(3, CONFIG_SYS_I2C_SPEED, 0x7f, &board_i2c_pad_info3);
}
