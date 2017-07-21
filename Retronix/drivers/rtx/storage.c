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


#ifdef CONFIG_ENV_IS_IN_MMC
	extern int mmc_get_env_dev(void) ;
#else
	int mmc_get_env_dev(void) {	return (0) ; }
#endif

static int       iStorageID[STORAGE_TYPE_COUNT]      = {0} ;
static int       iStorageIsExist[STORAGE_TYPE_COUNT] = {0} ;

struct blk_desc *pStorageBlockDesc[STORAGE_TYPE_COUNT] = {NULL} ;

int iStorage_GetBootDevNo( void )
{
	return ( mmc_get_env_dev() ) ;
}

void vStorage_Init( void )
{
	int iLoop ;
	
	for ( iLoop = 0 ; iLoop < STORAGE_TYPE_COUNT ; iLoop ++ )
	{
		iStorageID[iLoop]        = -1 ;
		iStorageIsExist[iLoop]   = 0 ;
		pStorageBlockDesc[iLoop] = NULL ;
	}

	#ifdef CONFIG_USB_STORAGE
	if ( iOptions_UsbStorageEnable() )
	{
		if ( usb_init() >= 0 )
		{
			iStorageID[STORAGE_TYPE_USB] = usb_stor_scan( 1 ) ;
		
			if ( iStorageID[STORAGE_TYPE_USB] >= 0 )
			{
				pStorageBlockDesc[STORAGE_TYPE_USB] = blk_get_devnum_by_type( IF_TYPE_USB , iStorageID[IF_TYPE_USB] ) ;
				if ( pStorageBlockDesc[STORAGE_TYPE_USB] != NULL )
				{
					iStorageIsExist[STORAGE_TYPE_USB] = 1 ;
				}
			}
		}
	}
	#endif

	#ifdef CONFIG_CMD_MMC
	{
		struct mmc *pMMCDevNode ;
		int iPutLoop ;
		
		iStorageID[STORAGE_TYPE_BOOT] = iStorage_GetBootDevNo() ;
		pMMCDevNode = find_mmc_device( iStorageID[STORAGE_TYPE_BOOT] ) ;
		if ( pMMCDevNode ) 
		{
			if( mmc_init( pMMCDevNode ) == 0 )
			{
				pStorageBlockDesc[STORAGE_TYPE_BOOT] = mmc_get_blk_desc(pMMCDevNode) ;
				if ( pStorageBlockDesc[STORAGE_TYPE_BOOT] != NULL )
				{
					iStorageIsExist[STORAGE_TYPE_BOOT] = 1 ;
				}
			}
		}
		if ( iOptions_ExtSDEnable( ) )
		{
			iPutLoop = STORAGE_TYPE_EXTSD0 ;
			for ( iLoop = 0 ; iLoop < CONFIG_RTX_BOOT_SYSTEM_MAX_EXTSD ; iLoop ++ )
			{		
				if ( iLoop == iStorage_GetBootDevNo() )
				{
					continue ;
				}

				iStorageID[iPutLoop] = iLoop ;
				pMMCDevNode = find_mmc_device( iStorageID[iPutLoop] ) ;
				if ( pMMCDevNode ) 
				{
					if( mmc_init( pMMCDevNode ) == 0 )
					{
						pStorageBlockDesc[iPutLoop] = mmc_get_blk_desc(pMMCDevNode) ;
						if ( pStorageBlockDesc[iPutLoop] != NULL )
						{
							iStorageIsExist[iPutLoop] = 1 ;
						}
					}
				}
				
				iPutLoop ++ ;
				
				if (iPutLoop > STORAGE_TYPE_EXTSD3)
				{
					break ;
				}
			}
		}
	}
	#endif	
}

int iStorage_IsExist( enum STORAGE_TYPE iType )
{
	if ( iStorageIsExist[iType] )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int iStorage_FileIsExist( enum STORAGE_TYPE iType , char *pFileName )
{
	if ( iStorageIsExist[iType] )
	{
		char cDevType[8] ;
		char cDevPart[8] ;
		
		if ( iType == STORAGE_TYPE_USB )
		{
			sprintf( cDevType , "usb" ) ;
		}
		else
		{
			sprintf( cDevType , "mmc" ) ;
		}
		sprintf( cDevPart , "%d" , iStorageID[iType] ) ;
		
		return ( file_exists( (const char *)cDevType , (const char *)cDevPart , (const char *)pFileName , FS_TYPE_ANY) ) ;
	}
	return ( 0 ) ;
}

int iStorage_LoadFile( enum STORAGE_TYPE iType , char *pFileName , void *pBuffer )
{
	if ( iStorageIsExist[iType] )
	{
		char cDevType[8] ;
		char cDevPart[8] ;
		int iRet ;
		loff_t iGetSize ;
		
		
		if ( iType == STORAGE_TYPE_USB )
		{
			sprintf( cDevType , "usb" ) ;
		}
		else
		{
			sprintf( cDevType , "mmc" ) ;
		}
		sprintf( cDevPart , "%d" , iStorageID[iType] ) ;
		
		if( ! file_exists( (const char *)cDevType , (const char *)cDevPart , (const char *)pFileName , FS_TYPE_ANY ) )
		{
			return ( 0 ) ;
		}
		
		if ( fs_set_blk_dev( (const char *)cDevType , (const char *)cDevPart , FS_TYPE_ANY ) )
		{
			return ( 0 ) ;
		}
		
		iRet = fs_read( pFileName , (ulong)pBuffer , 0 , 0 , &iGetSize ) ;
		
		if ( iRet < 0 )
		{
			return ( 0 ) ;
		}
		
		return ((int)iGetSize) ;
	}
	return ( 0 ) ;
}

unsigned long ulStorage_Read( enum STORAGE_TYPE iType , lbaint_t ulStart , lbaint_t ulBlkCnt, void *pBuffer )
{
	if ( !iStorageIsExist[iType] )
	{
		return ( 0 ) ;
	}
	return ( blk_dread( pStorageBlockDesc[iType] , ulStart , ulBlkCnt , pBuffer ) ) ;
}

unsigned long ulStorage_Write( enum STORAGE_TYPE iType , lbaint_t ulStart , lbaint_t ulBlkCnt, void *pBuffer )
{
	if ( !iStorageIsExist[iType] )
	{
		return ( 0 ) ;
	}
	return ( blk_dwrite( pStorageBlockDesc[iType] , ulStart , ulBlkCnt , pBuffer ) ) ;
}
