
#ifndef __RTX_BOARD_IOMUX_H__
#define __RTX_BOARD_IOMUX_H__

#include <asm/gpio.h>

#define BOARD_PAD_TOUCH_INT				IMX_GPIO_NR(6, 7)
#define BOARD_PAD_TOUCH_RESET			IMX_GPIO_NR(6, 8)
#define BOARD_PAD_TS_INT				IMX_GPIO_NR(3,26)

#define BOARD_PAD_FAN_THRM				IMX_GPIO_NR(3,16)
#define BOARD_PAD_FAN_THRMTRIP			IMX_GPIO_NR(3,17)
#define BOARD_PAD_FAN_TACHOIN			IMX_GPIO_NR(1,16)

#define BOARD_PAD_LVDS_BL_ENABLE		IMX_GPIO_NR(6,14)
#define BOARD_PAD_LVDS_PP_ENABLE		IMX_GPIO_NR(6,15)

#define BOARD_PAD_GPIO1					IMX_GPIO_NR(5, 2)
#define BOARD_PAD_GPIO2					IMX_GPIO_NR(3,18)
#define BOARD_PAD_GPIO3					IMX_GPIO_NR(3,19)
#define BOARD_PAD_GPIO4					IMX_GPIO_NR(4, 8)
#define BOARD_PAD_GPIO5					IMX_GPIO_NR(3,24)
#define BOARD_PAD_GPIO6					IMX_GPIO_NR(3,25)
#define BOARD_PAD_GPIO7					IMX_GPIO_NR(3,27)
#define BOARD_PAD_GPIO8					IMX_GPIO_NR(4, 9)

#define BOARD_PAD_HEADPHONE_DET			IMX_GPIO_NR(7, 8)
#define BOARD_PAD_MICROPHONE_DET		IMX_GPIO_NR(1, 9)
#define BOARD_PAD_AUD_AMP_STBY			IMX_GPIO_NR(4,10)

#define BOARD_PAD_3_AXIS_INT			IMX_GPIO_NR(1,18)

#define BOARD_PAD_RGMII_RESET			IMX_GPIO_NR(1,25)

#define BOARD_PAD_USB_OTG_PWR_EN		IMX_GPIO_NR(4, 6)

#define BOARD_PAD_USB_H1_PWR_EN			IMX_GPIO_NR(1,29)
#define BOARD_PAD_USB_HUB_RESET			IMX_GPIO_NR(3,23)

#define BOARD_PAD_SD2_CD				IMX_GPIO_NR(2, 2)
#define BOARD_PAD_SD2_WP				IMX_GPIO_NR(2, 3)

#define BOARD_PAD_SD3_CD				IMX_GPIO_NR(2, 0)
#define BOARD_PAD_SD3_WP				IMX_GPIO_NR(2, 1)

#define BOARD_PAD_PCIE_WAKE				IMX_GPIO_NR(5,20)
#define BOARD_PAD_PCIE_DISABLE			IMX_GPIO_NR(7,11)
#define BOARD_PAD_PCIE_RESET			IMX_GPIO_NR(7,12)

#define BOARD_PAD_MCU_RESET				IMX_GPIO_NR(6,10)
#define BOARD_PAD_MCU_PWR_KEEP			IMX_GPIO_NR(6, 9)
#define BOARD_PAD_MCU_BOOT_ENABLE		IMX_GPIO_NR(6,11)
#define BOARD_PAD_MCU_INT				IMX_GPIO_NR(3,31)

#define BOARD_PAD_PMIC_INT				IMX_GPIO_NR(7,13)

#define BOARD_PAD_SPI4_CS0				IMX_GPIO_NR(3,20)

#define BOARD_PAD_CAN1_STBY				IMX_GPIO_NR(4, 5)

#define BOARD_PAD_KEY_PWR_BTN			IMX_GPIO_NR(3,29)
#define BOARD_PAD_KEY_VOLUME_UP			IMX_GPIO_NR(1, 4)
#define BOARD_PAD_KEY_VOLUME_DOWN		IMX_GPIO_NR(1, 5)
#define BOARD_PAD_KEY_BACK				IMX_GPIO_NR(1, 2)
#define BOARD_PAD_KEY_HOME				IMX_GPIO_NR(6,16)

extern void board_setup_pads(void) ;

#endif
