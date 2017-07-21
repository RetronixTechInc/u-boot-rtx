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

void vBootMain_Init( void )
{
	vStorage_Init() ;
	vOptions_Init() ;
	vBootEnv_SetFromOptions( ) ;
	vBootMain_SetWatchdog( ) ;
}

void vBootMain_SetWatchdog( void )
{
	#ifdef CONFIG_RTX_EFM32
	{
		int iWatchDogTime = iOptions_GetWatchDogTimes() ;
		if ( iWatchDogTime != -1 )
		{
			vSet_efm32_watchdog( iWatchDogTime ) ;
		}
	}
	#endif	
}
