
#ifndef __RTX_BOARD_IOMUX_H__
#define __RTX_BOARD_IOMUX_H__

#include <asm/gpio.h>

#define BOARD_PAD_I2C1_EN				IMX_GPIO_NR(2, 7)

#define BOARD_PAD_UART1_EN				IMX_GPIO_NR(4,15)
#define BOARD_PAD_UART1_PTR_RESET		IMX_GPIO_NR(7, 6)

#define BOARD_PAD_UART2_EN				IMX_GPIO_NR(1, 0)

#define BOARD_PAD_UART5_EN				IMX_GPIO_NR(3,31)

#define BOARD_PAD_USB_OTG_PWR_EN		IMX_GPIO_NR(3,22)
#define BOARD_PAD_USB_H1_PWR_EN			IMX_GPIO_NR(1,29)

#define BOARD_PAD_SD1_CD				IMX_GPIO_NR(1, 1)
#define BOARD_PAD_SD1_WP				IMX_GPIO_NR(1, 9)

#define BOARD_PAD_SD2_CD				IMX_GPIO_NR(1, 4)
#define BOARD_PAD_SD2_WP				IMX_GPIO_NR(1, 2)

#define BOARD_PAD_SD4_RESET				IMX_GPIO_NR(4, 5)

#define BOARD_PAD_PMIC_INT				IMX_GPIO_NR(7,13)

#define BOARD_PAD_KEY_PWR_BTN			IMX_GPIO_NR(3,29)
#define BOARD_PAD_KEY_RESET_BTN			IMX_GPIO_NR(2, 2)

extern void board_setup_pads(void) ;

#endif
