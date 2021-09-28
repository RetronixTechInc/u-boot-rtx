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
#include <linux/libfdt.h>
#include <image.h>
#include <environment.h>

#if defined(CONFIG_RTX_EFM32)
#include <rtx/efm32.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_DYNAMIC_MMC_DEVNO
int get_mmc_env_devno(void) ;
#endif

#ifdef CONFIG_BISHOP_MAGIC_PACKAGE
#define CONFIG_RTX_MAGIC_PACKAGE "This file is RTX magic package for start up from extsdcard.\n"
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

typedef struct __LVDS_PAR__ {
	unsigned long ulchecksum ;
	unsigned long ulrefresh ;
	unsigned long ulxres ;
	unsigned long ulyres ;
	unsigned long pixclock ;
	unsigned long ulleft_margin ;
	unsigned long ulright_margin ;
	unsigned long ulupper_margin ;
	unsigned long ullower_margin ;
	unsigned long ulhsync_len ;
	unsigned long ulvsync_len ;
	unsigned long ulsync ;
	unsigned long ulvmode ;
	unsigned long ulRecv[3] ;
} lvdspar ;

typedef struct __BOOTSEL_INFO__ {
	unsigned long ulCheckCode ;
	unsigned char ubMagicCode[16] ;
	unsigned char ubMAC01[8] ;      //第1組MAC，fec_mac=****
	unsigned char ubMAC02[8] ;      //第2組MAC,smsc_mac=****
	unsigned char ubMAC03[8] ;      //第3組MAC
	unsigned char ubMAC04[8] ;      //第4組MAC
	unsigned char ubProductName[128] ;  //產品名稱
	unsigned char ubProductSerialNO[64] ;   //產品序號
	unsigned char ubBSPVersion[32] ;    //BSP版本
	unsigned long ulPasswordLen ;       //密碼長度
	unsigned char ubPassword[32] ;      //密碼
	unsigned long ulFunction ;          //bootsel command 功能
	unsigned long ulCmd ;
	unsigned long ulStatus ;
	unsigned long ulDataExistInfo ;
	lvdspar sLVDSVal ;           //lvds 參數,lvds_val=****
	unsigned long ulMcuWatchDog ; //MCU watch dog time. 0 is disable.
	unsigned char ubRecv01[116] ;
	unsigned char ubProductSerialNO_Vendor[64] ;    //生產，產品序號
	unsigned char ubMAC01_Vendor[8] ;      //生產，第1組MAC
	unsigned char ubMAC02_Vendor[8] ;      //生產，第2組MAC
	unsigned char ubMAC03_Vendor[8] ;      //生產，第3組MAC
	unsigned char ubMAC04_Vendor[8] ;      //生產，第4組MAC
	unsigned char ubRecv02[416] ;
} bootselinfo ;

static bootselinfo bootselinfodata ;
static char bootselnewpassword[32] ;
static int  bootselnewpasswordlen ;
static char bootselconfirmpassword[32] ;
static int  bootselconfirmpasswordlen ;

enum __BOOTSEL_FUNC__{
	DEF_BOOTSEL_FUNC_PASSWORD    = 0x00000001 ,
	DEF_BOOTSEL_FUNC_CHANG_PW    = 0x00000002 ,
	DEF_BOOTSEL_FUNC_UD_EXTSD    = 0x00000004 ,
	DEF_BOOTSEL_FUNC_UD_USB      = 0x00000008 ,
	DEF_BOOTSEL_FUNC_MENU        = 0x00000010 ,
	DEF_BOOTSEL_FUNC_CHG_STORAGE = 0x00000020 ,
	DEF_BOOTSEL_FUNC_SCANFILE_SELF = 0x00000040 ,
	DEF_BOOTSEL_FUNC_MENUKEY     = 0x00000080 ,
} ;

typedef struct __bootselfunc__ {
	char *        name ;
	unsigned long mask ;
} BOOTSELFUNC ;

static BOOTSELFUNC const bootselfuncarray[] = {
	{ (char *)"password" , DEF_BOOTSEL_FUNC_PASSWORD    } ,
	{ (char *)"change"   , DEF_BOOTSEL_FUNC_CHANG_PW    } ,
	{ (char *)"extsd"    , DEF_BOOTSEL_FUNC_UD_EXTSD    } ,
	{ (char *)"usb"      , DEF_BOOTSEL_FUNC_UD_USB      } ,
	{ (char *)"menu"     , DEF_BOOTSEL_FUNC_MENU        } ,
	{ (char *)"storage"  , DEF_BOOTSEL_FUNC_CHG_STORAGE } ,
	{ (char *)"selfmagic", DEF_BOOTSEL_FUNC_SCANFILE_SELF } ,
	{ (char *)"menukey"  , DEF_BOOTSEL_FUNC_MENUKEY     } ,
} ;

int bootsel_func_password( void )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_PASSWORD )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int bootsel_func_password_chg( void )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_CHANG_PW )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int bootsel_func_extsd( void )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_UD_EXTSD )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int bootsel_func_usbstorage( void )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_UD_USB )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int bootsel_func_menu( void )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_MENU )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int bootsel_func_changestorage( void )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_CHG_STORAGE )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;	
}

int bootsel_func_scanmagiccode_self( void )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_SCANFILE_SELF )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int bootsel_func_menukey( void )
{
	if ( bootselinfodata.ulFunction & DEF_BOOTSEL_FUNC_MENUKEY )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

#ifdef CONFIG_DYNAMIC_MMC_DEVNO
	extern int mmc_get_env_dev(void) ;
#endif

static int bootsel_getmmcdevno( void )
{
	#ifdef CONFIG_DYNAMIC_MMC_DEVNO
		return ( mmc_get_env_dev() ) ;
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
			blksize = blk_dwrite( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_SETTING_OFFSET , CONFIG_BOOT_SYSTEM_SETTING_SIZE , &bootselinfodata ) ;
			if ( blksize != CONFIG_BOOT_SYSTEM_SETTING_SIZE )
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
			blksize = blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_LOGO_OFFSET , CONFIG_BOOT_SYSTEM_LOGO_SIZE , (void *)CONFIG_LOADADDR ) ;
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

static void bootsel_set_fec_mac( void )
{
	int loop ;
	char macnum[32] = { 0 };
	char setstr[512] = { 0 };
    unsigned char *pMac = NULL;
    
	for ( loop = 0 ; loop < 4 ; loop++ )
	{
        
        switch( loop )
		{
			case 0 : pMac = &bootselinfodata.ubMAC01[0] ; break ;
			case 1 : pMac = &bootselinfodata.ubMAC02[0] ; break ;
			case 2 : pMac = &bootselinfodata.ubMAC03[0] ; break ;
			case 3 : pMac = &bootselinfodata.ubMAC04[0] ; break ;
			default :
				pMac = NULL ;
		}
		macnum[0] = 0 ; 
		setstr[0] = 0 ; 
		if ( pMac[6] && pMac != NULL)
		{
			sprintf( setstr , "%02x:%02x:%02x:%02x:%02x:%02x" ,
				pMac[0] , pMac[1] , pMac[2] , pMac[3] , pMac[4] , pMac[5] ) ;
			sprintf( macnum , "mac%d_val", loop+1 ) ;
			env_set( macnum , setstr ) ;
		}
	}	
}

static void bootsel_set_lvds_par( void )
{
	char setstr[512] ;
	int loop = 0 ;
	unsigned long * ulval ;
	unsigned long ulcount = 0 ;

	ulval = &bootselinfodata.sLVDSVal.ulrefresh ;
	for ( loop = 1 ; loop < sizeof(bootselinfodata.sLVDSVal)/sizeof(unsigned long) ; loop ++ )
	{
		if( *ulval < 4096 )
		{
			ulcount += *ulval ;
		}
		else
		{
			if( loop == 4 )
			{
				ulcount += *ulval ;
			}
			else
			{	
				ulcount = 0 ;
				break;
			}
		}
		ulval++;
	}

	if (ulcount != 0)
	{
		if (ulcount == bootselinfodata.sLVDSVal.ulchecksum)
		{
			sprintf( setstr , "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d" ,
				(int)bootselinfodata.sLVDSVal.ulrefresh , (int)bootselinfodata.sLVDSVal.ulxres , (int)bootselinfodata.sLVDSVal.ulyres ,
				(int)bootselinfodata.sLVDSVal.pixclock ,(int)bootselinfodata.sLVDSVal.ulleft_margin , (int)bootselinfodata.sLVDSVal.ulright_margin ,
				(int)bootselinfodata.sLVDSVal.ulupper_margin , (int)bootselinfodata.sLVDSVal.ullower_margin ,
				(int)bootselinfodata.sLVDSVal.ulhsync_len , (int)bootselinfodata.sLVDSVal.ulvsync_len ,
				(int)bootselinfodata.sLVDSVal.ulsync , (int)bootselinfodata.sLVDSVal.ulvmode
				) ;
			env_set( "lvds_val" , setstr ) ;
		}
		
	}
}
static void bootsel_adjust_bootargs( void )
{
	bootsel_set_fec_mac( ) ;
	bootsel_set_lvds_par( ) ;
}

static void vbootsel_def_func( void )
{
	bootselinfodata.ulFunction = 0 ;
	#ifdef CONFIG_BOOTSEL_FUNC_PASSWORD
	bootselinfodata.ulFunction |= DEF_BOOTSEL_FUNC_PASSWORD ;
	#endif
	#ifdef CONFIG_BOOTSEL_FUNC_CHANG_PW
	bootselinfodata.ulFunction |= DEF_BOOTSEL_FUNC_CHANG_PW ;
	#endif
	#ifdef CONFIG_BOOTSEL_FUNC_UD_EXTSD
	bootselinfodata.ulFunction |= DEF_BOOTSEL_FUNC_UD_EXTSD ;
	#endif
	#ifdef CONFIG_BOOTSEL_FUNC_UD_USB
	bootselinfodata.ulFunction |= DEF_BOOTSEL_FUNC_UD_USB ;
	#endif
	#ifdef CONFIG_BOOTSEL_FUNC_MENU
	bootselinfodata.ulFunction |= DEF_BOOTSEL_FUNC_MENU ;
	#endif
	#ifdef CONFIG_BOOTSEL_FUNC_CHG_STORAGE
	bootselinfodata.ulFunction |= DEF_BOOTSEL_FUNC_CHG_STORAGE ;
	#endif
	#ifdef CONFIG_BOOTSEL_FUNC_SCANFILE_SELF
	bootselinfodata.ulFunction |= DEF_BOOTSEL_FUNC_SCANFILE_SELF ;
	#endif
	#ifdef CONFIG_BOOTSEL_FUNC_MENUKEY
	bootselinfodata.ulFunction |= DEF_BOOTSEL_FUNC_MENUKEY ;
	#endif

    bootselinfodata.ulMcuWatchDog = ( unsigned long ) (CONFIG_MCU_WATCHDOG_TIME & 0xFFFF);

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
				blksize = blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_SETTING_OFFSET , CONFIG_BOOT_SYSTEM_SETTING_SIZE , pbuf ) ;
				if ( blksize == CONFIG_BOOT_SYSTEM_SETTING_SIZE )
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
			vbootsel_def_func();
			bootselinfodata.ulCheckCode = 0x5AA5AA55 ;
			bootsel_write_setting_data( ) ;
		}
	}
	#else
		memcpy( (void *)&bootselinfodata.ubMagicCode , (void *)bootseldefaultmagiccode , 16 ) ;
		memcpy( (void *)&bootselinfodata.ubPassword , (void *)bootseldefaultpassword , 8 ) ;
		bootselinfodata.ulPasswordLen = 8 ;
		vbootsel_def_func();
		bootselinfodata.ulCheckCode = 0x5AA5AA55 ;
	#endif
	bootselnewpasswordlen     = 0 ;
	bootselconfirmpasswordlen = 0 ;
	#if defined(CONFIG_RTX_EFM32)
	vBootsel_Efm32Loadconfig( ) ;
	#endif
	bootsel_adjust_bootargs( ) ;
	#if defined(CONFIG_RTX_EFM32)
    	vSet_efm32_watchdog( bootselinfodata.ulMcuWatchDog ) ;
	#endif
}

static int bootsel_load( int fstype , const char *ifname , const char *dev_part_str , const char *filename , int pos , int size , unsigned long addr )
{
	int len_read;
	loff_t actread;

	if ( !file_exists(ifname,dev_part_str,filename,fstype) )
	{
		return 0 ;
	}
	
	if ( fs_set_blk_dev( ifname , dev_part_str , fstype ) )
	{
		return 0 ;
	}

	len_read = fs_read( filename , addr , pos , size , &actread ) ;

	if (len_read < 0)
	{
		return 0;
	}

	return (int)( actread ) ;
}

int bootsel_load_backupsystem( void )
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

	if ( ! blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE , (void *)CONFIG_LOADADDR ) )
	{
		return 0 ;
	}

//	if ( ! image_check_magic( (const image_header_t *)CONFIG_LOADADDR ) )
//	{
//		return 0 ;
//	}

	//setenv( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm" ) ;

	if ( ! blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_URAMDISK_FS_OFFSET , CONFIG_BOOT_SYSTEM_URAMDISK_FS_SIZE , (void *)CONFIG_RD_LOADADDR ) )
	{
		return 0 ;
	}

//	if ( ! image_check_magic( (const image_header_t *)CONFIG_RD_LOADADDR ) )
//	{
//		return 0 ;
//	}

	env_set( "bootcmd_update"  , CONFIG_ENG_BOOTCMD ) ;

	if ( blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE , (void *)CONFIG_DTB_LOADADDR ) )
	{
		if ( fdt_magic((const void *)CONFIG_DTB_LOADADDR) == FDT_MAGIC)
		{
			env_set( "bootcmd_update"  , CONFIG_ENG_DTB_BOOTCMD ) ;
		}
	}

	printf("boot from ram disk\n") ;
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
	//skip kernel image header check
//	if ( ! image_check_magic( (const image_header_t *)CONFIG_LOADADDR ) )
//	{
//		return 0 ;
//	}
	//env_set( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm" ) ;
	if ( ! bootsel_load( FS_TYPE_ANY , ifname , dev_part_str , "uramdisk-recovery.img" , 0 , 0 , (unsigned long)CONFIG_RD_LOADADDR ) )
	{
		return 0 ;
	}
//	if ( ! image_check_magic( (const image_header_t *)CONFIG_RD_LOADADDR ) )
//	{
//		return 0 ;
//	}
	env_set( "bootcmd_update"  , CONFIG_ENG_BOOTCMD ) ;
	
	if ( bootsel_load( FS_TYPE_ANY , ifname , dev_part_str , "recovery.dtb" , 0 , 0  , (unsigned long)CONFIG_DTB_LOADADDR ) )
	{
		if ( fdt_magic((const void *)CONFIG_DTB_LOADADDR) == FDT_MAGIC)
		{
			env_set( "bootcmd_update"  , CONFIG_ENG_DTB_BOOTCMD ) ;
		}
	}
	printf("boot from %s [%s]\n" , ifname , dev_part_str ) ;
	run_command( "run bootcmd_update" , 0 ) ;
	return 0 ;

run_backup_command :
	/* run from backup system */
	env_set( "roption" , "recovery" ) ;
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

#ifdef CONFIG_BISHOP_MAGIC_PACKAGE
	if ( blk_dread( mmc_get_blk_desc(extsd_dev) , 0 , CONFIG_BOOT_SYSTEM_SETTING_SIZE , (ulong *)CONFIG_LOADADDR ) != CONFIG_BOOT_SYSTEM_SETTING_SIZE )
	{
		return 0 ;
	}

	if( !memcmp( (const char *)CONFIG_LOADADDR , CONFIG_RTX_MAGIC_PACKAGE , strlen( CONFIG_RTX_MAGIC_PACKAGE ) ))
	{
		goto run_old_command ;
	}	
#endif
	if ( blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_SETTING_OFFSET , CONFIG_BOOT_SYSTEM_SETTING_SIZE , (void *)CONFIG_LOADADDR ) != CONFIG_BOOT_SYSTEM_SETTING_SIZE )
	{
		return 0 ;
	}

	if( memcmp( (void *)CONFIG_LOADADDR , &bootselinfodata.ubMagicCode , 16 ) )
	{
		needcheckcode = 1 ;
		goto run_file_command ;
	}

	if ( ! blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE , (void *)CONFIG_LOADADDR ) )
	{
		goto run_file_command ;
	}
//	if ( ! image_check_magic( (const image_header_t *)CONFIG_LOADADDR ) )
//	{
//		goto run_file_command ;
//	}
	//env_set( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm" ) ;
	
	if ( ! blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET , CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE , (void *)CONFIG_RD_LOADADDR ) )
	{	
		goto run_file_command ;
	}
//	if ( ! image_check_magic( (const image_header_t *)CONFIG_RD_LOADADDR ) )
//	{
//		goto run_file_command ;
//	}
	
	env_set( "bootcmd_update"  , CONFIG_ENG_BOOTCMD ) ;
	
	if ( blk_dread( mmc_get_blk_desc(extsd_dev) , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE , (void *)CONFIG_DTB_LOADADDR ) )
	{
		if ( fdt_magic((const void *)CONFIG_DTB_LOADADDR) == FDT_MAGIC)
		{
			env_set( "bootcmd_update"  , CONFIG_ENG_DTB_BOOTCMD ) ;
		}
	}
	
	printf("boot from extsd card\n") ;
	env_set( "rstorage" , "mmc" ) ;
	env_set( "roption"  , "update" ) ;
	env_set( "ext_args" , CONFIG_ENG_BOOTARGS ) ;

	run_command( "run bootcmd_update" , 0 ) ;
	goto run_boot_exit ;

#ifdef CONFIG_BISHOP_MAGIC_PACKAGE
run_old_command:
	env_set( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm ${loadaddr} 0x800 0x2000;mmc read ${rd_loadaddr} 0x3000 0x2000 " ) ;
	printf("boot from extsd card\n") ;
	env_set( "rstorage" , "mmc" ) ;
	env_set( "roption"  , "update" ) ;
	env_set( "ext_args" , "setenv bootargs ${bootargs} root=/dev/ram0 rdinit=/sbin/init" ) ;
	run_command( "run bootcmd_update" , 0 ) ;
	goto run_boot_exit ;
#endif

run_file_command :
	env_set( "rstorage" , "mmc" ) ;
	env_set( "roption"  , "update" ) ;
	env_set( "ext_args" , CONFIG_ENG_BOOTARGS ) ;

	bootsel_load_system_from_files( "mmc" , sdidstr , needcheckcode ) ;
	
run_boot_exit :
	env_set( "rstorage" , NULL ) ;
	env_set( "roption"  , NULL ) ;
	env_set( "ext_args" , NULL ) ;
	env_set( "bootcmd_update" , NULL ) ;
	return 0 ;
}

static int bootsel_load_system_from_usb( int usbid )
{
	int needcheckcode = 0 ;
	char usbidstr[8] ;
	struct blk_desc *stor_dev = NULL;

	if ( usbid < 0 )
	{
		return 0 ;
	}

	sprintf( usbidstr , "%d" , usbid ) ;
	stor_dev = blk_get_devnum_by_type(IF_TYPE_USB, usbid ) ;
	if ( !stor_dev )
	{
		return 0 ;
	}
#ifdef CONFIG_BISHOP_MAGIC_PACKAGE
	if ( blk_dread( stor_dev , 0 , CONFIG_BOOT_SYSTEM_SETTING_SIZE , (ulong *)CONFIG_LOADADDR ) != CONFIG_BOOT_SYSTEM_SETTING_SIZE )
	{
		return 0 ;
	}
	if( !memcmp( (const char *)CONFIG_LOADADDR , CONFIG_RTX_MAGIC_PACKAGE , strlen( CONFIG_RTX_MAGIC_PACKAGE ) ))
	{
		goto run_old_command ;
	}
//	printf("read: %s, sizeof: %d, strlen: %d,strlen2: %d\n ", CONFIG_LOADADDR , sizeof(CONFIG_LOADADDR), strlen(CONFIG_LOADADDR),strlen( CONFIG_RTX_MAGIC_PACKAGE ) ) ;
#endif
	if ( blk_dread( stor_dev , CONFIG_BOOT_SYSTEM_SETTING_OFFSET , CONFIG_BOOT_SYSTEM_SETTING_SIZE , (ulong *)CONFIG_LOADADDR ) != CONFIG_BOOT_SYSTEM_SETTING_SIZE )
	{
		return 0 ;
	}

	if( memcmp( (void *)CONFIG_LOADADDR , &bootselinfodata.ubMagicCode , 16 ) )
	{
		needcheckcode = 1 ;
		goto run_file_command ;
	}

	if ( ! blk_dread( stor_dev , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_SIZE , (ulong *)CONFIG_LOADADDR ) )
	{
		goto run_file_command ;
	}
//	if ( ! image_check_magic( (const image_header_t *)CONFIG_LOADADDR ) )
//	{
//		goto run_file_command ;
//	}
	//env_set( "bootcmd_update"  , "run bootargs_base ext_args set_display set_mem; bootm" ) ;

	if ( ! blk_dread( stor_dev , CONFIG_BOOT_SYSTEM_UPDATE_FS_OFFSET , CONFIG_BOOT_SYSTEM_UPDATE_FS_SIZE , (ulong *)CONFIG_RD_LOADADDR ) )
	{
		goto run_file_command ;
	}
//	if ( ! image_check_magic( (const image_header_t *)CONFIG_RD_LOADADDR ) )
//	{
//		goto run_file_command ;
//	}
	env_set( "bootcmd_update"  , CONFIG_ENG_BOOTCMD ) ;

	if ( blk_dread( stor_dev , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_OFFSET , CONFIG_BOOT_SYSTEM_RECOVERY_KERNEL_DTB_SIZE , (ulong *)CONFIG_DTB_LOADADDR ) )
	{
		if ( fdt_magic((const void *)CONFIG_DTB_LOADADDR) == FDT_MAGIC)
		{
			env_set( "bootcmd_update"  , CONFIG_ENG_DTB_BOOTCMD ) ;
		}
	}

	printf("boot from usb\n") ;
	env_set( "rstorage" , "usb" ) ;
	env_set( "roption"  , "update" ) ;
	env_set( "ext_args" , CONFIG_ENG_BOOTARGS ) ;

	run_command( "run bootcmd_update" , 0 ) ;
	goto run_boot_exit ;
	
#ifdef CONFIG_BISHOP_MAGIC_PACKAGE
run_old_command:
	env_set( "bootcmd_update"  , "run bootargs_ramdisk;mmc dev 1;mmc read ${loadaddr} 0x800 0x2000;mmc read ${rd_loadaddr} 0x3000 0x2000;bootm ${loadaddr} ${rd_loadaddr}" ) ;
	printf("boot from extsd card\n") ;
	env_set( "bootargs_ramdisk" , "setenv bootargs console=ttymxc3 root=/dev/ram0 rootwait rw rdinit=/sbin/init\0" ) ;
	run_command( "run bootcmd_update" , 0 ) ;
	goto run_boot_exit ;
#endif

run_file_command :
	env_set( "rstorage" , "usb" ) ;
	env_set( "roption"  , "update" ) ;
	env_set( "ext_args" , CONFIG_ENG_BOOTARGS ) ;

	bootsel_load_system_from_files( "usb" , usbidstr , needcheckcode ) ;
	
run_boot_exit :
	env_set( "rstorage" , NULL ) ;
	env_set( "roption"  , NULL ) ;
	env_set( "ext_args" , NULL ) ;
	env_set( "bootcmd_update"  , NULL ) ;
	return 0 ;
}

static void bootsel_checkstorage_mmc( void )
{
#ifdef CONFIG_CMD_MMC
	int sdid = 0 ;
	
	if ( !bootsel_func_extsd() )
	{
		return ;
	}

	for ( sdid = 0 ; sdid < CONFIG_BOOT_SYSTEM_MAX_EXTSD ; sdid ++ )
	{

		//if ( sdid == bootsel_getmmcdevno() && !bootsel_func_scanmagiccode_self() ) continue ;

		bootsel_load_system_from_emmc( sdid ) ;
	}
#endif
}

static void bootsel_checkstorage_usb( void )
{
#ifdef CONFIG_USB_STORAGE
	int usbid = -1 ;

	if ( !bootsel_func_usbstorage() )
	{
		return ;
	}

	if (usb_init() < 0)
		return;
	else
		usbid = usb_stor_scan(1) ;
	bootsel_load_system_from_usb( usbid ) ;
	
#endif	
}

int bootsel_checkstorage( void )
{
	int ret  = 0 ;
	char* s ;
	
	gd->flags &= ~GD_FLG_SILENT;
	bootsel_checkstorage_mmc( ) ;
	bootsel_checkstorage_usb( ) ;
	
	s = env_get("silent") ;
	if(s && strncmp(s, "1", 1) == 0 )
		gd->flags |= GD_FLG_SILENT;
	#if defined(CONFIG_RTX_EFM32)
	vSet_efm32_watchdog( bootselinfodata.ulMcuWatchDog ) ;	
	#endif
	return ( ret ) ;
}

static void bootsel_getpassword( int *len , char *passwd )
{
	char ichar ;
	int  getlen ;
	int count = 0 ;
	
	getlen = 0 ;
	
	for (;;) 
	{
		udelay(10000); // 10ms
		count++;
		if ( count > 1500){	// len = 0 ,if not push enter in 10ms * 1500 = 15s.
			*len = 0 ;
			break ; 
		}

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

extern void bootsel_menu( int sel );
void bootsel_password( void )
{
	int times ;
	int len ;
	char password[32] ;
	char *s;

	if ( !bootsel_func_password() )
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
			if( memcmp( password , "androidrecovery" , 15 ) == 0 ){
					bootsel_menu( 'a' );
			}else if( memcmp( password , "rtxupdate" , 9 ) == 0 ){
					bootsel_menu( 'u' );
			}
			if ( times >= 3 ) 
			{
				s = env_get ("bootcmd");
				if(s)
					run_command (s, 0);
				else
					do_reset (NULL, 0, 0, NULL);
			}
			len   = 0 ;
		}else{
			s = env_get ("bootcmd");
			if(s)
				run_command (s, 0);
			else
				do_reset (NULL, 0, 0, NULL);
		}
		printf ("Please Enter Password(%d): ", times ) ;
	}
}

static void bootsel_newpassword( void )
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
	if ( !bootsel_func_menu() )
	{
		return ;
	}
	#ifdef CONFIG_CMD_MMC
	switch ( sel )
	{
		case 'u' :
		case 'U' :
		case 'r' :
		case 'R' :
			{
				if ( sel == 'r' || sel == 'R' )
				{
					env_set( "roption" , "recovery" ) ;
				}
				else
				{
					env_set( "roption" , "update" ) ;
				}
			    env_set( "rstorage" , "mmc" ) ;
			    env_set( "ext_args" , CONFIG_ENG_BOOTARGS ) ;
			    bootsel_load_backupsystem( ) ;
			}
			break ;
		case 'a' :
		case 'A' :
			break ;
		case 'p' :
		case 'P' :
			if ( !bootsel_func_password_chg() )
			{
				break ;
			}
			//bootsel_password( ) ;
			bootsel_newpassword( ) ;
			break ;
		default :
			break ;
	}
	#endif
}

static int bootsel_hex_string_to_binary_dec( int ch )
{
	if ( ch >= 'a' && ch <= 'f' )
	{
		ch = ch - 'a' + 10 ;
	}

	else if ( ch >= 'A' && ch <= 'F' )
	{
		ch = ch - 'A' + 10 ;
	}
	else if ( ch >= '0' && ch <= '9' )
	{
		ch = ch - '0' ;
	}
	else
	{
		return ( -1 ) ;
	}
	return ( ch ) ;
}

static int do_set_bootsel_setting(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	int loop ;
	int value = 0;
	unsigned long ulval = 0 ;
	int ch ;
	unsigned char * pMac ;
	lvdspar lvds_temp ;
	unsigned long* ullvds ;
	unsigned long* ullvds_temp ;

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
					bootselinfodata.ulFunction |= bootselfuncarray[loop].mask ;
				}
				else
				{
					bootselinfodata.ulFunction &= ~bootselfuncarray[loop].mask ;
				}
				bootsel_write_setting_data( ) ;
				return CMD_RET_SUCCESS;
			}
		}
	}
	else if ( strcmp( argv[1] , "mac" ) == 0 )
	{
		pMac = 0 ;
		if ( argc < 4 )
		{
			return CMD_RET_USAGE ;
		}
		
		switch( argv[2][0] )
		{
			case '0' : pMac = &bootselinfodata.ubMAC01[0] ; break ;
			case '1' : pMac = &bootselinfodata.ubMAC02[0] ; break ;
			case '2' : pMac = &bootselinfodata.ubMAC03[0] ; break ;
			case '3' : pMac = &bootselinfodata.ubMAC04[0] ; break ;
			default :
				return CMD_RET_USAGE ;
		}
		
		for ( loop = 0 ; loop <  6 ; loop ++ )
		{
			value = 0 ;
			ch = bootsel_hex_string_to_binary_dec( argv[3][loop*2] ) ;
			if ( ch == -1 )
			{
				return CMD_RET_USAGE ;
			}
			value = value + ch * 16 ;
			ch = bootsel_hex_string_to_binary_dec( argv[3][loop*2+1] ) ;
			if ( ch == -1 )
			{
				return CMD_RET_USAGE ;
			}
			value = value + ch ;
			pMac[loop] = value ;
		}
		pMac[6] = 1 ;
		bootsel_write_setting_data( ) ;
		return CMD_RET_SUCCESS;
	}
	else if ( strcmp( argv[1] , "menu" ) == 0 )
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
		else if ( strcmp( argv[2] , "android_recovery" ) == 0 )
		{
			bootsel_menu('a');
			return CMD_RET_SUCCESS;
		}
		else if ( strcmp( argv[2] , "password_change" ) == 0 )
		{
			bootsel_menu('p');
			return CMD_RET_SUCCESS;
		}
	}
	else if ( strcmp( argv[1] , "lvds" ) == 0 )
	{
		if ( argc < 4 )
		{
			return CMD_RET_USAGE ;
		}
		if ( strcmp( argv[2] , "parameter" ) == 0 )
		{
			memset(&lvds_temp, 0, sizeof(lvdspar)) ;
			ullvds_temp = &lvds_temp.ulrefresh ;
			for(loop = 0 ; loop < strlen(argv[3]) ; loop ++)
			{
				if ( argv[3][loop] >= '0' && argv[3][loop] <= '9' )
				{
					ch =  argv[3][loop] - '0' ;
					value *= 10 ;
					value += ch ;
				}
				else if ( argv[3][loop] == ',' )
				{
					*ullvds_temp = value ;
					lvds_temp.ulchecksum += value ;
					ullvds_temp++ ;
					value = 0 ;
				}
				else
				{
					return CMD_RET_USAGE ;
				}	
			}
			*ullvds_temp = value ;
			lvds_temp.ulchecksum += value ;
			
			ullvds_temp = &lvds_temp.ulchecksum ;
			ullvds = &bootselinfodata.sLVDSVal.ulchecksum ;

			for ( loop = 0 ; loop < sizeof(bootselinfodata.sLVDSVal)/sizeof(unsigned long) ; loop ++ )
			{
				*ullvds = *ullvds_temp ;
				ullvds++ ;
				ullvds_temp++;
			}
			//bootsel_set lvds parameter 60,1280,720,12345,100,10,50,20,10,10,0,1
			bootsel_write_setting_data( ) ;	
			return CMD_RET_SUCCESS;
		}
	}
	else if ( strcmp( argv[1] , "watchdog" ) == 0 )
	{
		if ( argc < 4 )
		{
			return CMD_RET_USAGE ;
		}
		if ( strcmp( argv[2] , "time" ) == 0 )
		{
            if(strlen(argv[3]) < 6)
            {
                for(loop = 0 ; loop < strlen(argv[3]) ; loop ++)
                {
                    if ( argv[3][loop] >= '0' && argv[3][loop] <= '9' )
                    {
                        ch =  argv[3][loop] - '0' ;
                        ulval *= 10 ;
                        ulval += ch ;
                    }
                    else
                    {
                        return CMD_RET_USAGE ;
                    }	
                }
            }
            else
            {
                return CMD_RET_USAGE ;
            }
            
            if(ulval <= 0xFFFF)
            {
                bootselinfodata.ulMcuWatchDog = ulval ;
                bootsel_write_setting_data( ) ;	
                return CMD_RET_SUCCESS;
            }
            else
            {
                return CMD_RET_USAGE ;
            }
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
	"    function storage  <enable/disable>\n"
	"    function selfmagic  <enable/disable>\n"
	"    function menukey  <enable/disable>\n"
	"** MAC class **\n"
	"    mac 0 <000000000000>\n"
	"    mac 1 <000000000000>\n"
	"    mac 2 <000000000000>\n"
	"    mac 3 <000000000000>\n"
	"** menu class **\n"
	"    menu update\n"
	"    menu recovery\n"
	"    menu android_recovery\n"
	"    menu password_change\n"
	"** lvds class **\n"
	"    lvds parameter <refresh,xres,yres,pixclock,left_margin,right_margin,up_margin,low_margin,hsync_len,vsync_len,sync,vmode>\n"
	"** watchdog class **\n"
	"    watchdog time 0(max 65535 seconds.)\n"
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

#ifdef CONFIG_BOOT_CMD_RESET_SETTING
static int do_reset_setting(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	memset( (void *)&bootselinfodata , 0 , sizeof(bootselinfo) ) ;

	memcpy( (void *)&bootselinfodata.ubMagicCode , (void *)bootseldefaultmagiccode , 16 ) ;
	memcpy( (void *)&bootselinfodata.ubPassword , (void *)bootseldefaultpassword , 8 ) ;
	bootselinfodata.ulPasswordLen = 8 ;
	vbootsel_def_func();
	bootselinfodata.ulCheckCode = 0x5AA5AA55 ;
	bootsel_write_setting_data( ) ;
	
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	setting_reset, 1, 0,	do_reset_setting,
	"reset setting to default",
	""
);
#endif

#ifdef CONFIG_BOOT_SYSTEM_SHOW_SETTING_INFO
static int do_show_setting_info(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	int loop , loop1 ;
	unsigned long * ulval ;
	
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
	
	printf( "Product Serial:" ) ;
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
	
	printf( "BSP Version:" ) ;
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

	for ( loop = 0 ; loop < sizeof(bootselfuncarray) / sizeof(BOOTSELFUNC) ; loop ++ )
	{
		if ( bootselinfodata.ulFunction & bootselfuncarray[loop].mask )
		{
			printf( "function %s : enable \n", bootselfuncarray[loop].name ) ;
		}
		else
		{
			printf( "function %s : disable \n", bootselfuncarray[loop].name ) ;
		}
	}
	
	printf( "lvds parameter:" ) ;
	ulval = &bootselinfodata.sLVDSVal.ulchecksum ;
	for ( loop = 0 ; loop < sizeof(bootselinfodata.sLVDSVal)/sizeof(unsigned long) ; loop ++ )
	{
		if ( loop == (sizeof(bootselinfodata.sLVDSVal)/sizeof(unsigned long))-1 )
		{
			printf( "%d" , (int)*ulval++ ) ;
		}
		else
		{
			printf( "%d," , (int)*ulval++ ) ;
		}
	}
	printf( "\n" ) ;

	printf( "McuWatchDog seconds:" ) ;
    printf( "%d" , (unsigned int)(bootselinfodata.ulMcuWatchDog & 0xFFFF) ) ;
	printf( "\n" ) ;

	for ( loop1 = 0 ; loop1 < 4 ; loop1 ++ )
	{
		printf( "ubMAC0%d_Vendor:" , loop1 ) ;
		for ( loop = 0 ; loop < 8 ; loop ++ )
		{
			if ( loop == 7 )
			{
				printf( "%02X" , bootselinfodata.ubMAC01_Vendor[loop+loop1*8] ) ;
			}
			else
			{
				printf( "%02X:" , bootselinfodata.ubMAC01_Vendor[loop+loop1*8] ) ;
			}
		}
		printf( "\n" ) ;
	}

	printf( "Product Serial_Vendor:" ) ;
	for ( loop = 0 ; loop < 64 ; loop ++ )
	{
		if ( bootselinfodata.ubProductSerialNO_Vendor[loop] == 0x00 )
		{
			break ;
		}
		if ( bootselinfodata.ubProductSerialNO_Vendor[loop] >= 32 && bootselinfodata.ubProductSerialNO_Vendor[loop] < 128 )
		{
			printf( "%c" , bootselinfodata.ubProductSerialNO_Vendor[loop] ) ;
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

void vBootsel_Efm32MakeCheckSum( unsigned char *ubData )
{
	int iLoop , iMaxLoop ;
	unsigned char ubCheckSum = 0 ;

	iMaxLoop = ubData[0] ;
	for ( iLoop = 0 ; iLoop < iMaxLoop-1 ; iLoop ++ )
	{
		ubCheckSum += ubData[iLoop] ;
	}
	ubData[iMaxLoop-1] = ubCheckSum ;

	//printf( "[%s-%d] CheckSum %02X \n" , __FILE__ , __LINE__ , ubCheckSum ) ;
}

void vBootsel_Efm32Loadconfig( void )
{
	struct udevice *dev;
	int ret;
	unsigned char ubSentBuf[128] = { 0 } ;
	unsigned char ubRecvBuf[128] = { 0 } ;
	unsigned char *ubAdr ;
	int iLoop ,iLoopMac ;
	int iCount ;
	int iLoadFlag ;
	unsigned char * pMac ;

	iLoadFlag = 1 ;

	ubSentBuf[0] = 5 ;
	ubSentBuf[1] = 0x91 ;

	ubAdr = (unsigned char *)&ubSentBuf[2] ;
	ubAdr[0] = (unsigned char)(10 >> (0 * 8));

	vBootsel_Efm32MakeCheckSum( ubSentBuf ) ;

	/* Configure EFM32: 7bit address 0x0c */
	ret = i2c_get_chip_for_busnum(CONFIG_MCU_WDOG_BUS, 0x0c, 0, &dev);
	if (ret) {
		printf("Cannot find efm32: %d\n", ret);
	}
	else
	{
		iCount = 0 ;
		while ( iCount < 100 )
		{
			if ( dm_i2c_write( dev, 0x00 , (uint8_t *)ubSentBuf , 5 ) )
			{
				if ( iCount == 0 )
				{
					printf("%s:i2c_write:error count = %2d\n", __func__ , iCount );
				}
				else
				{
					printf("\b\b\b%2d ", iCount );
				}
			}
			else
			{
				if ( dm_i2c_read( dev , 0x0 , ubRecvBuf , 3 + 120 ) )
				{
					printf("\n%s:i2c_read:error count = %2d\n", __func__ , iCount );
					break ;
				}
				else
				{
					//check MAC01
					for ( iLoop = 0 ; iLoop < 6 ; iLoop ++ )
					{
						if(iLoop % 6 == 0)
						{
							iLoadFlag = 0 ;
						}
						if ( ubRecvBuf[2 + iLoop + 64 + 32] == 0 )
						{
						}
						else
						{
							iLoadFlag = 1 ;
						}
					}
					if ( iLoadFlag == 1 )
					{
						for ( iLoop = 0 ; iLoop < 6 ; iLoop ++ )
						{
							if ( bootselinfodata.ubMAC01[iLoop] == 0 )
							{
							}
							else
							{
								iLoadFlag = 0 ;
							}
						}
					}
					printf( "Load MAC&Serial : %d\n" , iLoadFlag ) ;

					break ;
				}
			}
			iCount ++ ;
		}

		//load config
		if ( iLoadFlag == 1 )
		{
			//Mac
			pMac = 0 ;
			for ( iLoopMac = 0 ; iLoopMac < 4 ; iLoopMac++ )
			{
				switch( iLoopMac )
				{
					case 0 : pMac = &bootselinfodata.ubMAC01[0] ; break ;
					case 1 : pMac = &bootselinfodata.ubMAC02[0] ; break ;
					case 2 : pMac = &bootselinfodata.ubMAC03[0] ; break ;
					case 3 : pMac = &bootselinfodata.ubMAC04[0] ; break ;
					default :
						pMac = NULL ;
				}
				for ( iLoop = 0 ; iLoop < 6 ; iLoop ++ )
				{
					pMac[iLoop] = ubRecvBuf[ 2 + iLoop + iLoopMac * 6 + 64 + 32 ] ;
				}
				pMac[6] = 1 ;
			}

			//Product Serial
			pMac = 0 ;
			pMac = &bootselinfodata.ubProductSerialNO[0] ;
			for ( iLoop = 0 ; iLoop < 64 ; iLoop ++ )
			{
				pMac[iLoop] = ubRecvBuf[ 2 + iLoop ] ;
			}

			//BSP Version
			pMac = 0 ;
			pMac = &bootselinfodata.ubBSPVersion[0] ;
			for ( iLoop = 0 ; iLoop < 32 ; iLoop ++ )
			{
				pMac[iLoop] = ubRecvBuf[ 2 + iLoop + 64 ] ;
			}

			//write data
			bootsel_write_setting_data( ) ;
		}
	}
}
#endif
