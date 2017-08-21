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
#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <usb.h>

#include "board_iomux.h"

#ifdef CONFIG_USB_EHCI_MX6

#define USB_OTHERREGS_OFFSET	0x800
#define UCTRL_PWR_POL			(1 << 9)

void board_setup_usb(void)
{
	/*
	 * set daisy chain for otg_pin_id on 6q.
	 * for 6dl, this bit is reserved
	 */
	imx_iomux_set_gpr_register(1, 13, 1, 0);
}

int board_ehci_hcd_init(int port)
{
	u32 *usbnc_usb_ctrl;

	if (port > 1)
		return -EINVAL;

	usbnc_usb_ctrl = (u32 *)(USB_BASE_ADDR + USB_OTHERREGS_OFFSET +	 port * 4);

	setbits_le32(usbnc_usb_ctrl, UCTRL_PWR_POL);

	return 0;
}

int board_ehci_power(int port, int on)
{
	switch (port) {
		case 0:
			#ifdef BOARD_PAD_USB_OTG_PWR_EN
				if (on)
					gpio_direction_output(BOARD_PAD_USB_OTG_PWR_EN, 1);
				else
					gpio_direction_output(BOARD_PAD_USB_OTG_PWR_EN, 0);
			#endif
			break;
		case 1:
			#ifdef BOARD_PAD_USB_H1_PWR_EN
				if (on)
					gpio_direction_output(BOARD_PAD_USB_H1_PWR_EN, 1);
				else
					gpio_direction_output(BOARD_PAD_USB_H1_PWR_EN, 0);
			#endif
			break;
		default:
			printf("MXC USB port %d not yet supported\n", port);
			return -EINVAL;
	}

	return 0;
}

#endif
