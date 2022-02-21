/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <asm/arch/imx8mm_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/arch/clock.h>
#include <spl.h>
#include <asm/mach-imx/dma.h>
#include <power/pmic.h>
#include <power/bd71837.h>
#include "../../freescale/common/tcpc.h"
#include <usb.h>
#include <sec_mipi_dsim.h>
#include <imx_mipi_dsi_bridge.h>
#include <mipi_dsi_panel.h>
#include <asm/mach-imx/video.h>
#if defined(CONFIG_BOOT_SYSTEM)
#include <rtx/bootsel.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)
#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_ODE | PAD_CTL_PUE | PAD_CTL_PE)

static iomux_v3_cfg_t const uart_pads[] = {
	IMX8MM_PAD_SAI3_TXFS_UART2_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MM_PAD_SAI3_TXC_UART2_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MM_PAD_SAI3_RXC_UART2_RTS_B | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MM_PAD_SAI3_RXD_UART2_CTS_B | MUX_PAD_CTRL(UART_PAD_CTRL),

/*	IMX8MM_PAD_UART4_RXD_UART4_RX | MUX_PAD_CTRL(UART_PAD_CTRL),	*/
/*	IMX8MM_PAD_UART4_TXD_UART4_TX | MUX_PAD_CTRL(UART_PAD_CTRL),	*/
};

static iomux_v3_cfg_t const pca6416_pads[] = {
	IMX8MM_PAD_UART4_RXD_GPIO5_IO28  | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_UART4_TXD_GPIO5_IO29  | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static iomux_v3_cfg_t const usb_reset_pads[] = {
	IMX8MM_PAD_SD1_DATA2_GPIO2_IO4  | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static iomux_v3_cfg_t const usb_pads[] = {
	IMX8MM_PAD_GPIO1_IO12_GPIO1_IO12 | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_SD1_RESET_B_GPIO2_IO10 | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_SD1_STROBE_GPIO2_IO11 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MM_PAD_GPIO1_IO02_WDOG1_WDOG_B  | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

#ifdef CONFIG_FSL_FSPI
#define QSPI_PAD_CTRL	(PAD_CTL_DSE2 | PAD_CTL_HYS)
static iomux_v3_cfg_t const qspi_pads[] = {
	IMX8MM_PAD_NAND_ALE_QSPI_A_SCLK | MUX_PAD_CTRL(QSPI_PAD_CTRL | PAD_CTL_PE | PAD_CTL_PUE),
	IMX8MM_PAD_NAND_CE0_B_QSPI_A_SS0_B | MUX_PAD_CTRL(QSPI_PAD_CTRL),

	IMX8MM_PAD_NAND_DATA00_QSPI_A_DATA0 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA01_QSPI_A_DATA1 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA02_QSPI_A_DATA2 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA03_QSPI_A_DATA3 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
};

int board_qspi_init(void)
{
	imx_iomux_v3_setup_multiple_pads(qspi_pads, ARRAY_SIZE(qspi_pads));

	set_clk_qspi();

	return 0;
}
#endif

#ifdef CONFIG_MXC_SPI
#define SPI_PAD_CTRL	(PAD_CTL_DSE2 | PAD_CTL_HYS)

static iomux_v3_cfg_t const ecspi3_pads[] = {
	IMX8MM_PAD_UART1_RXD_ECSPI3_SCLK | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MM_PAD_UART1_TXD_ECSPI3_MOSI | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MM_PAD_UART2_RXD_ECSPI3_MISO | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MM_PAD_UART2_TXD_GPIO5_IO25 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_spi(void)
{
	imx_iomux_v3_setup_multiple_pads(ecspi3_pads, ARRAY_SIZE(ecspi3_pads));
	gpio_request(IMX_GPIO_NR(5, 25), "ECSPI3 CS");
}

int board_spi_cs_gpio(unsigned bus, unsigned cs)
{
		return IMX_GPIO_NR(5, 25);
}
#endif

#ifdef CONFIG_NAND_MXS
#define NAND_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_HYS)
#define NAND_PAD_READY0_CTRL (PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_PUE)
static iomux_v3_cfg_t const gpmi_pads[] = {
	IMX8MM_PAD_NAND_ALE_RAWNAND_ALE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_CE0_B_RAWNAND_CE0_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_CE1_B_RAWNAND_CE1_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_CLE_RAWNAND_CLE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA00_RAWNAND_DATA00 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA01_RAWNAND_DATA01 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA02_RAWNAND_DATA02 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA03_RAWNAND_DATA03 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA04_RAWNAND_DATA04 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA05_RAWNAND_DATA05	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA06_RAWNAND_DATA06	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA07_RAWNAND_DATA07	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_RE_B_RAWNAND_RE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_READY_B_RAWNAND_READY_B | MUX_PAD_CTRL(NAND_PAD_READY0_CTRL),
	IMX8MM_PAD_NAND_WE_B_RAWNAND_WE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_WP_B_RAWNAND_WP_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
};

static void setup_gpmi_nand(void)
{
	imx_iomux_v3_setup_multiple_pads(gpmi_pads, ARRAY_SIZE(gpmi_pads));
}
#endif

#define DIP_S1_PAD IMX_GPIO_NR(2, 6)
#define DIP_S2_PAD IMX_GPIO_NR(2, 7)
#define DIP_S3_PAD IMX_GPIO_NR(2, 8)
#define DIP_S4_PAD IMX_GPIO_NR(2, 9)
static iomux_v3_cfg_t const dip_pads[] = {
	IMX8MM_PAD_SD1_DATA4_GPIO2_IO6  | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_SD1_DATA5_GPIO2_IO7  | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_SD1_DATA6_GPIO2_IO8  | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_SD1_DATA7_GPIO2_IO9  | MUX_PAD_CTRL(NO_PAD_CTRL),
};
static void setup_iomux_dip(void)
{
	char dipsw[4] ;
	imx_iomux_v3_setup_multiple_pads(dip_pads,
					 ARRAY_SIZE(dip_pads));
	gpio_request(DIP_S1_PAD, "dip_s1");
	gpio_request(DIP_S2_PAD, "dip_s2");
	gpio_request(DIP_S3_PAD, "dip_s3");
	gpio_request(DIP_S4_PAD, "dip_s4");
	gpio_direction_input(DIP_S1_PAD);
	gpio_direction_input(DIP_S2_PAD);
	gpio_direction_input(DIP_S3_PAD);
	gpio_direction_input(DIP_S4_PAD);
	sprintf( dipsw , "%d,%d,%d,%d", gpio_get_value(DIP_S1_PAD), gpio_get_value(DIP_S2_PAD), gpio_get_value(DIP_S3_PAD), gpio_get_value(DIP_S4_PAD));
	env_set("dipsw",dipsw);
	printf("read dipsw %s\n",dipsw);
}

#define DI_1_PAD IMX_GPIO_NR(1, 6)
#define DI_2_PAD IMX_GPIO_NR(1, 7)
#define DI_3_PAD IMX_GPIO_NR(1, 8)
#define DI_4_PAD IMX_GPIO_NR(1, 9)
static iomux_v3_cfg_t const di_pads[] = {
	IMX8MM_PAD_GPIO1_IO06_GPIO1_IO6  | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_GPIO1_IO07_GPIO1_IO7  | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_GPIO1_IO08_GPIO1_IO8  | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MM_PAD_GPIO1_IO09_GPIO1_IO9  | MUX_PAD_CTRL(NO_PAD_CTRL),
};
static void setup_iomux_di(void)
{
	char di[4] ;
	imx_iomux_v3_setup_multiple_pads(di_pads,
					 ARRAY_SIZE(di_pads));
	gpio_request(DI_1_PAD, "di_1");
	gpio_request(DI_2_PAD, "di_2");
	gpio_request(DI_3_PAD, "di_3");
	gpio_request(DI_4_PAD, "di_4");
	gpio_direction_input(DI_1_PAD);
	gpio_direction_input(DI_2_PAD);
	gpio_direction_input(DI_3_PAD);
	gpio_direction_input(DI_4_PAD);
	sprintf( di , "%d,%d,%d,%d", gpio_get_value(DI_1_PAD), gpio_get_value(DI_2_PAD), gpio_get_value(DI_3_PAD), gpio_get_value(DI_4_PAD));
	env_set("di",di);
	printf("read di %s\n",di);
}

int board_early_init_f(void)
{
	struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));

	set_wdog_reset(wdog);

	imx_iomux_v3_setup_multiple_pads(uart_pads, ARRAY_SIZE(uart_pads));

#ifdef CONFIG_NAND_MXS
	setup_gpmi_nand(); /* SPL will call the board_early_init_f */
#endif

	return 0;
}

#ifdef CONFIG_BOARD_POSTCLK_INIT
int board_postclk_init(void)
{
	/* TODO */
	return 0;
}
#endif

int dram_init(void)
{
	/* rom_pointer[1] contains the size of TEE occupies */
	if (rom_pointer[1])
		gd->ram_size = PHYS_SDRAM_SIZE - rom_pointer[1];
	else
		gd->ram_size = PHYS_SDRAM_SIZE;

	return 0;
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
	return 0;
}
#endif

#ifdef CONFIG_FEC_MXC
#define FEC_RST_PAD IMX_GPIO_NR(5, 1)
static iomux_v3_cfg_t const fec1_rst_pads[] = {
	IMX8MM_PAD_SAI3_TXD_GPIO5_IO1 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_iomux_fec(void)
{
	imx_iomux_v3_setup_multiple_pads(fec1_rst_pads,
					 ARRAY_SIZE(fec1_rst_pads));

	gpio_request(FEC_RST_PAD, "fec1_rst");
	gpio_direction_output(FEC_RST_PAD, 0);
	udelay(1000);
	gpio_direction_output(FEC_RST_PAD, 1);
}

static int setup_fec(void)
{
	struct iomuxc_gpr_base_regs *const iomuxc_gpr_regs
		= (struct iomuxc_gpr_base_regs *) IOMUXC_GPR_BASE_ADDR;

	setup_iomux_fec();

	/* Use 125M anatop REF_CLK1 for ENET1, not from external */
	clrsetbits_le32(&iomuxc_gpr_regs->gpr[1],
			IOMUXC_GPR_GPR1_GPR_ENET1_TX_CLK_SEL_SHIFT, 0);
	return set_clk_enet(ENET_125MHZ);
}

int board_phy_config(struct phy_device *phydev)
{
	/* enable rgmii rxc skew and phy mode select to RGMII copper */
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x1f);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x8);

	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x00);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x82ee);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x05);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x100);

	if (phydev->drv->config)
		phydev->drv->config(phydev);
	return 0;
}
#endif

#ifdef CONFIG_USB_TCPC
struct tcpc_port port1;
struct tcpc_port port2;

static int setup_pd_switch(uint8_t i2c_bus, uint8_t addr)
{
	struct udevice *bus;
	struct udevice *i2c_dev = NULL;
	int ret;
	uint8_t valb;

	ret = uclass_get_device_by_seq(UCLASS_I2C, i2c_bus, &bus);
	if (ret) {
		printf("%s: Can't find bus\n", __func__);
		return -EINVAL;
	}

	ret = dm_i2c_probe(bus, addr, 0, &i2c_dev);
	if (ret) {
		printf("%s: Can't find device id=0x%x\n",
			__func__, addr);
		return -ENODEV;
	}

	ret = dm_i2c_read(i2c_dev, 0xB, &valb, 1);
	if (ret) {
		printf("%s dm_i2c_read failed, err %d\n", __func__, ret);
		return -EIO;
	}
	valb |= 0x4; /* Set DB_EXIT to exit dead battery mode */
	ret = dm_i2c_write(i2c_dev, 0xB, (const uint8_t *)&valb, 1);
	if (ret) {
		printf("%s dm_i2c_write failed, err %d\n", __func__, ret);
		return -EIO;
	}

	/* Set OVP threshold to 23V */
	valb = 0x6;
	ret = dm_i2c_write(i2c_dev, 0x8, (const uint8_t *)&valb, 1);
	if (ret) {
		printf("%s dm_i2c_write failed, err %d\n", __func__, ret);
		return -EIO;
	}

	return 0;
}

int pd_switch_snk_enable(struct tcpc_port *port)
{
	if (port == &port1) {
		debug("Setup pd switch on port 1\n");
		return setup_pd_switch(1, 0x72);
	} else if (port == &port2) {
		debug("Setup pd switch on port 2\n");
		return setup_pd_switch(1, 0x73);
	} else
		return -EINVAL;
}

struct tcpc_port_config port1_config = {
	.i2c_bus = 1, /*i2c2*/
	.addr = 0x50,
	.port_type = TYPEC_PORT_UFP,
	.max_snk_mv = 5000,
	.max_snk_ma = 3000,
	.max_snk_mw = 40000,
	.op_snk_mv = 9000,
	.switch_setup_func = &pd_switch_snk_enable,
};

struct tcpc_port_config port2_config = {
	.i2c_bus = 1, /*i2c2*/
	.addr = 0x52,
	.port_type = TYPEC_PORT_UFP,
	.max_snk_mv = 5000,
	.max_snk_ma = 3000,
	.max_snk_mw = 40000,
	.op_snk_mv = 9000,
	.switch_setup_func = &pd_switch_snk_enable,
};

static int setup_typec(void)
{
	int ret;

	debug("tcpc_init port 2\n");
	ret = tcpc_init(&port2, port2_config, NULL);
	if (ret) {
		printf("%s: tcpc port2 init failed, err=%d\n",
		       __func__, ret);
	} else if (tcpc_pd_sink_check_charging(&port2)) {
		/* Disable PD for USB1, since USB2 has priority */
		port1_config.disable_pd = true;
		printf("Power supply on USB2\n");
	}

	debug("tcpc_init port 1\n");
	ret = tcpc_init(&port1, port1_config, NULL);
	if (ret) {
		printf("%s: tcpc port1 init failed, err=%d\n",
		       __func__, ret);
	} else {
		if (!port1_config.disable_pd)
			printf("Power supply on USB1\n");
		return ret;
	}

	return ret;
}

int board_usb_init(int index, enum usb_init_type init)
{
	int ret = 0;
	struct tcpc_port *port_ptr;

	debug("board_usb_init %d, type %d\n", index, init);

	if (index == 0)
		port_ptr = &port1;
	else
		port_ptr = &port2;

	imx8m_usb_power(index, true);

	if (init == USB_INIT_HOST)
		tcpc_setup_dfp_mode(port_ptr);
	else
		tcpc_setup_ufp_mode(port_ptr);

	return ret;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	int ret = 0;

	debug("board_usb_cleanup %d, type %d\n", index, init);

	if (init == USB_INIT_HOST) {
		if (index == 0)
			ret = tcpc_disable_src_vbus(&port1);
		else
			ret = tcpc_disable_src_vbus(&port2);
	}

	imx8m_usb_power(index, false);
	return ret;
}

int board_ehci_usb_phy_mode(struct udevice *dev)
{
	int ret = 0;
	enum typec_cc_polarity pol;
	enum typec_cc_state state;
	struct tcpc_port *port_ptr;

	if (dev->seq == 0)
		port_ptr = &port1;
	else
		port_ptr = &port2;

	tcpc_setup_ufp_mode(port_ptr);

	ret = tcpc_get_cc_status(port_ptr, &pol, &state);
	if (!ret) {
		if (state == TYPEC_STATE_SRC_RD_RA || state == TYPEC_STATE_SRC_RD)
			return USB_INIT_HOST;
	}

	return USB_INIT_DEVICE;
}

#endif

#ifdef CONFIG_USB_EHCI_HCD
int board_usb_init(int index, enum usb_init_type init)
{
	int ret = 0;

	debug("board_usb_init %d, type %d\n", index, init);
	imx_iomux_v3_setup_multiple_pads(usb_pads, ARRAY_SIZE(usb_pads));
	gpio_request(IMX_GPIO_NR(1, 12), "USB1_OTG_PWR");
	gpio_direction_output(IMX_GPIO_NR(1, 12), 1);
	gpio_request(IMX_GPIO_NR(2, 10), "USBP7_ONOFF");
	gpio_direction_output(IMX_GPIO_NR(2, 10), 1);
	gpio_request(IMX_GPIO_NR(2, 11), "USBP6_ONOFF");
	gpio_direction_output(IMX_GPIO_NR(2, 11), 1);

	imx8m_usb_power(index, true);

	return ret;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	int ret = 0;

	debug("board_usb_cleanup %d, type %d\n", index, init);
	gpio_request(IMX_GPIO_NR(1, 12), "USB1_OTG_PWR");
	gpio_direction_output(IMX_GPIO_NR(1, 12), 0);
	gpio_request(IMX_GPIO_NR(2, 10), "USBP7_ONOFF");
	gpio_direction_output(IMX_GPIO_NR(2, 10), 0);
	gpio_request(IMX_GPIO_NR(2, 11), "USBP6_ONOFF");
	gpio_direction_output(IMX_GPIO_NR(2, 11), 0);

	imx8m_usb_power(index, false);

	return ret;
}
#endif


#define PCA6416_ADDR 0x20
static int pca6416_reg_write(struct udevice *dev, uint addr, uint mask, uint data)
{
	uint8_t valb;
	int err;

	if (mask != 0xff) {
		err = dm_i2c_read(dev, addr, &valb, 1);
		if (err){
			printf("%s:dm_i2c_read:error\n", __func__);
			return err;
		}

		valb &= ~mask;
		valb |= data;
	} else {
		valb = data;
	}

	err = dm_i2c_write(dev, addr, (uint8_t *)&valb, 1);
	if (err){
		printf("%s: dm_i2c_write error 0x%x=0x%x \n", __func__, addr, valb);
		return err;
	}

	return err;
}

static void led_init(void)
{
	struct udevice *main_dev;
	int i2c_bus = 1;
	int ret;

	imx_iomux_v3_setup_multiple_pads(usb_reset_pads, ARRAY_SIZE(usb_reset_pads));
	gpio_request(IMX_GPIO_NR(2, 4), "USB_RESET");
	gpio_direction_output(IMX_GPIO_NR(2, 4), 1);

	imx_iomux_v3_setup_multiple_pads(pca6416_pads,
					 ARRAY_SIZE(pca6416_pads));

	gpio_request(IMX_GPIO_NR(5, 29), "RESET");
	gpio_direction_output(IMX_GPIO_NR(5, 29), 1);

	ret = i2c_get_chip_for_busnum(i2c_bus, PCA6416_ADDR, 1, &main_dev);
	if (ret) {
		printf("Cannot find pca6416: %d\n", ret);
	}
	else
	{
		/* PCA6416_init */
		/* Set polarity to non-inverted */
		pca6416_reg_write(main_dev, 0x04, 0xff, 0x00);
		pca6416_reg_write(main_dev, 0x05, 0xff, 0x00);

		/* Set Port0 the are high */
		pca6416_reg_write(main_dev, 0x02, 0xff, 0xFF);	//led off
		/* Set Port1 the are low */
		pca6416_reg_write(main_dev, 0x03, 0xff, 0x00);

		/* Configure P07-P00 as outputs */
		pca6416_reg_write(main_dev, 0x06, 0xff, 0x00);
		/* Configure P17-P10 as outputs */
		pca6416_reg_write(main_dev, 0x07, 0xff, 0x00);

		printf("pca6416 init\n");
		/* If Port is input status,read Port value */
		//pca6416_i2c_reg_read(main_dev, 0x00, &val);
		//printf("PCA6416 Port0 Read : 0x%x \n", val);
		//pca6416_i2c_reg_read(main_dev, 0x01, &val);
		//printf("PCA6416 Port1 Read : 0x%x \n", val);
	}
}

static void late_led_init(void)
{
	struct udevice *main_dev;
	int i2c_bus = 1;
	int ret;

	u64 u64LedValue = 0x00 ;

	ret = i2c_get_chip_for_busnum(i2c_bus, PCA6416_ADDR, 1, &main_dev);
	if (ret) {
		printf("Cannot find pca6416: %d\n", ret);
	}
	else
	{
		printf("LED init value");
		/* Set Port0 P6/P4/P2/P0 the output are high */
		u64LedValue = env_get_hex("initled1", 0x55);	//default all red
		printf(" %#llx", u64LedValue);
		if ( u64LedValue <= 0xff )
		{
			pca6416_reg_write(main_dev, 0x02, 0xff, u64LedValue);
			mdelay(300);
		}

		u64LedValue = env_get_hex("initled2", 0xAA);	//default all green
		printf(" %#llx", u64LedValue);
		if ( u64LedValue <= 0xff )
		{
			pca6416_reg_write(main_dev, 0x02, 0xff, u64LedValue);
			mdelay(300);
		}

		u64LedValue = env_get_hex("initled3", 0xFF);	//default off
		printf(" %#llx", u64LedValue);
		if ( u64LedValue <= 0xff )
		{
			pca6416_reg_write(main_dev, 0x02, 0xff, u64LedValue);
			mdelay(300);
		}

		u64LedValue = env_get_hex("initled4", 0x00);	//default all green+red
		printf(" %#llx", u64LedValue);
		if ( u64LedValue <= 0xff )
		{
			pca6416_reg_write(main_dev, 0x02, 0xff, u64LedValue);
		}
		printf("\n");
	}
}

int board_init(void)
{
#ifdef CONFIG_USB_TCPC
	setup_typec();
#endif

	led_init();

#ifdef CONFIG_MXC_SPI
	setup_spi();
#endif

#ifdef CONFIG_FEC_MXC
	setup_fec();
#endif

#ifdef CONFIG_FSL_FSPI
	board_qspi_init();
#endif

	return 0;
}

int board_mmc_get_env_dev(int devno)
{
	return devno - 1;
}

int mmc_map_to_kernel_blk(int devno)
{
	return devno + 1;
}

#ifdef CONFIG_VIDEO_MXS

#define LT9611_MAIN 0x3b

static const struct sec_mipi_dsim_plat_data imx8mm_mipi_dsim_plat_data = {
	.version	= 0x1060200,
	.max_data_lanes = 4,
	.max_data_rate  = 1500000000ULL,
	.reg_base = MIPI_DSI_BASE_ADDR,
	.gpr_base = CSI_BASE_ADDR + 0x8000,
};

static int lt9611_i2c_reg_write(struct udevice *dev, uint addr, uint mask, uint data)
{
	uint8_t valb;
	int err;

	if (mask != 0xff) {
		err = dm_i2c_read(dev, addr, &valb, 1);
		if (err)
			return err;

		valb &= ~mask;
		valb |= data;
	} else {
		valb = data;
	}

	err = dm_i2c_write(dev, addr, (uint8_t *)&valb, 1);
	return err;
}

static int lt9611_i2c_reg_read(struct udevice *dev, uint8_t addr, uint8_t *data)
{
	uint8_t valb;
	int err;

	err = dm_i2c_read(dev, addr, &valb, 1);
	if (err)
		return err;

	*data = (int)valb;
	return 0;
}

static void lt9611_init(void)
{
	struct udevice *bus, *main_dev;
	int i2c_bus = 2;
	int ret;
	uint8_t val1,val2,val3;

	ret = uclass_get_device_by_seq(UCLASS_I2C, i2c_bus, &bus);
	if (ret) {
		printf("%s: No bus %d\n", __func__, i2c_bus);
		return;
	}

	ret = dm_i2c_probe(bus, LT9611_MAIN, 0, &main_dev);
	if (ret) {
		printf("%s: Can't find device id=0x%x, on bus %d\n",
			__func__, LT9611_MAIN, i2c_bus);
		return;
	}

	printf("%s: init register\n", __func__);
	/* LT9611_Chip_ID */
	lt9611_i2c_reg_write(main_dev, 0xFF, 0xff, 0x80);
	lt9611_i2c_reg_write(main_dev, 0xee, 0xff, 0x01);
	lt9611_i2c_reg_read(main_dev, 0x00, &val1);
	lt9611_i2c_reg_read(main_dev, 0x00, &val2);
	lt9611_i2c_reg_read(main_dev, 0x00, &val3);
	printf("LT9611 ring Chip ID: 0x%x 0x%x 0x%x \n", val1, val2, val3);
	lt9611_i2c_reg_write(main_dev, 0xFF, 0xff, 0x81);
	lt9611_i2c_reg_write(main_dev, 0x01, 0xff, 0x18);
	lt9611_i2c_reg_write(main_dev, 0xFF, 0xff, 0x80);

	/* LT9611_System_Init */
	lt9611_i2c_reg_write(main_dev, 0xFF, 0xff, 0x82);
	lt9611_i2c_reg_write(main_dev, 0x51, 0xff, 0x11);
	lt9611_i2c_reg_write(main_dev, 0xFF, 0xff, 0x82);
	lt9611_i2c_reg_write(main_dev, 0x1b, 0xff, 0x69);
	lt9611_i2c_reg_write(main_dev, 0x1c, 0xff, 0x78);
	lt9611_i2c_reg_write(main_dev, 0xcb, 0xff, 0x69);
	lt9611_i2c_reg_write(main_dev, 0xcc, 0xff, 0x78);

	lt9611_i2c_reg_write(main_dev, 0xFF, 0xff, 0x80);
	lt9611_i2c_reg_write(main_dev, 0x04, 0xff, 0xf0);
	lt9611_i2c_reg_write(main_dev, 0x06, 0xff, 0xf0);
	lt9611_i2c_reg_write(main_dev, 0x0a, 0xff, 0x80);
	lt9611_i2c_reg_write(main_dev, 0x0b, 0xff, 0x46);
	lt9611_i2c_reg_write(main_dev, 0x0d, 0xff, 0xef);
	lt9611_i2c_reg_write(main_dev, 0x11, 0xff, 0xfa);

}

#define DISPLAY_MIX_SFT_RSTN_CSR		0x00
#define DISPLAY_MIX_CLK_EN_CSR		0x04

   /* 'DISP_MIX_SFT_RSTN_CSR' bit fields */
#define BUS_RSTN_BLK_SYNC_SFT_EN	BIT(6)

   /* 'DISP_MIX_CLK_EN_CSR' bit fields */
#define LCDIF_PIXEL_CLK_SFT_EN		BIT(7)
#define LCDIF_APB_CLK_SFT_EN		BIT(6)

void disp_mix_bus_rstn_reset(ulong gpr_base, bool reset)
{
	if (!reset)
		/* release reset */
		setbits_le32(gpr_base + DISPLAY_MIX_SFT_RSTN_CSR, BUS_RSTN_BLK_SYNC_SFT_EN);
	else
		/* hold reset */
		clrbits_le32(gpr_base + DISPLAY_MIX_SFT_RSTN_CSR, BUS_RSTN_BLK_SYNC_SFT_EN);
}

void disp_mix_lcdif_clks_enable(ulong gpr_base, bool enable)
{
	if (enable)
		/* enable lcdif clks */
		setbits_le32(gpr_base + DISPLAY_MIX_CLK_EN_CSR, LCDIF_PIXEL_CLK_SFT_EN | LCDIF_APB_CLK_SFT_EN);
	else
		/* disable lcdif clks */
		clrbits_le32(gpr_base + DISPLAY_MIX_CLK_EN_CSR, LCDIF_PIXEL_CLK_SFT_EN | LCDIF_APB_CLK_SFT_EN);
}

struct mipi_dsi_client_dev lt9611_dev = {
	.channel	= 0,
	.lanes = 4,
	.format  = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
			  MIPI_DSI_MODE_EOT_PACKET | MIPI_DSI_MODE_VIDEO_HSE,
	.name = "LT9611",
};

struct mipi_dsi_client_dev rm67191_dev = {
	.channel	= 0,
	.lanes = 4,
	.format  = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
			  MIPI_DSI_MODE_EOT_PACKET | MIPI_DSI_MODE_VIDEO_HSE,
};

#define FSL_SIP_GPC			0xC2000000
#define FSL_SIP_CONFIG_GPC_PM_DOMAIN	0x3
#define DISPMIX				9
#define MIPI				10

void do_enable_mipi2hdmi(struct display_info_t const *dev)
{
	gpio_request(IMX_GPIO_NR(4, 27), "M2HPWR_EN");
	gpio_direction_output(IMX_GPIO_NR(4, 27), 1);

	/* LT9611 initialization */
	lt9611_init();

	/* enable the dispmix & mipi phy power domain */
	call_imx_sip(FSL_SIP_GPC, FSL_SIP_CONFIG_GPC_PM_DOMAIN, DISPMIX, true, 0);
	call_imx_sip(FSL_SIP_GPC, FSL_SIP_CONFIG_GPC_PM_DOMAIN, MIPI, true, 0);

	/* Put lcdif out of reset */
	disp_mix_bus_rstn_reset(imx8mm_mipi_dsim_plat_data.gpr_base, false);
	disp_mix_lcdif_clks_enable(imx8mm_mipi_dsim_plat_data.gpr_base, true);

	/* Setup mipi dsim */
	sec_mipi_dsim_setup(&imx8mm_mipi_dsim_plat_data);
	imx_mipi_dsi_bridge_attach(&lt9611_dev); /* attach lt9611 device */
}

void do_enable_mipi_led(struct display_info_t const *dev)
{
	/* enable the dispmix & mipi phy power domain */
	call_imx_sip(FSL_SIP_GPC, FSL_SIP_CONFIG_GPC_PM_DOMAIN, DISPMIX, true, 0);
	call_imx_sip(FSL_SIP_GPC, FSL_SIP_CONFIG_GPC_PM_DOMAIN, MIPI, true, 0);

	/* Put lcdif out of reset */
	disp_mix_bus_rstn_reset(imx8mm_mipi_dsim_plat_data.gpr_base, false);
	disp_mix_lcdif_clks_enable(imx8mm_mipi_dsim_plat_data.gpr_base, true);

	/* Setup mipi dsim */
	sec_mipi_dsim_setup(&imx8mm_mipi_dsim_plat_data);

	rm67191_init();
	rm67191_dev.name = displays[1].mode.name;
	imx_mipi_dsi_bridge_attach(&rm67191_dev); /* attach rm67191 device */
}

void board_quiesce_devices(void)
{
//	gpio_request(IMX_GPIO_NR(4, 27), "M2HPWR_EN");
//	gpio_direction_output(IMX_GPIO_NR(4, 27), 1);
}

struct display_info_t const displays[] = {{
	.bus = LCDIF_BASE_ADDR,
	.addr = 0,
	.pixfmt = 24,
	.detect = NULL,
	.enable	= do_enable_mipi2hdmi,
	.mode	= {
		.name			= "MIPI2HDMI",
		.refresh		= 60,
		.xres			= 1920,
		.yres			= 1080,
		.pixclock		= 6734, /* 148500000 */
		.left_margin	= 148,
		.right_margin	= 88,
		.upper_margin	= 36,
		.lower_margin	= 4,
		.hsync_len		= 44,
		.vsync_len		= 5,
		.sync			= FB_SYNC_EXT,
		.vmode			= FB_VMODE_NONINTERLACED

} }, {
	.bus = LCDIF_BASE_ADDR,
	.addr = 0,
	.pixfmt = 24,
	.detect = NULL,
	.enable	= do_enable_mipi_led,
	.mode	= {
		.name			= "RM67191_OLED",
		.refresh		= 60,
		.xres			= 1080,
		.yres			= 1920,
		.pixclock		= 7575, /* 132000000 */
		.left_margin	= 34,
		.right_margin	= 20,
		.upper_margin	= 4,
		.lower_margin	= 10,
		.hsync_len		= 2,
		.vsync_len		= 2,
		.sync			= FB_SYNC_EXT,
		.vmode			= FB_VMODE_NONINTERLACED

} } };
size_t display_count = ARRAY_SIZE(displays);
#endif

int board_late_init(void)
{
	late_led_init();
	setup_iomux_dip();
	setup_iomux_di();

#if defined(CONFIG_BOOT_SYSTEM)
	bootsel_init() ;
#endif

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

	return 0;
}

#ifdef CONFIG_FSL_FASTBOOT
#ifdef CONFIG_ANDROID_RECOVERY
int is_recovery_key_pressing(void)
{
	return 0; /*TODO*/
}
#endif /*CONFIG_ANDROID_RECOVERY*/
#endif /*CONFIG_FSL_FASTBOOT*/
