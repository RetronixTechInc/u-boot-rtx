/*
 * efm32.c - driver for EFM32 MCU
 *
 * Copyright (c) 2014 Retronix Technology Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <i2c.h>

void vSet_efm32_watchdog( unsigned long sec )
{
#ifdef CONFIG_RTX_EFM32_I2C_BUS
	unsigned char ubSentBuf[8] = { 0 } ;
	unsigned char ubRecvBuf[8] = { 0 } ;
	unsigned int iBus = i2c_get_bus_num() ;
    int i ;
    
    ubSentBuf[0] = 5 ;
    ubSentBuf[1] = 0x8E ;
    ubSentBuf[2] = (unsigned char)( sec >> 8 & 0xFF ) ;
    ubSentBuf[3] = (unsigned char)( sec >> 0 & 0xFF ) ;
    ubSentBuf[4] = 0 ;
    
    for ( i = 0; i < ubSentBuf[0] - 1; i++ )
    {
        ubSentBuf[ubSentBuf[0] - 1] += ubSentBuf[i] ;
    }

	if ( i2c_set_bus_num( CONFIG_RTX_EFM32_I2C_BUS ) ) 
	{
		printf("unable to set i2c bus\n");
	}
	else
	{
		if ( !i2c_probe( 0xc ) )
		{
			
			if ( i2c_write( 0xc , 0x00 , 0 , (uint8_t *)ubSentBuf , 5 ) ) 
			{
				
				printf("%s:i2c_write:error\n", __func__);
			}
			else
			{
				if ( i2c_read( 0xc , 0x0 , 0 , ubRecvBuf , 3 ) ) 
				{
					printf("%s:i2c_read:error\n", __func__);
				}
				else
				{
					printf("watchdog: %d sec. ([%02X][%02X][%02X])\n", (int)sec ,ubRecvBuf[0],ubRecvBuf[1],ubRecvBuf[2]);
				}
			}
			
		}
		else
		{
			printf("unable to probe efm32\n");
		}
	}
	
	/* reset the i2c bus */
	i2c_set_bus_num( iBus ) ;
#endif
}

