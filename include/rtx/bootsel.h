/*
 * Copyright (C) 2012
 * 
 * (C) Copyright 2012 AAI
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

#ifndef _BOOTSEL_H_
#define _BOOTSEL_H_

int bootsel_func_password( void ) ;
int bootsel_func_password_chg( void ) ;
int bootsel_func_extsd( void ) ;
int bootsel_func_usbstorage( void ) ;
int bootsel_func_menu( void ) ;
int bootsel_func_changestorage( void ) ;
int bootsel_func_scanmagiccode_self( void ) ;

int bootsel_load_logo_data( void ) ;
void bootsel_init( void ) ;
int bootsel_checkstorage( void ) ;
void bootsel_password( void ) ;
void bootsel_menu( int sel ) ;

#endif	/* _BOOTSEL_H_ */
