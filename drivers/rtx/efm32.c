/*
 * efm32.c - driver for EFM32 MCU
 *
 * Copyright (c) 2014 Retronix Technology Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <i2c.h>

#ifdef CONFIG_RTX_SET_TIMEOUT
void vEFM32_SetTimeout( )
{
	unsigned char buf[4] = { 0 } ;
	unsigned int bus = i2c_get_bus_num() ;
		
	if ( i2c_set_bus_num( 1 ) ) 
	{
		printf("unable to set i2c bus\n");
	}
	else
	{
		if ( !i2c_probe( 0xc ) )
		{
			buf[0] = 0x00 ;
			if ( i2c_read( 0xc , 0xEB0000 , 3 , buf , 1 ) ) 
			//if ( i2c_read( 0xc , 0xEB01FF , 3 , buf , 1 ) ) 
			{
				printf("%s:i2c_write:error\n", __func__);
			}
			
		}
		else
		{
			printf("unable to probe efm32\n");
		}
	}
	/* reset the i2c bus */
	i2c_set_bus_num(bus);
}
#endif /*CONFIG_RTX_SET_TIMEOUT*/
