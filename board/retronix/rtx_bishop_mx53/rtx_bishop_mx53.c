/*
 * (C) Copyright 2011 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/clock.h>
#include <asm/arch/iomux-mx53.h>
#include <asm/errno.h>
#include <netdev.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <asm/gpio.h>
#ifdef CONFIG_RTX_EFM32
#include <rtx/efm32.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE);

	return 0;
}
void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
}

static iomux_v3_cfg_t rtx_bishop_mx53_pads[] = {
	/* I2C2 */
	MX53_PAD_EIM_EB2__I2C2_SCL,
	MX53_PAD_EIM_D16__I2C2_SDA,

	/* UART1 */
	MX53_PAD_EIM_EB3__UART1_RI,
	MX53_PAD_EIM_D19__UART1_CTS,
	MX53_PAD_EIM_D20__UART1_RTS,
	MX53_PAD_EIM_D25__UART1_DSR,
	MX53_PAD_EIM_D24__UART1_DTR,
	MX53_PAD_EIM_D23__UART1_DCD,
	MX53_PAD_PATA_DIOW__UART1_TXD_MUX,
	MX53_PAD_PATA_DMACK__UART1_RXD_MUX,

	/* LID_OPN_CLS_SW*/
	MX53_PAD_EIM_CS0__GPIO2_23,
	/* MODEM_DISABLE_B */
	MX53_PAD_EIM_A24__GPIO5_4,
	/* MODEM_RESET_B */
	MX53_PAD_EIM_A25__GPIO5_2,
	/* MODEM_WKUP */
	MX53_PAD_EIM_D27__GPIO3_27,
	/* MODEM_RING0 */
	MX53_PAD_EIM_D28__GPIO3_28,
	/* WIFI_BT_PWR_EN */
	MX53_PAD_EIM_DA10__GPIO3_10,
	/* RECOVERY_MODE_SW */
	MX53_PAD_EIM_DA11__GPIO3_11,
	/* USB_OTG_OC */
	MX53_PAD_EIM_DA12__GPIO3_12,
	/* USB_HUB_RESET_B */
	MX53_PAD_EIM_DA14__GPIO3_14,
	/* GSM_CONTROL */
	MX53_PAD_GPIO_17__GPIO7_12,

	/* BT_EN*/ 
	MX53_PAD_NANDF_ALE__GPIO6_8,
	/* WIFI_EN */
	MX53_PAD_NANDF_CLE__GPIO6_7,	
	/* WFI_RST */  
	MX53_PAD_NANDF_WP_B__GPIO6_9,
	/* BT_WAKEUP */  
	MX53_PAD_NANDF_RB0__GPIO6_10,
	/* OSC_CKIH1_EN */
	MX53_PAD_NANDF_CS0__GPIO6_11,
	/* WIFI_WAKEUP */  
	MX53_PAD_NANDF_CS1__GPIO6_14,
	/* ACCL_INT1_IN */
	MX53_PAD_NANDF_CS2__GPIO6_15,
	/* ACCL_INT2_IN */
	MX53_PAD_NANDF_CS3__GPIO6_16,

	/* BT_PCM */
	MX53_PAD_CSI0_DAT4__AUDMUX_AUD3_TXC,
	MX53_PAD_CSI0_DAT5__AUDMUX_AUD3_TXD,
	MX53_PAD_CSI0_DAT6__AUDMUX_AUD3_TXFS,
	MX53_PAD_CSI0_DAT7__AUDMUX_AUD3_RXD,
	/* I2C1 */
	MX53_PAD_CSI0_DAT8__I2C1_SDA,
	MX53_PAD_CSI0_DAT9__I2C1_SCL,
	/*UART4*/ 
	MX53_PAD_CSI0_DAT12__UART4_TXD_MUX,
	MX53_PAD_CSI0_DAT13__UART4_RXD_MUX,
	/*UART5*/ 
	MX53_PAD_CSI0_DAT14__UART5_TXD_MUX,
	MX53_PAD_CSI0_DAT15__UART5_RXD_MUX,
	
	/* FEC */ 
	MX53_PAD_FEC_MDC__FEC_MDC,
	MX53_PAD_FEC_MDIO__FEC_MDIO,
	MX53_PAD_FEC_REF_CLK__FEC_TX_CLK,
	MX53_PAD_FEC_RX_ER__FEC_RX_ER,
	MX53_PAD_FEC_CRS_DV__FEC_RX_DV,
	MX53_PAD_FEC_RXD1__FEC_RDATA_1,
	MX53_PAD_FEC_RXD0__FEC_RDATA_0,
	MX53_PAD_FEC_TX_EN__FEC_TX_EN,
	MX53_PAD_FEC_TXD1__FEC_TDATA_1,
	MX53_PAD_FEC_TXD0__FEC_TDATA_0,

	/* LED */
	MX53_PAD_GPIO_14__GPIO4_4,
	MX53_PAD_GPIO_19__GPIO4_5,
	MX53_PAD_KEY_COL0__GPIO4_6,
	MX53_PAD_KEY_COL1__GPIO4_8,
	MX53_PAD_KEY_ROW1__GPIO4_9,
	MX53_PAD_KEY_COL2__GPIO4_10,
	MX53_PAD_KEY_ROW2__GPIO4_11,
	MX53_PAD_KEY_COL3__GPIO4_12,
	MX53_PAD_KEY_ROW3__GPIO4_13,
	MX53_PAD_KEY_COL4__GPIO4_14,

	/* DIP switch */
	MX53_PAD_SD1_CMD__GPIO1_18,
	MX53_PAD_SD1_CLK__GPIO1_20,
	MX53_PAD_SD1_DATA0__GPIO1_16,
	/* WiFi_LED */
        MX53_PAD_SD1_DATA1__GPIO1_17,
	MX53_PAD_SD1_DATA3__GPIO1_21,
	/* Buzzer PWM */
	MX53_PAD_SD1_DATA2__PWM2_PWMO,
	
	/* SD2 */  //WIFI_SDIO
	MX53_PAD_SD2_CMD__ESDHC2_CMD,
	MX53_PAD_SD2_CLK__ESDHC2_CLK,
	MX53_PAD_SD2_DATA0__ESDHC2_DAT0,
	MX53_PAD_SD2_DATA1__ESDHC2_DAT1,
	MX53_PAD_SD2_DATA2__ESDHC2_DAT2,
	MX53_PAD_SD2_DATA3__ESDHC2_DAT3,

	/* UART2 */
	MX53_PAD_PATA_DMARQ__UART2_TXD_MUX,
	MX53_PAD_PATA_BUFFER_EN__UART2_RXD_MUX,
	MX53_PAD_PATA_DIOR__UART2_RTS,
	MX53_PAD_PATA_INTRQ__UART2_CTS,
	/* UART3*/
	MX53_PAD_PATA_CS_1__UART3_RXD_MUX,
	MX53_PAD_PATA_CS_0__UART3_TXD_MUX,

	/* FEC_nRST */
	MX53_PAD_PATA_DA_0__GPIO7_6,
	/* CPEN */
	MX53_PAD_PATA_DA_1__GPIO7_7,
	/* USB_OTG_PWR_EN */
	MX53_PAD_PATA_DA_2__GPIO7_8,
	/* SD3 */ //EMMC
	MX53_PAD_PATA_DATA8__ESDHC3_DAT0,
	MX53_PAD_PATA_DATA9__ESDHC3_DAT1,
	MX53_PAD_PATA_DATA10__ESDHC3_DAT2,
	MX53_PAD_PATA_DATA11__ESDHC3_DAT3,
	MX53_PAD_PATA_DATA0__ESDHC3_DAT4,
	MX53_PAD_PATA_DATA1__ESDHC3_DAT5,
	MX53_PAD_PATA_DATA2__ESDHC3_DAT6,
	MX53_PAD_PATA_DATA3__ESDHC3_DAT7,
	MX53_PAD_PATA_IORDY__ESDHC3_CLK,
	MX53_PAD_PATA_RESET_B__ESDHC3_CMD,
	/* FEC_nINT */
	MX53_PAD_PATA_DATA4__GPIO2_4,
	/* RESET */
	MX53_PAD_PATA_DATA6__GPIO2_6,
	/* BZ */
	MX53_PAD_PATA_DATA7__GPIO2_7,
	/* ADM3311_EN */ 
	MX53_PAD_PATA_DATA12__GPIO2_12,
	/*  ADM3311_SD */ 
	MX53_PAD_PATA_DATA13__GPIO2_13,
	/* MODE SELECT 1 */  
	MX53_PAD_PATA_DATA14__GPIO2_14,
	/* MODE SELECT 2 */  
	MX53_PAD_PATA_DATA15__GPIO2_15,
	/* MCU_BOOT_EN */  
	MX53_PAD_GPIO_0__GPIO1_0,
	/* I2C3 */
	MX53_PAD_GPIO_3__I2C3_SCL,
	MX53_PAD_GPIO_6__I2C3_SDA,
	/* SYSTEM_DOWN */
	MX53_PAD_GPIO_5__GPIO1_5,
	/* SYS_ON_OFF_CTL */
	MX53_PAD_GPIO_7__GPIO1_7,
	/* PMIC_ON_OFF_REQ */
	MX53_PAD_GPIO_8__GPIO1_8,
	/*MCU_UPDATE_PWR_ON*/  
	MX53_PAD_GPIO_13__GPIO4_3,
	/* PMIC_INT */
	MX53_PAD_GPIO_16__GPIO7_11,
	/*MCU_RESETn*/  
	MX53_PAD_GPIO_18__GPIO7_13,
};

/* MX53 SMD GPIO PIN configurations */

#define MX53_BISHOP_MCU_BOOT_EN			(0*32 + 0)	/* GPIO1_0 */
#define MX53_BISHOP_SYSTEM_DOWN			(0*32 + 5)	/* GPIO1_5 */
#define MX53_BISHOP_SYS_ON_OFF_CTL			(0*32 + 7)	/* GPIO1_7 */
#define MX53_BISHOP_PMIC_ON_OFF_REQ		(0*32 + 8)	/* GPIO1_8 */
#define MX53_BISHOP_DIP_SWITCH_3			(0*32 + 16)	/* GPIO1_16 */
#define MX53_BISHOP_DIP_SWITCH_1			(0*32 + 18)	/* GPIO1_18 */
#define MX53_BISHOP_DIP_SWITCH_2			(0*32 + 20)	/* GPIO1_20 */
#define MX53_BISHOP_WIFI_LED_GREEN			(0*32 + 17)	/* GPIO1_17 */
#define MX53_BISHOP_BUZZER_PWM_OFF			(0*32 + 19)	/* GPIO1_19 */
#define MX53_BISHOP_WIFI_LED_RED			(0*32 + 21)	/* GPIO1_21 */

#define MX53_BISHOP_FEC_INT			(1*32 + 4)	/* GPIO2_4 */
#define MX53_BISHOP_RESET				(1*32 + 6)	/* GPIO2_6 */
#define MX53_BISHOP_BZ				(1*32 + 7)	/* GPIO2_7 */
#define MX53_BISHOP_ADM3311_EN			(1*32 + 12)	/* GPIO2_12 */
#define MX53_BISHOP_ADM3311_SD			(1*32 + 13)	/* GPIO2_13 */
#define MX53_BISHOP_MODE_SELECT_1			(1*32 + 14)	/* GPIO2_14 */
#define MX53_BISHOP_MODE_SELECT_2			(1*32 + 15)	/* GPIO2_15 */
#define MX53_BISHOP_LID_OPN_CLS_SW			(1*32 + 23)	/* GPIO2_23 */

#define MX53_BISHOP_WIFI_BT_PWR_EN			(2*32 + 10)	/*GPIO3_10 */
#define MX53_BISHOP_RECOVERY_MODE_SW		(2*32 + 11)	/*GPIO3_11 */
#define MX53_BISHOP_USB_OTG_OC				(2*32 + 12)	/*GPIO3_12 */
#define MX53_BISHOP_USB_HUB_RESET_B		(2*32 + 14)	/*GPIO3_14 */
#define MX53_BISHOP_MODEM_WKUP				(2*32 + 27)	/* GPIO3_27 */
#define MX53_BISHOP_MODEM_RING0			(2*32 + 28)	/* GPIO3_28 */

#define MX53_BISHOP_MCU_UPDATE_PWR_ON		(3*32 + 3)	/* GPIO4_3 */
#define MX53_BISHOP_PWR_LED_GREEN			(3*32 + 4)	/* GPIO4_4 */
#define MX53_BISHOP_FUNC_LED_GREEN			(3*32 + 5)	/* GPIO4_5 */
#define MX53_BISHOP_PWR_LED_RED			(3*32 + 6)	/* GPIO4_6 */
#define MX53_BISHOP_DATA_LED_RED			(3*32 + 8)	/* GPIO4_8 */
#define MX53_BISHOP_DATA_LED_GREEN			(3*32 + 9)	/* GPIO4_9 */
#define MX53_BISHOP_LINK_LED_RED			(3*32 + 10)	/* GPIO4_10 */
#define MX53_BISHOP_LINK_LED_GREEN			(3*32 + 11)	/* GPIO4_11 */
#define MX53_BISHOP_BT_LED_GREEN			(3*32 + 12)	/* GPIO4_12 */
#define MX53_BISHOP_BT_LED_RED				(3*32 + 13)	/* GPIO4_13 */
#define MX53_BISHOP_FUNC_LED_RED			(3*32 + 14)	/* GPIO4_14 */

#define MX53_BISHOP_MODEM_RESET_B			(4*32 + 2)	/* GPIO5_2 */
#define MX53_BISHOP_MODEM_DISABLE_B		(4*32 + 4)	/* GPIO5_4 */

#define MX53_BISHOP_WIFI_EN				(5*32 + 7)	/* GPIO6_7 */
#define MX53_BISHOP_BT_EN					(5*32 + 8)	/* GPIO6_8 */
#define MX53_BISHOP_WIFI_RST				(5*32 + 9)	/* GPIO6_9 */
#define MX53_BISHOP_BT_WAKEUP				(5*32 + 10)	/* GPIO6_10 */
#define MX53_BISHOP_OSC_CKIH1_EN			(5*32 + 11)	/* GPIO6_11 */
#define MX53_BISHOP_WIFI_WAKEUP			(5*32 + 14)	/* GPIO6_14 */
#define MX53_BISHOP_ACCL_INT1_IN			(5*32 + 15)	/* GPIO6_15 */
#define MX53_BISHOP_ACCL_INT2_IN			(5*32 + 16)	/* GPIO6_16 */

#define MX53_BISHOP_FEC_RST				(6*32 + 6)	/* GPIO7_6 */
#define MX53_BISHOP_CPEN					(6*32 + 7)	/* GPIO7_7 */
#define MX53_BISHOP_USB_OTG_PWR_EN			(6*32 + 8)	/* GPIO7_8 */
#define MX53_BISHOP_PMIC_INT				(6*32 + 11)	/* GPIO7_11 */
#define MX53_BISHOP_GSM_CONTROL			(6*32 + 12) /* GPIO7_12 */
#define MX53_BISHOP_MCU_RESETn				(6*32 + 13)	/* GPIO7_13 */

static void setup_iomux(void)
{
	imx_iomux_v3_setup_multiple_pads(rtx_bishop_mx53_pads, ARRAY_SIZE(rtx_bishop_mx53_pads));
    
    gpio_request(MX53_BISHOP_PWR_LED_RED, "pwr-led-red");
	gpio_direction_output(MX53_BISHOP_PWR_LED_RED, 1);
	gpio_request(MX53_BISHOP_PWR_LED_GREEN, "pwr-led-green");
	gpio_direction_output(MX53_BISHOP_PWR_LED_GREEN, 1);

	gpio_request(MX53_BISHOP_DATA_LED_RED, "data-led-red");
	gpio_direction_output(MX53_BISHOP_DATA_LED_RED, 0);
	gpio_request(MX53_BISHOP_DATA_LED_GREEN, "data-led-green");
	gpio_direction_output(MX53_BISHOP_DATA_LED_GREEN, 0);

	gpio_request(MX53_BISHOP_LINK_LED_RED, "link-led-red");
	gpio_direction_output(MX53_BISHOP_LINK_LED_RED, 0);
	gpio_request(MX53_BISHOP_LINK_LED_GREEN, "link-led-green");
	gpio_direction_output(MX53_BISHOP_LINK_LED_GREEN, 0);

	gpio_request(MX53_BISHOP_BT_LED_GREEN, "bt-led-green");
	gpio_direction_output(MX53_BISHOP_BT_LED_GREEN, 0);
	gpio_request(MX53_BISHOP_BT_LED_RED, "bt-led-red");
	gpio_direction_output(MX53_BISHOP_BT_LED_RED, 0);

	gpio_request(MX53_BISHOP_FUNC_LED_RED, "func-led-red");
	gpio_direction_output(MX53_BISHOP_FUNC_LED_RED, 0);
	gpio_request(MX53_BISHOP_FUNC_LED_GREEN, "func-led-green");
	gpio_direction_output(MX53_BISHOP_FUNC_LED_GREEN, 0);

	gpio_request(MX53_BISHOP_WIFI_LED_GREEN, "wifi-led-green");
	gpio_direction_output(MX53_BISHOP_WIFI_LED_GREEN, 0);
	gpio_request(MX53_BISHOP_WIFI_LED_RED, "wifi-led-red");
	gpio_direction_output(MX53_BISHOP_WIFI_LED_RED, 0);
}

#ifdef CONFIG_FSL_ESDHC
struct fsl_esdhc_cfg esdhc_cfg[1] = {
	{MMC_SDHC3_BASE_ADDR},
};

int board_mmc_getcd(struct mmc *mmc)
{
	return 1;
}

int board_mmc_init(bd_t *bis)
{
	u32 index;
	int ret;

	esdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC_CLK);

	for (index = 0; index < CONFIG_SYS_FSL_ESDHC_NUM; index++) {
		switch (index) {
		case 0:
			break;

		default:
			printf("Warning: you configured more ESDHC controller"
				"(%d) as supported by the board(1)\n",
				CONFIG_SYS_FSL_ESDHC_NUM);
			return -EINVAL;
		}
		ret = fsl_esdhc_initialize(bis, &esdhc_cfg[index]);
		if (ret)
			return ret;
	}

	return 0;
}
#endif

int board_early_init_f(void)
{
	setup_iomux();

	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;
#ifdef CONFIG_RTX_EFM32
#ifdef CONFIG_RTX_SET_TIMEOUT
	vEFM32_SetTimeout( ) ;
#endif
#endif

	return 0;
}

int checkboard(void)
{
	puts("Board: Retronix BI-SHOP MX53\n");

	return 0;
}

void vDebug_Led( void )
{
    while( 1 )
    {

	    gpio_direction_output(MX53_BISHOP_BT_LED_GREEN, 1);
        mdelay(100) ;
       	gpio_direction_output(MX53_BISHOP_BT_LED_GREEN, 0);
        mdelay(100) ;
    }
}

