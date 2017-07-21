/*
 * Copyright (C) 2014
 * 
 * (C) Copyright 2014 Retronix
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <mmc.h>
#include <part.h>
#include <usb.h>
#include <fs.h>
#include <libfdt.h>
#include <image.h>
#include <environment.h>

#ifdef CONFIG_RTX_EFM32
	#include "../../include/efm32.h"
#endif

#include "../../include/bootsystem.h"

static void vBootEnv_SetEthMac( void )
{
	int  iLoop ;
	char sbMacID[32] = { 0 };
	char sbMacData[512] = { 0 };
    unsigned char *pMac = NULL;
    
	for ( iLoop = 0 ; iLoop < 4 ; iLoop++ )
	{
		pMac = pOptions_GetMacData( iLoop ) ;
		if ( pMac == NULL )
		{
			continue ;
		}
		
		sbMacID[0] = 0 ; 
		sbMacData[0] = 0 ; 
		if ( pMac[6] )
		{
			sprintf( sbMacData , "%02x:%02x:%02x:%02x:%02x:%02x" , pMac[0] , pMac[1] , pMac[2] , pMac[3] , pMac[4] , pMac[5] ) ;
			sprintf( sbMacID , "mac%d_val", iLoop+1 ) ;
			setenv( sbMacID , sbMacData ) ;
		}
	}	
}

static void vBootEnv_SetLVDS( void )
{
	char sbLVDSData[512] ;
	int  iLoop = 0 ;
	unsigned long * pGetValue ;
	unsigned long ulChecksum = 0 ;

	pGetValue = pOptions_GetLVDSData( ) ;

	for ( iLoop = 1 ; iLoop < 16 ; iLoop ++ )
	{
		ulChecksum += pGetValue[iLoop] ;
	}

	if (ulChecksum == pGetValue[0])
	{
		sprintf( sbLVDSData , "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d" ,
			(int)pGetValue[1] , (int)pGetValue[2] , (int)pGetValue[3] ,
			(int)pGetValue[4] , (int)pGetValue[5] , (int)pGetValue[6] ,
			(int)pGetValue[7] , (int)pGetValue[8] ,
			(int)pGetValue[9] , (int)pGetValue[10] ,
			(int)pGetValue[11] , (int)pGetValue[12]
			) ;
		setenv( "lvds_val" , sbLVDSData ) ;
	}	
}

void vBootEnv_SetFromOptions( void )
{
	vBootEnv_SetEthMac( ) ;
	vBootEnv_SetLVDS( ) ;
}

#ifdef CONFIG_RTX_BOOT_CMD_RESET_ENV
static int do_BootEnvReset(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	set_default_env(NULL);
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	env_reset, 1, 0, do_BootEnvReset,
	"reset environment to default",
	""
);
#endif

