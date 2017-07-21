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

static void vPassword_Get( int *pLen , char *pPasswd )
{
	char sbChar ;
	int  iGetLen ;
	int  iCount = 0 ;
	
	iGetLen = 0 ;
	
	for (;;) 
	{
		udelay(10000); // 10ms
		iCount++;
		if ( iCount > 1500){	// len = 0 ,if not push enter in 10ms * 1500 = 15s.
			*pLen = 0 ;
			break ; 
		}

		if (tstc()) 
		{   /* we got a key press   */
			sbChar = getc();  /* consume input       */
			if( ( sbChar == '\r' ) || ( sbChar == '\n' ) ) 
			{ /* Enter         */
				if ( iGetLen < 32 ) 
				{
					pPasswd[iGetLen] = 0x00 ;
				}
				else 
				{
					pPasswd[31] = 0x00 ;
				}
				puts ("\r\n");
				*pLen = iGetLen ;
				break ;
			} 
			else if ( ( sbChar == 0x08 ) || ( sbChar == 0x7F ) ) 
			{
				if ( iGetLen )
				{
					iGetLen -- ;
					puts ("\b \b");
				}
				continue ;
			}

			if ( iGetLen < 31 ) 
			{
				pPasswd[iGetLen] = sbChar ;
				pPasswd[iGetLen+1] = 0x00 ;
			}
			iGetLen++ ;
			puts( "*" ) ;
		}
	}	
}

void vPassword_Process( void )
{
	int  iTimes ;
	int  iLen ;
	char sbPassword[32] ;
	char *pRunCmd ;

	if ( !iOptions_PasswordEnable( ) )
	{
		return ;
	}

	printf ("Please Enter Password : ") ;
	
	iTimes = 0 ;
	iLen   = 0 ;

	for (;;) 
	{
		vPassword_Get( &iLen , sbPassword ) ;
		
		// check password
		if ( iLen > 0 ) 
		{
			iTimes ++ ;
			//printf ("Password(%d):%s\n", iLen , password ) ;
			if ( iOptions_CheckPassword( iLen , sbPassword ) )
			{
				break ;
			}
			
			/*
			if( memcmp( sbPassword , "androidrecovery" , 15 ) == 0 )
			{
				iBootMenu_Select( 'a' );
			}
			else if( memcmp( sbPassword , "rtxupdate" , 9 ) == 0 )
			{
				iBootMenu_Select( 'u' );
			}
			*/
			
			if ( iTimes >= 3 ) 
			{
				pRunCmd = getenv ("bootcmd");
				if(pRunCmd)
					run_command (pRunCmd, 0);
				else
					do_reset (NULL, 0, 0, NULL);
			}
			
			iLen   = 0 ;
		}
		else
		{
			pRunCmd = getenv ("bootcmd");
			if(pRunCmd)
				run_command (pRunCmd, 0);
			
			do_reset (NULL, 0, 0, NULL);
		}
		
		printf ("Please Enter Password(%d): ", iTimes ) ;
	}
}

void vPassword_SetNewPassword( void )
{
	int  iOK = 0 ;
	int  iNewLen ;
	char sbNewPassword[32] ;
	int  iConfirmLen ;
	char sbConfirmPassword[32] ;
	
	iNewLen = 0 ;
	iConfirmLen = 0 ;
	
	printf ("Please Enter the New Password : ");
	vPassword_Get( &iNewLen , sbNewPassword ) ;
	printf ("Please Confirm the Password : ");
	vPassword_Get( &iConfirmLen , sbConfirmPassword ) ;
	
	if ( iNewLen )
	{
		if ( iNewLen == iConfirmLen )
		{
			if ( memcmp( sbNewPassword , sbConfirmPassword , iNewLen ) == 0 )
			{
				vOptions_SetPassword( iNewLen , sbNewPassword ) ;
				
				iOK = 1 ;
			}
		}
	}
	
	if ( iOK )
	{
		printf("\nChange password success.\n");
	}
	else
	{
		printf("\nFail to change password.\n");
	}
}

