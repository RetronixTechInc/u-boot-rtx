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
#include <usb.h>
#include <fs.h>
#include <libfdt.h>
#include <image.h>
#include <environment.h>

#ifdef CONFIG_DYNAMIC_MMC_DEVNO
int mmc_get_env_devno(void) ;
//int get_mmc_env_devno(void) ;
#endif

static unsigned char const bootseldefaultmagiccode[16] = {
	0x10 , 0x01 , 0xC0 , 0x34 , 0x00 , 0x00 , 0x71 , 0x00 ,
	0x00 , 0xA0 , 0x00 , 0xdf , 0x00 , 0x17 , 0x00 , 0x00 
} ;

static unsigned char const bootseldefaultpassword[8] = {
	'2' , '7' , '9' , '9' , '9' , '9' , '2' , '9'
} ;

#ifdef CONFIG_USB_STORAGE
extern int usb_get_stor_dev( void ) ;
#endif

typedef struct __BOOTSEL_INFO__ {
	unsigned long ulCheckCode ;
	unsigned char ubMagicCode[16] ;
	unsigned char ubMAC01[8] ;
	unsigned char ubMAC02[8] ;
	unsigned char ubMAC03[8] ;
	unsigned char ubMAC04[8] ;
	unsigned char ubProductName[128] ;
	unsigned char ubProductSerialNO[64] ;
	unsigned char ubBSPVersion[32] ;
	unsigned long ulPasswordLen ;
	unsigned char ubPassword[32] ;
	unsigned long ulFunction ;
	unsigned long ulCmd ;
	unsigned long ulStatus ;
	unsigned char ubRecv[188] ;
} bootselinfo ;

static bootselinfo bootselinfodata ;
static char bootselnewpassword[32] ;
static int  bootselnewpasswordlen ;
static char bootselconfirmpassword[32] ;
static int  bootselconfirmpasswordlen ;

enum __BOOTSEL_FUNC__{
	DEF_BOOTSEL_FUNC_PASSWORD = 0x00000001 ,
	DEF_BOOTSEL_FUNC_CHANG_PW = 0x00000002 ,
	DEF_BOOTSEL_FUNC_UD_EXTSD = 0x00000004 ,
	DEF_BOOTSEL_FUNC_UD_USB   = 0x00000008 ,
	DEF_BOOTSEL_FUNC_MENU     = 0x00000010 ,
} ;

typedef struct __bootselfunc__ {
	char *        name ;
	unsigned long mask ;
} BOOTSELFUNC ;

static BOOTSELFUNC const bootselfuncarray[] = {
	{ (char *)"password" , DEF_BOOTSEL_FUNC_PASSWORD } ,
	{ (char *)"change"   , DEF_BOOTSEL_FUNC_CHANG_PW } ,
	{ (char *)"extsd"    , DEF_BOOTSEL_FUNC_UD_EXTSD } ,
	{ (char *)"usb"      , DEF_BOOTSEL_FUNC_UD_USB   } ,
	{ (char *)"menu"     , DEF_BOOTSEL_FUNC_MENU     } ,
} ;

void bootsel_adjust_bootargs( void ) ;

static int bootsel_getmmcdevno( void )
{
	#ifdef CONFIG_DYNAMIC_MMC_DEVNO
		return ( mmc_get_env_devno() ) ;
	#else
		return (CONFIG_SYS_MMC_ENV_DEV) ;
	#endif
}

static void bootsel_write_setting_data( void )
{
#ifdef CONFIG_CMD_MMC
	struct mmc *extsd_dev = NULL ;
	int sdid = bootsel_getmmcdevno() ;
	u32 blksize ;
	
	extsd_dev = find_mmc_device( sdid ) ;
	if ( extsd_dev ) 
	{
		if( mmc_init( extsd_dev ) == 0 )
		{
			blksize = extsd_dev->block_dev.block_write( (int)sdid , CONFIG_BOOT_SYSTEM_SETTING_OFFSET , 1 , &bootselinfodata ) ;
			if ( blksize != 1 )
			{
				printf("\n emmc write error.\n") ;
			}
		}
	}
#endif
}

int bootsel_load_logo_data( void )
{
#ifdef CONFIG_CMD_MMC
	struct mmc *extsd_dev = NULL ;
	int sdid = bootsel_getmmcdevno() ;
	u32 blksize ;
	
	extsd_dev = find_mmc_device( sdid ) ;
	if ( extsd_dev ) 
	{
		if( mmc_init( extsd_dev ) == 0 )
		{
			blksize = extsd_dev->block_dev.block_read( (int)sdid , CONFIG_BOOT_SYSTEM_LOGO_OFFSET , CONFIG_BOOT_SYSTEM_LOGO_SIZE , (void *)CONFIG_LOADADDR ) ;
			if ( blksize != CONFIG_BOOT_SYSTEM_LOGO_SIZE )
			{
				printf("\n emmc write error.\n") ;
				return 0 ;
			}
			return 1 ;
		}
	}
#endif
	return 0 ;
}

void bootsel_init( void )
{
	memset( (void *)&bootselinfodata , 0 , sizeof(bootselinfo) ) ;
	#ifdef CONFIG_CMD_MMC
	{
		struct mmc *extsd_dev = NULL ;
		int sdid = bootsel_getmmcdevno() ;
		u32 blksize ;
		void *pbuf = (void *)CONFIG_LOADADDR ;
		int ok = 0 ;
		
		extsd_dev = find_mmc_device( sdid ) ;
		if ( extsd_dev ) 
		{
			if( mmc_init( extsd_dev ) == 0 )
			{
				blksize = extsd_dev->block_dev.block_read( (int)sdid , CONFIG_BOOT_SYSTEM_SETTING_OFFSET , 1 , pbuf ) ;
				if ( blksize == 1 )
				{
					memcpy( (void *)&bootselinfodata , (void *)pbuf , sizeof(bootselinfo) ) ;
					if ( bootselinfodata.ulCheckCode == 0x5AA5AA55 )
					{
						ok = 1 ;
					}
				}
			}
		}
		if ( !ok )
		{
			memcpy( (void *)&bootselinfodata.ubMagicCode , (void *)bootseldefaultmagiccode , 16 ) ;
			memcpy( (void *)&bootselinfodata.ubPassword , (void *)bootseldefaultpassword , 8 ) ;
			bootselinfodata.ulPasswordLen = 8 ;
			bootselinfodata.ulCheckCode = 0x5AA5AA55 ;
			bootsel_write_setting_data( ) ;
		}
	}
	#else
		memcpy( (void *)&bootselinfodata.ubMagicCode , (void *)bootseldefaultmagiccode , 16 ) ;
		memcpy( (void *)&bootselinfodata.ubPassword , (void *)bootseldefaultpassword , 8 ) ;
		bootselinfodata.ulPasswordLen = 8 ;
		bootselinfodata.ulCheckCode = 0x5AA5AA55 ;
	#endif
	bootselnewpasswordlen     = 0 ;
	bootselconfirmpasswordlen = 0 ;
	bootsel_adjust_bootargs( ) ;
}

static int bootsel_load( int fstype , const char *ifname , const char *dev_part_str , const char *filename , int pos , int size , unsigned long addr )
{
	loff_t len_read;

	if ( !file_exists(ifname,dev_part_str,filename,fstype) )
	{
		return 0 ;
	}
	
	if ( fs_set_blk_dev( ifname , dev_part_str , fstype ) )
	{
		return 0 ;
	}

	if (fs_read( filename , addr , pos , size , &len_read) < 0)
	{
		return 0;
	}

	return (int)( len_read ) ;
}

static int bootsel_load_backupsystem( void )
{
	int sdid = 0 ;
	struct mmc *extsd_dev = NULL ;
	
	sdid = bootsel_getmmcdevno() ;
				
	extsd_dev = find_mmc_device( sdid ) ;
		
	if ( !extsd_dev ) 
	{
		return 0 ;
	}
				
	if( mmc_init( extsd_dev ) != 0 )
	{
		return 0 ;
	}

	if ( ! extsd_dev->block_dev.block_read( sdid , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE , (void *)CONFIG_LOADADDR ) )
	{
		return 0 ;
	}
	if ( ! image_check_magic( (const image_header_t *)CONFIG_LOADADDR ) )
	{
		return 0 ;
	}
	//setenv( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm" ) ;
	
	if ( ! extsd_dev->block_dev.block_read( sdid , CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET , CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE , (void *)CONFIG_RD_LOADADDR ) )
	{	
		return 0 ;
	}
	if ( ! image_check_magic( (const image_header_t *)CONFIG_RD_LOADADDR ) )
	{
		return 0 ;
	}
	
	setenv( "bootcmd_update"  , CONFIG_ENG_BOOTCMD ) ;
	
	if ( extsd_dev->block_dev.block_read( sdid , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE , (void *)CONFIG_DTB_LOADADDR ) )
	{
		if ( fdt_magic((const void *)CONFIG_DTB_LOADADDR) == FDT_MAGIC)
		{
			setenv( "bootcmd_update"  , CONFIG_ENG_DTB_BOOTCMD ) ;
		}		
	}
	
	printf("boot from ram disk\n") ;
	setenv( "roption"  , "recovery" ) ;
	run_command( "run bootcmd_update" , 0 ) ;
	return 0 ;
}

static int bootsel_load_system_from_files( const char *ifname ,  const char *dev_part_str , int needcheckcode )
{
	u32 blksize ;
	
	if ( needcheckcode )
	{
		blksize = bootsel_load( FS_TYPE_ANY , ifname , dev_part_str , "check_code" , 0 , 0 , (unsigned long)CONFIG_LOADADDR ) ;
		if( blksize < 16 )
		{
			return 0 ;
		}
		if( memcmp( (void *)CONFIG_LOADADDR , &bootselinfodata.ubMagicCode , 16 ) )
		{
			return 0 ;
		}
	}
	
	if ( ! bootsel_load( FS_TYPE_ANY , ifname , dev_part_str , "uImage-recovery" , 0 , 0 , (unsigned long)CONFIG_LOADADDR ) )
	{
		goto run_backup_command ;
	}
	if ( ! image_check_magic( (const image_header_t *)CONFIG_LOADADDR ) )
	{
		return 0 ;
	}
	//setenv( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm" ) ;
	
	if ( ! bootsel_load( FS_TYPE_ANY , ifname , dev_part_str , "uramdisk-imx6.img" , 0 , 0 , (unsigned long)CONFIG_RD_LOADADDR ) )
	{
		return 0 ;
	}
	if ( ! image_check_magic( (const image_header_t *)CONFIG_RD_LOADADDR ) )
	{
		return 0 ;
	}

	setenv( "bootcmd_update"  , CONFIG_ENG_BOOTCMD ) ;
	
	if ( bootsel_load( FS_TYPE_ANY , ifname , dev_part_str , "recovery.dtb" , 0 , 0  , (unsigned long)CONFIG_DTB_LOADADDR ) )
	{
		if ( fdt_magic((const void *)CONFIG_DTB_LOADADDR) == FDT_MAGIC)
		{
			setenv( "bootcmd_update"  , CONFIG_ENG_DTB_BOOTCMD ) ;
		}
	}

	printf("boot from %s [%s]\n" , ifname , dev_part_str ) ;
	run_command( "run bootcmd_update" , 0 ) ;
	return 0 ;

run_backup_command :
	/* run from backup system */
	bootsel_load_backupsystem( ) ;
	return 0 ;
}

static int bootsel_load_system_from_emmc( int sdid )
{
	int needcheckcode = 0 ;
	char sdidstr[8] ;
	struct mmc *extsd_dev = NULL ;

	sprintf( sdidstr , "%d" , sdid ) ;
	extsd_dev = find_mmc_device( sdid ) ;

	if ( !extsd_dev ) 
	{
		return 0 ;
	}
				
	if( mmc_init( extsd_dev ) != 0 )
	{
		return 0 ;
	}

	if ( extsd_dev->block_dev.block_read( (int)sdid , CONFIG_BOOT_SYSTEM_SETTING_OFFSET , 1 , (void *)CONFIG_LOADADDR ) != 1 )
	{
		return 0 ;
	}

	if( memcmp( (void *)CONFIG_LOADADDR , &bootselinfodata.ubMagicCode , 16 ) )
	{
		
		needcheckcode = 1 ;
		goto run_file_command ;
	}

	if ( ! extsd_dev->block_dev.block_read( sdid , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE , (void *)CONFIG_LOADADDR ) )
	{
		goto run_file_command ;
	}
	if ( ! image_check_magic( (const image_header_t *)CONFIG_LOADADDR ) )
	{
		goto run_file_command ;
	}
	//setenv( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm" ) ;
	
	if ( ! extsd_dev->block_dev.block_read( sdid , CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET , CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE , (void *)CONFIG_RD_LOADADDR ) )
	{	
		goto run_file_command ;
	}
	if ( ! image_check_magic( (const image_header_t *)CONFIG_RD_LOADADDR ) )
	{
		goto run_file_command ;
	}
	
	setenv( "bootcmd_update"  , CONFIG_ENG_BOOTCMD ) ;
	
	if ( extsd_dev->block_dev.block_read( sdid , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE , (void *)CONFIG_DTB_LOADADDR ) )
	{
		if ( fdt_magic((const void *)CONFIG_DTB_LOADADDR) == FDT_MAGIC)
		{
			setenv( "bootcmd_update"  , CONFIG_ENG_DTB_BOOTCMD ) ;
		}
	}
	
	printf("boot from extsd card\n") ;
	setenv( "rstorage" , "mmc" ) ;
	setenv( "roption"  , "update" ) ;
	setenv( "ext_args" , CONFIG_ENG_BOOTARGS ) ;

	run_command( "run bootcmd_update" , 0 ) ;
	goto run_boot_exit ;

run_file_command :
	setenv( "rstorage" , "mmc" ) ;
	setenv( "roption"  , "update" ) ;
	setenv( "ext_args" , CONFIG_ENG_BOOTARGS ) ;

	bootsel_load_system_from_files( "mmc" , sdidstr , needcheckcode ) ;
	
run_boot_exit :
	setenv( "rstorage" , NULL ) ;
	setenv( "roption"  , NULL ) ;
	setenv( "ext_args" , NULL ) ;
	setenv( "bootcmd_update" , NULL ) ;
	return 0 ;
}

static int bootsel_load_system_from_usb( int usbid )
{
	int needcheckcode = 0 ;
	char usbidstr[8] ;
	block_dev_desc_t *stor_dev = NULL;
	
	if ( usbid < 0 )
	{
		return 0 ;
	}
	
	sprintf( usbidstr , "%d" , usbid ) ;
	stor_dev = usb_stor_get_dev( usbid ) ;
	if ( !stor_dev )
	{
		return 0 ;
	}
	
	if ( stor_dev->block_read( usbid , CONFIG_BOOT_SYSTEM_SETTING_OFFSET , 1 , (ulong *)CONFIG_LOADADDR ) != 1 )
	{
		return 0 ;
	}

	if( memcmp( (void *)CONFIG_LOADADDR , &bootselinfodata.ubMagicCode , 16 ) )
	{
		needcheckcode = 1 ;
		goto run_file_command ;
	}
	
	if ( ! stor_dev->block_read( usbid , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE , (ulong *)CONFIG_LOADADDR ) )
	{
		goto run_file_command ;
	}
	if ( ! image_check_magic( (const image_header_t *)CONFIG_LOADADDR ) )
	{
		goto run_file_command ;
	}
	//setenv( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm" ) ;

	if ( ! stor_dev->block_read( usbid , CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET , CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE , (ulong *)CONFIG_RD_LOADADDR ) )
	{
		goto run_file_command ;
	}
	if ( ! image_check_magic( (const image_header_t *)CONFIG_RD_LOADADDR ) )
	{
		goto run_file_command ;
	}
	setenv( "bootcmd_update"  , CONFIG_ENG_BOOTCMD ) ;

	if ( stor_dev->block_read( usbid , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE , (ulong *)CONFIG_DTB_LOADADDR ) )
	{
		if ( fdt_magic((const void *)CONFIG_DTB_LOADADDR) == FDT_MAGIC)
		{
			setenv( "bootcmd_update"  , CONFIG_ENG_DTB_BOOTCMD ) ;
		}
	}

	printf("boot from usb\n") ;
	setenv( "rstorage" , "usb" ) ;
	setenv( "roption"  , "update" ) ;
	setenv( "ext_args" , CONFIG_ENG_BOOTARGS ) ;

	run_command( "run bootcmd_update" , 0 ) ;
	goto run_boot_exit ;
	
run_file_command :
	setenv( "rstorage" , "usb" ) ;
	setenv( "roption"  , "update" ) ;
	setenv( "ext_args" , CONFIG_ENG_BOOTARGS ) ;

	bootsel_load_system_from_files( "usb" , usbidstr , needcheckcode ) ;
	
run_boot_exit :
	setenv( "rstorage" , NULL ) ;
	setenv( "roption"  , NULL ) ;
	setenv( "ext_args" , NULL ) ;
	setenv( "bootcmd_update"  , NULL ) ;
	return 0 ;
}

static void bootsel_checkstorage_mmc( void )
{
#ifdef CONFIG_CMD_MMC
	int sdid = 0 ;
	
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_UD_EXTSD )
	{
		return ;
	}

	for ( sdid = 0 ; sdid < CONFIG_BOOT_SYSTEM_MAX_EXTSD ; sdid ++ )
	{
		if ( sdid == bootsel_getmmcdevno() ) continue ;

		bootsel_load_system_from_emmc( sdid ) ;
	}
#endif
}

static void bootsel_checkstorage_usb( void )
{
#ifdef CONFIG_USB_STORAGE
	int usbid = -1 ;

	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_UD_USB )
	{
		return ;
	}

	usbid = usb_stor_scan(1) ;
	bootsel_load_system_from_usb( usbid ) ;
	
#endif	
}

int bootsel_checkstorage( void )
{
	int ret  = 0 ;
	
	bootsel_checkstorage_mmc( ) ;
	bootsel_checkstorage_usb( ) ;
	
	return ( ret ) ;
}

void bootsel_getpassword( int *len , char *passwd )
{
	char ichar ;
	int  getlen ;
	
	getlen = 0 ;
	
	for (;;) 
	{
		if (tstc()) 
		{   /* we got a key press   */
			ichar = getc();  /* consume input       */
			if( ( ichar == '\r' ) || ( ichar == '\n' ) ) 
			{ /* Enter         */
				if ( getlen < 32 ) 
				{
					passwd[getlen] = 0x00 ;
				}
				else 
				{
					passwd[31] = 0x00 ;
				}
				puts ("\r\n");
				*len = getlen ;
				break ;
			} 
			else if ( ( ichar == 0x08 ) || ( ichar == 0x7F ) ) 
			{
				if ( getlen )
				{
					getlen -- ;
					puts ("\b \b");
				}
				continue ;
			}

			if ( getlen < 31 ) 
			{
				passwd[getlen] = ichar ;
				passwd[getlen+1] = 0x00 ;
			}
			getlen++ ;
			puts( "*" ) ;
		}
	}	
}

void bootsel_password( void )
{
	int times ;
	int len ;
	char password[32] ;

	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_PASSWORD )
	{
		return ;
	}

	printf ("Please Enter Password : ");
	times = 0 ;
	len   = 0 ;

	if ( !bootselinfodata.ulPasswordLen )
	{
		return ;
	}

	if ( bootselinfodata.ulPasswordLen >= 32 )
	{
		return ;
	}
	
	for (;;) 
	{
		bootsel_getpassword( &len , password ) ;
		// check password
		if ( len > 0 ) 
		{
			times ++ ;
			//printf ("Password(%d):%s\n", len , password ) ;
			if ( len == bootselinfodata.ulPasswordLen ) 
			{
				if( memcmp( password , bootselinfodata.ubPassword , bootselinfodata.ulPasswordLen ) == 0 ) 
				{
					break ;
				}
			}
			if ( times >= 3 ) 
			{
				do_reset (NULL, 0, 0, NULL);
			}
			len   = 0 ;
		}
		printf ("Please Enter Password(%d): ", times ) ;
	}
}

void bootsel_newpassword( void )
{
	int ok = 0 ;
	
	bootselnewpasswordlen = 0 ;
	bootselconfirmpasswordlen = 0 ;
	
	printf ("Please Enter the New Password : ");
	bootsel_getpassword( &bootselnewpasswordlen , bootselnewpassword ) ;
	printf ("Please Confirm the Password : ");
	bootsel_getpassword( &bootselconfirmpasswordlen , bootselconfirmpassword ) ;
	
	if ( bootselnewpasswordlen )
	{
		if ( bootselnewpasswordlen == bootselconfirmpasswordlen )
		{
			if ( memcmp( bootselnewpassword , bootselconfirmpassword , bootselnewpasswordlen ) == 0 )
			{
				bootselinfodata.ulPasswordLen = bootselnewpasswordlen ;
				memcpy( bootselinfodata.ubPassword , bootselnewpassword , bootselnewpasswordlen ) ;
				ok = 1 ;
				if ( bootselinfodata.ulCheckCode != 0x5AA5AA55 )
				{
					char ichar ;
					printf ("Do you want to save ? (y/n) : ") ;
					for (;;) 
					{
						if (tstc()) 
						{   /* we got a key press   */
							ichar = getc();  /* consume input       */
							if ( ichar == 'y' || ichar == 'Y' )
							{
								bootselinfodata.ulCheckCode = 0x5AA5AA55 ;
								bootsel_write_setting_data( ) ;
							}
							break ;
						}
					}
				}
				else
				{
					bootsel_write_setting_data( ) ;
				}
			}
		}
	}
	if ( ok )
	{
		printf("\nChange password success.\n");
	}
	else
	{
		printf("\nFail to change password.\n");
	}
}

void bootsel_menu( int sel )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_MENU )
	{
		return ;
	}
	#ifdef CONFIG_CMD_MMC
	switch ( sel )
	{
		case 'u' :
		case 'U' :
			setenv( "rstorage" , "mmc" ) ;
			setenv( "roption" , "normal" ) ;
			setenv( "ext_args" , CONFIG_ENG_BOOTARGS ) ;
			setenv("bootcmd_update", CONFIG_ENG_UKEY_BOOTCMD );
			run_command( "run bootcmd_update" , 0 ) ;
			break ;
		case 'r' :
		case 'R' :
			setenv( "ext_args" , CONFIG_ANDROID_RECOVERY_BOOTARGS ) ;
			setenv("bootcmd_android_recovery", CONFIG_ANDROID_RECOVERY_BOOTCMD );
			run_command( "run bootcmd_android_recovery" , 0 ) ;
			break ;
		case 'p' :
		case 'P' :
			if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_CHANG_PW )
			{
				break ;
			}
			bootsel_password( ) ;
			bootsel_newpassword( ) ;
			break ;
		default :
			break ;
	}
	#endif
}

static unsigned char nibbleFromChar(char c)
{
	if(c >= '0' && c <= '9') return c - '0';
	if(c >= 'a' && c <= 'f') return c - 'a' + 10;
	if(c >= 'A' && c <= 'F') return c - 'A' + 10;
	return 255;
}

void bootsel_set_fec_mac( int inum, char* data )
{
	char* pdata ;
	int i ;
	unsigned char bval[6] ;

	switch(inum)
	{
	case 0 :
		pdata = (char*)bootselinfodata.ubMAC01;
		break ;
	case 1 :
		pdata = (char*)bootselinfodata.ubMAC02;
		break ;
	case 2 :
		pdata = (char*)bootselinfodata.ubMAC03;
		break ;
	case 3 :
		pdata = (char*)bootselinfodata.ubMAC04;
		break ;
	default :
		return ;
	}

	for(i = 0 ; i < strlen(data) ; i ++ )
	{
		bval[i] = 0;
		bval[i] = nibbleFromChar(data[i]);
		if(bval[i] == 255)
		{
			return ;
		}
	}
	for(i = 0 ; i < strlen(data)/2 ; i ++ )
	{
		*pdata = bval[i*2+ 1] + (bval[i*2 ] << 4);
		pdata++;
	}
	*pdata += 1;
	bootsel_write_setting_data( ) ;
}

void bootsel_Read_Mac()
{
	char* commandline ;
	char Mac[32] ;

	if ( bootselinfodata.ubMAC01[6] )
	{
		sprintf( Mac , "fec_mac=%02x:%02x:%02x:%02x:%02x:%02x" ,
			bootselinfodata.ubMAC01[0] , bootselinfodata.ubMAC01[1] ,
			bootselinfodata.ubMAC01[2] , bootselinfodata.ubMAC01[3] ,
			bootselinfodata.ubMAC01[4] , bootselinfodata.ubMAC01[5] 
			) ;
		setenv("bootargs", Mac);
	}
	return ;
}

void bootsel_adjust_bootargs( void )
{
	bootsel_Read_Mac( ) ;
}

static int do_set_bootsel_setting(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	int loop ;
	int inum ;
	
	if ( argc < 3 )
	{
		return CMD_RET_USAGE ;
	}
	
	if ( strcmp( argv[1] , "function" ) == 0 )
	{
		if ( argc < 4 )
		{
			return CMD_RET_USAGE ;
		}
		for ( loop = 0 ; loop < sizeof(bootselfuncarray) / sizeof(BOOTSELFUNC) ; loop ++ )
		{
			if ( strcmp( argv[2] , bootselfuncarray[loop].name ) == 0 )
			{
				if ( strcmp( argv[3] , "enable" ) == 0 )
				{
					bootselinfodata.ulFunction &= ~bootselfuncarray[loop].mask ;
				}
				else
				{
					bootselinfodata.ulFunction |= bootselfuncarray[loop].mask ;
				}
				bootsel_write_setting_data( ) ;
				return CMD_RET_SUCCESS;
			}
		}
	}
	else if ( strcmp( argv[1] , "mac" ) == 0 )
	{
		if ( argc < 4 )
		{
			return CMD_RET_USAGE ;
		}
		inum = simple_strtol(argv[2], NULL, 10) ;

		if(strlen(argv[3]) == 12)
		{
			bootsel_set_fec_mac(inum ,(char*)argv[3]);
			return CMD_RET_SUCCESS;
		}
	}
	else if ( strcmp( argv[1] , "run" ) == 0 )
	{
		if ( argc < 3 )
		{
			return CMD_RET_USAGE ;
		}
		if ( strcmp( argv[2] , "update" ) == 0 )
		{
			bootsel_menu('u');
			return CMD_RET_SUCCESS;
		}
		else if ( strcmp( argv[2] , "recovery" ) == 0 )
		{
			bootsel_menu('r');
			return CMD_RET_SUCCESS;
		}
	}
	
	return CMD_RET_USAGE;
}

U_BOOT_CMD(
	bootsel_set, 4, 0,	do_set_bootsel_setting,
	"set the setting data",
	"- command formated\n"
	"    <class> <data>\n"
	"    <class> <function> <enable/disable>\n"
	"** function class **\n"
	"    function password <enable/disable>\n"
	"    function change   <enable/disable>\n"
	"    function extsd    <enable/disable>\n"
	"    function usb      <enable/disable>\n"
	"    function menu     <enable/disable>\n"
	"** MAC class **\n"
	"    mac 0 000000000000\n"
	"    mac 1 000000000000\n"
	"    mac 2 000000000000\n"
	"    mac 3 000000000000\n"
	"** run class **\n"
	"    run update\n"
	"    run recovery\n"
);

#ifdef CONFIG_BOOT_CMD_RESET_ENV
static int do_reset_env(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	set_default_env(NULL);
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	env_reset, 1, 0,	do_reset_env,
	"reset environment to default",
	""
);
#endif

#ifdef CONFIG_BOOT_SYSTEM_SHOW_SETTING_INFO
static int do_show_setting_info(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	int loop , loop1 ;
	
	printf( "ulCheckCode:%08X\n" , (unsigned int)bootselinfodata.ulCheckCode ) ;
	printf( "ubMagicCode:" ) ;
	for ( loop = 0 ; loop < 16 ; loop ++ )
	{
		if ( loop == 15 )
		{
			printf( "0x%02X" , bootselinfodata.ubMagicCode[loop] ) ;
		}
		else
		{
			printf( "0x%02X," , bootselinfodata.ubMagicCode[loop] ) ;
		}
	}
	printf( "\n" ) ;
	
	for ( loop1 = 0 ; loop1 < 4 ; loop1 ++ )
	{
		printf( "ubMAC0%d:" , loop1 ) ;
		for ( loop = 0 ; loop < 8 ; loop ++ )
		{
			if ( loop == 7 )
			{
				printf( "%02X" , bootselinfodata.ubMAC01[loop+loop1*8] ) ;
			}
			else
			{
				printf( "%02X:" , bootselinfodata.ubMAC01[loop+loop1*8] ) ;
			}
		}
		printf( "\n" ) ;
	}
	
	printf( "Product Name:" ) ;
	for ( loop = 0 ; loop < 128 ; loop ++ )
	{
		if ( bootselinfodata.ubProductName[loop] == 0x00 )
		{
			break ;
		}
		if ( bootselinfodata.ubProductName[loop] >= 32 && bootselinfodata.ubProductName[loop] < 128 )
		{
			printf( "%c" , bootselinfodata.ubProductName[loop] ) ;
		}
		else
		{
			printf( "%c" , '?' ) ;
		}
	}
	printf( "\n" ) ;
	
	printf( "BSP Version:" ) ;
	for ( loop = 0 ; loop < 64 ; loop ++ )
	{
		if ( bootselinfodata.ubProductSerialNO[loop] == 0x00 )
		{
			break ;
		}
		if ( bootselinfodata.ubProductSerialNO[loop] >= 32 && bootselinfodata.ubProductSerialNO[loop] < 128 )
		{
			printf( "%c" , bootselinfodata.ubProductSerialNO[loop] ) ;
		}
		else
		{
			printf( "%c" , '?' ) ;
		}
	}
	printf( "\n" ) ;
	
	printf( "Product Serial:" ) ;
	for ( loop = 0 ; loop < 32 ; loop ++ )
	{
		if ( bootselinfodata.ubBSPVersion[loop] == 0x00 )
		{
			break ;
		}
		if ( bootselinfodata.ubBSPVersion[loop] >= 32 && bootselinfodata.ubBSPVersion[loop] < 128 )
		{
			printf( "%c" , bootselinfodata.ubBSPVersion[loop] ) ;
		}
		else
		{
			printf( "%c" , '?' ) ;
		}
	}
	printf( "\n" ) ;
	
	return CMD_RET_SUCCESS;
}

/* get setting information*/
U_BOOT_CMD(
	setting_info, 1, 0,	do_show_setting_info,
	"show setting info.",
	""
);

void bootsel_ddr_calibration( void )
{
	#if defined(CONFIG_CMD_MMC) || defined(CONFIG_DDR_AUTO_CALIBRATION)
	unsigned long ulDDRcal[128] ;
	struct mmc *extsd_dev = NULL ;
	u32 blksize ;
	void *pbuf = (void *)CONFIG_LOADADDR ;
#ifdef CONFIG_DYNAMIC_MMC_DEVNO
	int sdid = ( mmc_get_env_devno() ) ;
#else
	int sdid = (CONFIG_SYS_MMC_ENV_DEV) ;
#endif
	int ok = 1 ;
	unsigned int reg;

	memset( (void *)&ulDDRcal , 0 , sizeof(ulDDRcal) ) ;

	extsd_dev = find_mmc_device( sdid ) ;
	if ( extsd_dev )
	{
		if( mmc_init( extsd_dev ) == 0 )
		{
			blksize = extsd_dev->block_dev.block_read( (int)sdid , 0x3 , 1 , pbuf ) ;
			if ( blksize == 1 )
			{
				memcpy( (void *)&ulDDRcal , (void *)pbuf , sizeof(ulDDRcal) ) ;
				if ( ulDDRcal[0x0] == 0x00 )
				{
					ok = 0 ;
				}
			}
		}
	}

	if ( !ok )
	{
		ulDDRcal[0x00] = 0x00000001 ;
		printf("\nChange DDR Calibration information success.\n");
		reg = readl(MMDC_P0_BASE_ADDR + 0x80C);
		ulDDRcal[42*2] = reg ;
		printf("MPWLDECTRL0_OFFSET : 0x%08x\n", reg);
		reg = readl(MMDC_P0_BASE_ADDR + 0x810);
		ulDDRcal[43*2] = reg ;
		printf("MPWLDECTRL1_1FFSET : 0x%08x\n", reg);
		reg = readl(MMDC_P1_BASE_ADDR + 0x80C);
		ulDDRcal[44*2] = reg ;
		printf("MPWLDECTRL0_OFFSET : 0x%08x\n", reg);
		reg = readl(MMDC_P1_BASE_ADDR + 0x810);
		ulDDRcal[45*2] = reg ;
		printf("MPWLDECTRL1_1FFSET : 0x%08x\n", reg);

		reg = readl(MMDC_P0_BASE_ADDR + 0x83C);
		ulDDRcal[46*2] = reg ;
		printf("MPDGCTRL0_OFFSET : 0x%08x\n", reg);
		reg = readl(MMDC_P0_BASE_ADDR + 0x840);
		ulDDRcal[47*2] = reg ;
		printf("MPDGCTRL1_OFFSET : 0x%08x\n", reg);
		reg = readl(MMDC_P1_BASE_ADDR + 0x83C);
		ulDDRcal[48*2] = reg ;
		printf("MPDGCTRL0_OFFSET : 0x%08x\n", reg);
		reg = readl(MMDC_P1_BASE_ADDR + 0x840);
		ulDDRcal[49*2] = reg ;
		printf("MPDGCTRL1_OFFSET : 0x%08x\n", reg);

		reg = readl(MMDC_P0_BASE_ADDR + 0x848);
		ulDDRcal[50*2] = reg ;
		printf("MPRDDLCTL_OFFSET : 0x%08x\n", reg);
		reg = readl(MMDC_P1_BASE_ADDR + 0x848);
		ulDDRcal[51*2] = reg ;
		printf("MPRDDLCTL_OFFSET : 0x%08x\n", reg);

		reg = readl(MMDC_P0_BASE_ADDR + 0x850);
		ulDDRcal[52*2] = reg ;
		printf("MPWRDLCTL_OFFSET : 0x%08x\n", reg);
		reg = readl(MMDC_P1_BASE_ADDR + 0x850);
		ulDDRcal[53*2] = reg ;
		printf("MPWRDLCTL_OFFSET : 0x%08x\n", reg);

		blksize = extsd_dev->block_dev.block_write( (int)sdid , 0x3 , 1 , ulDDRcal ) ;
	}
	#endif

}


#endif
