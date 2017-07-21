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

DECLARE_GLOBAL_DATA_PTR;

static int iBootUpdate_CheckFileMagicCode(enum STORAGE_TYPE iType)
{
	if ( iType >= STORAGE_TYPE_COUNT )
	{
		return ( 0 ) ;
	}
	
	#if defined(CONFIG_RTX_LOADADDR)
	{
		void * pBuf = (void *)CONFIG_RTX_LOADADDR ;
		
		if ( iStorage_LoadFile( iType , "check_code" , (void *)pBuf) )
		{
			if ( iOptions_CheckMagicCode( (void *)pBuf ) )
			{
				return 1 ;
			}
		}
	}
	#endif
	
	return ( 0 ) ;
}

static int iBootUpdate_CheckRawMagicCode( enum STORAGE_TYPE iType )
{
	if ( iType >= STORAGE_TYPE_COUNT )
	{
		return ( 0 ) ;
	}
	
	#if defined(CONFIG_RTX_PARTITION_0_DATA_OFFSET) && defined(CONFIG_RTX_PARTITION_0_DATA_SIZE) && defined(CONFIG_RTX_LOADADDR)
	{
		u32    u32Size ;
		void * pBuf = (void *)CONFIG_RTX_LOADADDR ;
		
		u32Size = ulStorage_Read( iType , (CONFIG_RTX_PARTITION_0_DATA_OFFSET) , (CONFIG_RTX_PARTITION_0_DATA_SIZE) , pBuf);

		if ( u32Size == (CONFIG_RTX_PARTITION_0_DATA_SIZE) )
		{
			if ( iOptions_CheckMagicCode( (void *)pBuf ) )
			{
				return ( 1 ) ;
			}
		}
	}
	#endif
	
	return ( 0 ) ;
}

static int iBootUpdate_CheckMagicCode( int *iType )
{
	int iLoop ;
	int iCheckEnable[STORAGE_TYPE_COUNT] ;
	
	for ( iLoop = 0 ; iLoop < STORAGE_TYPE_COUNT ; iLoop ++ )
	{
		iCheckEnable[iLoop] = 0 ;
	}
	
	if ( iOptions_BootStorageScanMagicCodeEnable( ) )
	{
		iCheckEnable[STORAGE_TYPE_BOOT] = 1 ;
	}
	
	if ( iOptions_ExtSDEnable( ) )
	{
		for ( iLoop = STORAGE_TYPE_EXTSD0 ; iLoop < STORAGE_TYPE_EXTSD3+1 ; iLoop ++ )
		{
			iCheckEnable[iLoop] = 1 ;
		}
	}
	
	if ( iOptions_UsbStorageEnable() )
	{
		iCheckEnable[STORAGE_TYPE_USB] = 1 ;
	}
	
	for ( iLoop = 0 ; iLoop < STORAGE_TYPE_COUNT ; iLoop ++ )
	{
		if ( iCheckEnable[iLoop] )
		{
			if ( iStorage_IsExist( iLoop ) )
			{
				if ( iBootUpdate_CheckRawMagicCode( iLoop ) || iBootUpdate_CheckFileMagicCode( iLoop ) )
				{
					*iType      = iLoop ;
					return ( 1 ) ;
				}
			}			
		}
	}
	
	return ( 0 ) ;
}

static int iBootUpdate_GetKernel( int iType , int iMode )
{
	#if defined(CONFIG_RTX_LOADADDR)
	{
		void * pBuf = (void *)CONFIG_RTX_LOADADDR ;
		if ( ! iMode )
		{
			if ( iStorage_LoadFile( iType , "uImage-recovery" , (void *)pBuf) )
			{
				if ( image_check_magic( (const image_header_t *)pBuf ) )
				{
					return ( 1 ) ;
				}
			}
		}
		
		#if defined(CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_OFFSET) && defined(CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_SIZE)
		{
			u32    u32Size ;
				
			u32Size = ulStorage_Read( iType , (CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_OFFSET) , (CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_SIZE) , pBuf);

			if ( u32Size == (CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_SIZE) )
			{
				if ( image_check_magic( (const image_header_t *)pBuf ) )
				{
					return ( 1 ) ;
				}
			}
		}
		#endif
	}
	#endif
			
	return ( 0 ) ;
}

static int iBootUpdate_GetKernelDTB( int iType , int iMode )
{
	#if defined(CONFIG_RTX_LOADADDR_DTB)
	{
		void * pBuf = (void *)CONFIG_RTX_LOADADDR_DTB ;
		
		if ( ! iMode )
		{
			if ( iStorage_LoadFile( iType , "recovery.dtb" , (void *)pBuf) )
			{
				if ( fdt_magic((const void *)pBuf) == FDT_MAGIC)
				{
					return ( 1 ) ;
				}
			}
		}
		
		#if defined(CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_DTB_OFFSET) && defined(CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_DTB_SIZE)
		{
			u32    u32Size ;
				
			u32Size = ulStorage_Read( iType , (CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_DTB_OFFSET) , (CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_DTB_SIZE) , pBuf);

			if ( u32Size == (CONFIG_RTX_PARTITION_0_RECOVERY_KERNEL_DTB_SIZE) )
			{
				if ( fdt_magic((const void *)pBuf) == FDT_MAGIC)
				{
					return ( 1 ) ;
				}
			}
		}
		#endif
	}
	#endif
			
	return ( 0 ) ;
}

static int iBootUpdate_GetRamDisk( int iType , int iMode )
{
	#if defined(CONFIG_RTX_LOADADDR_RD)
	{
		void * pBuf = (void *)CONFIG_RTX_LOADADDR_RD ;
		
		if ( ! iMode )
		{
			if ( iStorage_LoadFile( iType , "uramdisk-recovery.img" , (void *)pBuf) )
			{
				if ( image_check_magic( (const image_header_t *)pBuf ) )
				{
					return ( 1 ) ;
				}
			}
		}
		
		#if defined(CONFIG_RTX_PARTITION_0_RAM_DISK2_OFFSET) && defined(CONFIG_RTX_PARTITION_0_RAM_DISK2_SIZE)
		{
			u32    u32Size ;
				
			u32Size = ulStorage_Read( iType , (CONFIG_RTX_PARTITION_0_RAM_DISK2_OFFSET) , (CONFIG_RTX_PARTITION_0_RAM_DISK2_SIZE) , pBuf);

			if ( u32Size == (CONFIG_RTX_PARTITION_0_RAM_DISK2_SIZE) )
			{
				if ( image_check_magic( (const image_header_t *)pBuf ) )
				{
					return ( 1 ) ;
				}
			}
		}
		#endif
	}
	#endif
			
	return ( 0 ) ;
}

static void vBootUpdate_SetBaseEnvArgv( int iMode )
{
	/* arguments */
	setenv( "fdt_high"    , "0xffffffff" ) ;
	setenv( "initrd_high" , "0xffffffff" ) ;
	setenv( "splashpos"   , "m,m" ) ;
	setenv( "silent"      , "0" ) ;
	setenv( "bootargs_base" , "setenv bootargs no_console_suspend" ) ;
	
	#if defined(CONSOLE_DEV) && defined(CONFIG_BAUDRATE)
		setenv( "bootargs_console" , "setenv bootargs ${bootargs} console=" CONSOLE_DEV "," __stringify(CONFIG_BAUDRATE) ) ;
	#else
		setenv( "bootargs_console" , "setenv bootargs ${bootargs} console=/dev/ttymxc0,115200" ) ;
	#endif
					
	#if defined(CONFIG_RTX_ROOTFS)
		setenv( "ext_args" , "setenv bootargs ${bootargs} root=/dev/ram0 rdinit=/sbin/init rdisk_option=${roption} storage=${rstorage} mmcroot=" CONFIG_RTX_ROOTFS ) ;
	#else
		setenv( "ext_args" , "setenv bootargs ${bootargs} root=/dev/ram0 rdinit=/sbin/init rdisk_option=${roption} storage=${rstorage}" ) ;
	#endif
	
	/* command */
	if ( iMode )
	{
		#if defined(CONFIG_RTX_LOADADDR) && defined(CONFIG_RTX_LOADADDR_RD) && defined(CONFIG_RTX_LOADADDR_DTB)
			setenv( "bootcmd_update"  , "run bootargs_base bootargs_console ext_args; bootm " __stringify(CONFIG_RTX_LOADADDR) " " __stringify(CONFIG_RTX_LOADADDR_RD) " " __stringify(CONFIG_RTX_LOADADDR_DTB) ) ;
		#endif
	}
	else
	{
		#if defined(CONFIG_RTX_LOADADDR) && defined(CONFIG_RTX_LOADADDR_RD)
			setenv( "bootcmd_update"  , "run bootargs_base bootargs_console ext_args; bootm " __stringify(CONFIG_RTX_LOADADDR) " " __stringify(CONFIG_RTX_LOADADDR_RD) ) ;
		#endif
	}
}

static void vBootUpdate_CheckStorage( void )
{
	int iType = 0 ;
	
	if ( iBootUpdate_CheckMagicCode( &iType ) )
	{
		if ( ! iBootUpdate_GetKernel( iType , 0 ) )
		{
			return ;
		}

		if ( ! iBootUpdate_GetRamDisk( iType , 0 ) )
		{
			return ;
		}
		
		if ( iType == STORAGE_TYPE_USB )
		{
			setenv( "rstorage" , "usb" ) ;
		}
		else
		{
			setenv( "rstorage" , "mmc" ) ;
		}
		
		setenv( "roption"     , "update" ) ;
		
		vBootUpdate_SetBaseEnvArgv( iBootUpdate_GetKernelDTB( iType , 0 ) ) ;
		
		run_command( "run bootcmd_update" , 0 ) ;
		
		vBootMain_SetWatchdog( ) ;
	}
}

int iBootUpdate_LoadBackupSystem( void )
{
	if ( ! iBootUpdate_GetKernel( STORAGE_TYPE_BOOT , 1 ) )
	{
		return ( 0 ) ;
	}

	if ( ! iBootUpdate_GetRamDisk( STORAGE_TYPE_BOOT , 1 ) )
	{
		return ( 0 ) ;
	}
	
	vBootUpdate_SetBaseEnvArgv( iBootUpdate_GetKernelDTB( STORAGE_TYPE_BOOT , 1 ) ) ;

	run_command( "run bootcmd_update" , 0 ) ;
	
	return ( 0 ) ;
}

int iBootUpdate_Check( void )
{
	int iRet  = 0 ;
	char* pStr ;
	
	gd->flags &= ~GD_FLG_SILENT;
	
	vBootUpdate_CheckStorage( ) ;
	
	pStr = getenv("silent") ;
	
	if( pStr && strncmp(pStr, "1", 1) == 0 )
	{
		gd->flags |= GD_FLG_SILENT;
	}
		
	return ( iRet ) ;
}

