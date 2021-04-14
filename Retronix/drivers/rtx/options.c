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

enum __OPTIONS_FUNC__{
	DEF_OPTIONS_FUNC_PASSWORD      = 0x00000001 ,
	DEF_OPTIONS_FUNC_CHANG_PW      = 0x00000002 ,
	DEF_OPTIONS_FUNC_UD_EXTSD      = 0x00000004 ,
	DEF_OPTIONS_FUNC_UD_USB        = 0x00000008 ,
	DEF_OPTIONS_FUNC_MENU          = 0x00000010 ,
	DEF_OPTIONS_FUNC_CHG_STORAGE   = 0x00000020 ,
	DEF_OPTIONS_FUNC_SCANFILE_SELF = 0x00000040 ,
} ;

static unsigned char const ubOptionsDefaultMagicCode[16] = {
	0x10 , 0x01 , 0xC0 , 0x34 , 0x00 , 0x00 , 0x71 , 0x00 ,
	0x00 , 0xA0 , 0x00 , 0xdf , 0x00 , 0x17 , 0x00 , 0x00 
} ;

static unsigned char const ubOptionsDefaultPassword[8] = {
	'2' , '7' , '9' , '9' , '9' , '9' , '2' , '9'
} ;

typedef struct __OPTIONS_LVDS_PAR__ {
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
} OPTIONS_LVDS_PAR ;

typedef struct __OPTIONS_DATA__ {
	unsigned long    ulCheckCode ;                  //(  0 +   4) 檢查碼
	unsigned char    ubMagicCode[16] ;              //(  4 +  16) 金鑰
	unsigned char    ubMAC[4][8] ;                  //( 20 +  32) 第1~4組MAC，fec_mac=****
	unsigned char    ubProductName[128] ;           //( 52 + 128) 產品名稱
	unsigned char    ubProductSerialNO[64] ;        //(180 +  64) 產品序號
	unsigned char    ubBSPVersion[32] ;             //(244 +  32) BSP版本
	unsigned long    ulPasswordLen ;                //(276 +   4) 密碼長度
	unsigned char    ubPassword[32] ;               //(280 +  32) 密碼
	unsigned long    ulFunction ;                   //(312 +   4) 功能
	unsigned long    ulCmd ;                        //(316 +   4) 下次執行指令
	unsigned long    ulStatus ;                     //(320 +   4) 執行結果
	unsigned long    ulDataExistInfo ;              //(324 +   4)
	OPTIONS_LVDS_PAR sLVDSVal ;          	        //(328 +  64) lvds 參數,lvds_val=****
    unsigned long    ulMcuWatchDog ; 		        //(392 +   4) MCU watch dog time. 0 is disable.
    unsigned long    ulMcuWatchDogFlag ;            //(396 +   4) MCU watch dog flag
	unsigned long    ulDdrType ;                	//(400 +   4) DDR Size, 0=512M, 1=1G, 2=2G, ...
	unsigned char    ubRecv01[112] ;                //(404 + 112) 保留
	
	unsigned char    ubProductSerialNO_Vendor[64] ; //(512 +  64) 生產，產品序號
	unsigned char    ubMAC01_Vendor[8] ;            //(576 +   8) 生產，第1組MAC
	unsigned char    ubMAC02_Vendor[8] ;            //(584 +   8) 生產，第2組MAC
	unsigned char    ubMAC03_Vendor[8] ;            //(592 +   8) 生產，第3組MAC
	unsigned char    ubMAC04_Vendor[8] ;            //(600 +   8) 生產，第4組MAC
	unsigned char    ubRecv02[416] ;                //(608 + 416)
	                                                //(1024)
} OPTIONS_DATA ;

typedef struct __OPTIONSFUNCDATA__ {
	char *        sbName ;
	unsigned long ulMask ;
} OPTIONSFUNCDATA ;

static OPTIONSFUNCDATA const stcOptionsFuncData[] = {
	{ (char *)"password" , DEF_OPTIONS_FUNC_PASSWORD    } ,
	{ (char *)"change"   , DEF_OPTIONS_FUNC_CHANG_PW    } ,
	{ (char *)"extsd"    , DEF_OPTIONS_FUNC_UD_EXTSD    } ,
	{ (char *)"usb"      , DEF_OPTIONS_FUNC_UD_USB      } ,
	{ (char *)"menu"     , DEF_OPTIONS_FUNC_MENU        } ,
	{ (char *)"storage"  , DEF_OPTIONS_FUNC_CHG_STORAGE } ,
	{ (char *)"selfmagic", DEF_OPTIONS_FUNC_SCANFILE_SELF } ,
} ;

static OPTIONS_DATA stcOptionsData ;

static void vOptions_WriteSettingData( void )
{
	u32 blksize ;
	#if defined(CONFIG_RTX_PARTITION_0_DATA_OFFSET) && defined(CONFIG_RTX_PARTITION_0_DATA_SIZE)
		blksize = ulStorage_Write( STORAGE_TYPE_BOOT , (CONFIG_RTX_PARTITION_0_DATA_OFFSET) , (CONFIG_RTX_PARTITION_0_DATA_SIZE) , &stcOptionsData ) ;
		if ( blksize != (CONFIG_RTX_PARTITION_0_DATA_SIZE) )
		{
			printf("\n emmc write error.\n") ;
		}
	#endif
}

static void vOptions_SetDefaultValue( void )
{
	memset( (void *)&stcOptionsData , 0 , sizeof(OPTIONS_DATA) ) ;

	stcOptionsData.ulFunction = 0
		#ifdef CONFIG_RTX_BOOT_SYSTEM_OPTIONS_FUNC_PASSWORD
			| DEF_OPTIONS_FUNC_PASSWORD 
		#endif
		#ifdef CONFIG_RTX_BOOT_SYSTEM_OPTIONS_FUNC_CHANG_PW
			| DEF_OPTIONS_FUNC_CHANG_PW 
		#endif
		#ifdef CONFIG_RTX_BOOT_SYSTEM_OPTIONS_FUNC_UD_EXTSD
			| DEF_OPTIONS_FUNC_UD_EXTSD 
		#endif
		#ifdef CONFIG_RTX_BOOT_SYSTEM_OPTIONS_FUNC_UD_USB
			| DEF_OPTIONS_FUNC_UD_USB 
		#endif
		#ifdef CONFIG_RTX_BOOT_SYSTEM_OPTIONS_FUNC_MENU
			| DEF_OPTIONS_FUNC_MENU 
		#endif
		#ifdef CONFIG_RTX_BOOT_SYSTEM_OPTIONS_FUNC_CHG_STORAGE
			| DEF_OPTIONS_FUNC_CHG_STORAGE 
		#endif
		#ifdef CONFIG_RTX_BOOT_SYSTEM_OPTIONS_FUNC_SCANFILE_SELF
			| DEF_OPTIONS_FUNC_SCANFILE_SELF 
		#endif
		;

	#if defined(CONFIG_RTX_EFM32_WATCHDOG_TIME)
		stcOptionsData.ulMcuWatchDog     = ( unsigned long ) (CONFIG_RTX_EFM32_WATCHDOG_TIME & 0xFFFF);
		stcOptionsData.ulMcuWatchDogFlag = 1 ;
    #endif

	memcpy( (void *)&stcOptionsData.ubMagicCode , (void *)ubOptionsDefaultMagicCode , 16 ) ;
	memcpy( (void *)&stcOptionsData.ubPassword , (void *)ubOptionsDefaultPassword , 8 ) ;
	stcOptionsData.ulPasswordLen = 8 ;
	
	stcOptionsData.ulCheckCode = 0x5AA5AA55 ;
	stcOptionsData.ulDdrType = 0x00000000 ;		//DDR Type=0
}

void vOptions_Init( void )
{
	int    iOk = 0 ;

	#if defined(CONFIG_RTX_PARTITION_0_DATA_OFFSET) && defined(CONFIG_RTX_PARTITION_0_DATA_SIZE) && defined(CONFIG_RTX_LOADADDR)
		{
			u32    u32Size ;
			void * pBuf = (void *)CONFIG_RTX_LOADADDR ;
			
			u32Size = ulStorage_Read( STORAGE_TYPE_BOOT , (CONFIG_RTX_PARTITION_0_DATA_OFFSET) , (CONFIG_RTX_PARTITION_0_DATA_SIZE) , pBuf);

			if ( u32Size == (CONFIG_RTX_PARTITION_0_DATA_SIZE) )
			{
				memcpy( (void *)&stcOptionsData , (void *)pBuf , sizeof(OPTIONS_DATA) ) ;
				if ( stcOptionsData.ulCheckCode == 0x5AA5AA55 )
				{
					iOk = 1 ;
				}
			}
		}
	#endif
	if ( !iOk )
	{
		vOptions_SetDefaultValue();
		vOptions_WriteSettingData( ) ;
	}
}

int iOptions_PasswordEnable( void )
{
	if ( stcOptionsData.ulFunction & DEF_OPTIONS_FUNC_PASSWORD )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int iOptions_ChangePasswordEnable( void )
{
	if ( stcOptionsData.ulFunction & DEF_OPTIONS_FUNC_CHANG_PW )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int iOptions_ExtSDEnable( void )
{
	if ( stcOptionsData.ulFunction & DEF_OPTIONS_FUNC_UD_EXTSD )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int iOptions_UsbStorageEnable( void )
{
	if ( stcOptionsData.ulFunction & DEF_OPTIONS_FUNC_UD_USB )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int iOptions_MenuEnable( void )
{
	if ( stcOptionsData.ulFunction & DEF_OPTIONS_FUNC_MENU )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int iOptions_AutoDetectStorageEnable( void )
{
	if ( stcOptionsData.ulFunction & DEF_OPTIONS_FUNC_CHG_STORAGE )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;	
}

int iOptions_BootStorageScanMagicCodeEnable( void )
{
	if ( stcOptionsData.ulFunction & DEF_OPTIONS_FUNC_SCANFILE_SELF )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

int iOptions_CheckPassword( int iLen , char *pPasswd )
{
	if ( iLen == stcOptionsData.ulPasswordLen )
	{
		if( memcmp( pPasswd , stcOptionsData.ubPassword , stcOptionsData.ulPasswordLen ) == 0 ) 
		{
			return ( 1 ) ;
		}
	}
	return ( 0 ) ;
}

void vOptions_SetPassword( int iLen , char *pPasswd )
{
	if ( iLen )
	{
		if ( iLen < 32 )
		{
			stcOptionsData.ulPasswordLen = iLen ;
			memcpy( stcOptionsData.ubPassword , pPasswd , iLen ) ;
			vOptions_WriteSettingData( ) ;
		}
	}
}

unsigned char * pOptions_GetMacData( int iIndex )
{
	if ( iIndex < 4 )
	{
		return ( (unsigned char *)&stcOptionsData.ubMAC[iIndex][0] ) ;
	}
	return ( NULL ) ;
}

unsigned long * pOptions_GetLVDSData( void )
{
	return ( (unsigned long *)&stcOptionsData.sLVDSVal.ulchecksum ) ;
}

int iOptions_GetWatchDogTimes( void )
{
	if ( stcOptionsData.ulMcuWatchDogFlag )
	{
		return ( stcOptionsData.ulMcuWatchDog ) ;
	}
	return ( -1 ) ;
}

int iOptions_CheckMagicCode( void *pData )
{
	if( ! memcmp( (void *)pData , &stcOptionsData.ubMagicCode , 16 ) )
	{
		return ( 1 ) ;
	}
	return ( 0 ) ;
}

/* ================================================================== */
/* ------------------------------------------------------------------ */
static int iOptions_HexStringToBinary( int ch )
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

static int do_OptionsSetFunction(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int iIndex ;
	
	if (argc != 3)
		return CMD_RET_USAGE;
	
	for ( iIndex = 0 ; iIndex < sizeof(stcOptionsFuncData) / sizeof(OPTIONSFUNCDATA) ; iIndex ++ )
	{
		if ( strcmp( argv[1] , stcOptionsFuncData[iIndex].sbName ) == 0 )
		{
			if ( strcmp( argv[2] , "enable" ) == 0 )
			{
				stcOptionsData.ulFunction |= stcOptionsFuncData[iIndex].ulMask ;
			}
			else
			{
				stcOptionsData.ulFunction &= ~stcOptionsFuncData[iIndex].ulMask ;
			}
			vOptions_WriteSettingData( ) ;
			return CMD_RET_SUCCESS;
		}
	}
	
	return CMD_RET_USAGE;
}

static int do_OptionsSetMac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int iIndex ;
	unsigned char * pMac ;
	int iValue = 0;
	int iCh ;
	
	pMac = 0 ;
	
	if ( argc != 3 )
	{
		return CMD_RET_USAGE ;
	}
	iValue = argv[1][0] - '0' ;
	
	if ( iValue < 0 )
	{
		return CMD_RET_USAGE ;
	}
	
	if ( iValue >= 4 )
	{
		return CMD_RET_USAGE ;
	}
	
	pMac = &stcOptionsData.ubMAC[iValue][0] ;
		
	for ( iIndex = 0 ; iIndex <  6 ; iIndex ++ )
	{
		iValue = 0 ;
		iCh = iOptions_HexStringToBinary( argv[2][iIndex*2] ) ;
		if ( iCh == -1 )
		{
			return CMD_RET_USAGE ;
		}
		iValue = iValue + iCh * 16 ;
		iCh = iOptions_HexStringToBinary( argv[2][iIndex*2+1] ) ;
		if ( iCh == -1 )
		{
			return CMD_RET_USAGE ;
		}
		iValue = iValue + iCh ;
		pMac[iIndex] = iValue ;
	}
	
	pMac[6] = 1 ;
	
	vOptions_WriteSettingData( ) ;
	
	return CMD_RET_SUCCESS;
}

static int do_OptionsSetMenu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2)
		return CMD_RET_USAGE;
	
	if ( strcmp( argv[1] , "update" ) == 0 )
	{
		if ( iBootMenu_Select('u') )
			return CMD_RET_SUCCESS;
	}
	else if ( strcmp( argv[1] , "recovery" ) == 0 )
	{
		if ( iBootMenu_Select('r') )
			return CMD_RET_SUCCESS;
	}
	else if ( strcmp( argv[1] , "android_recovery" ) == 0 )
	{
		if ( iBootMenu_Select('a') )
			return CMD_RET_SUCCESS;
	}
	else if ( strcmp( argv[1] , "password_change" ) == 0 )
	{
		if ( iOptions_ChangePasswordEnable() )
		{
			vPassword_SetNewPassword( ) ;
		}
		return CMD_RET_SUCCESS;
	}
	
	return CMD_RET_USAGE;
}

static int do_OptionsSetLVDS(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int iIndex ;
	int iValue ;
	int iCount ;
	int iHadData ;
	OPTIONS_LVDS_PAR stcLVDS ;
	unsigned long* pLVDSValue ;
	
	if (argc != 3)
		return CMD_RET_USAGE;
	
	if ( strcmp( argv[1] , "parameter" ) == 0 )
	{
		iValue = 0 ;
		iCount = 0 ;
		iHadData = 0 ;
		memset(&stcLVDS, 0, sizeof(OPTIONS_LVDS_PAR)) ;
		pLVDSValue = &stcLVDS.ulrefresh ;
		
		for(iIndex = 0 ; iIndex < strlen(argv[2]) ; iIndex ++)
		{
			if ( argv[2][iIndex] >= '0' && argv[2][iIndex] <= '9' )
			{
				iValue = iValue * 10 + (argv[2][iIndex] - '0') ;
				iHadData = 1 ;
			}
			else if ( argv[2][iIndex] == ',' )
			{
				*pLVDSValue = iValue ;
				stcLVDS.ulchecksum += iValue ;
				iCount ++ ;
				
				if ( iCount >= 15 )
				{
					return CMD_RET_USAGE ;
				}
				
				pLVDSValue++ ;
				iValue   = 0 ;
				iHadData = 0 ;
			}
			else
			{
				return CMD_RET_USAGE ;
			}	
		}
		
		if ( iHadData )
		{
			if ( iCount >= 15 )
			{
				return CMD_RET_USAGE ;
			}
			*pLVDSValue = iValue ;
			stcLVDS.ulchecksum += iValue ;
		}
		
		memcpy( (void *)&stcOptionsData.sLVDSVal , (const void *)&stcLVDS , sizeof(OPTIONS_LVDS_PAR) ) ;
		
		vOptions_WriteSettingData( ) ;	
		
		return CMD_RET_SUCCESS;
	}
	
	return CMD_RET_USAGE;
}

static int do_OptionsSetWatchdog(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int iTime ;
	
	if (argc != 3)
		return CMD_RET_USAGE;
	
	if ( strcmp( argv[1] , "time" ) == 0 )
	{
		if( strlen(argv[2]) < 6 )
        {
			iTime = simple_strtoul( argv[2] , NULL , 10 ) ;
		}
		else
		{
			return CMD_RET_USAGE ;
		}
		
		if( iTime <= 0xFFFF )
		{
			stcOptionsData.ulMcuWatchDog = iTime ;
			stcOptionsData.ulMcuWatchDogFlag = 1 ;
			vOptions_WriteSettingData( ) ;	
			return CMD_RET_SUCCESS;
		}
	}
	
	return CMD_RET_USAGE;
}

static int do_OptionsSetDdrType(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int iType ;

	if (argc != 3)
		return CMD_RET_USAGE;

	if ( strcmp( argv[1] , "type" ) == 0 )
	{
		if( strlen(argv[2]) < 2 )
        	{
			iType = simple_strtoul( argv[2] , NULL , 10 ) ;
		}
		else
		{
			return CMD_RET_USAGE ;
		}
		if( iType <= 0xFF )
		{
			stcOptionsData.ulDdrType = iType ;
			vOptions_WriteSettingData( ) ;
			return CMD_RET_SUCCESS;
		}
	}
	return CMD_RET_USAGE;
}

static cmd_tbl_t stcOptionsSubCmd[] = {
	U_BOOT_CMD_MKENT(function , 3, 0, do_OptionsSetFunction , "", ""),
	U_BOOT_CMD_MKENT(mac      , 3, 0, do_OptionsSetMac      , "", ""),
	U_BOOT_CMD_MKENT(menu     , 2, 0, do_OptionsSetMenu     , "", ""),
	U_BOOT_CMD_MKENT(lvds     , 3, 0, do_OptionsSetLVDS     , "", ""),
	U_BOOT_CMD_MKENT(watchdog , 3, 0, do_OptionsSetWatchdog , "", ""),
	U_BOOT_CMD_MKENT(ddrtype  , 3, 0, do_OptionsSetDdrType  , "", ""),
};

static int do_OptionsSetOps(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *cp;

	cp = find_cmd_tbl(argv[1], stcOptionsSubCmd, ARRAY_SIZE(stcOptionsSubCmd));

	/* Drop the bootsel_set command */
	argc--;
	argv++;

	if (cp == NULL || argc > cp->maxargs)
		return CMD_RET_USAGE;
	if (flag == CMD_FLAG_REPEAT && !cp->repeatable)
		return CMD_RET_SUCCESS;

	return cp->cmd(cmdtp, flag, argc, argv);
}

U_BOOT_CMD(
	bootsel_set, 4, 0,	do_OptionsSetOps,
	"set the Retronix data",
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
	"** ddrtype class **\n"
	"    ddrtype type 0(0=default, 1=type1, 2=type2, ...)\n"
);

#ifdef CONFIG_RTX_BOOT_CMD_RESET_SETTING
static int do_OptionsResetData(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	vOptions_SetDefaultValue( ) ;
	vOptions_WriteSettingData( ) ;
	
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	setting_reset, 1, 0, do_OptionsResetData,
	"reset the Retronix data to default",
	""
);
#endif

#ifdef CONFIG_RTX_BOOT_SYSTEM_SHOW_SETTING_INFO
static int do_OptionsDumpData(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int loop , loop1 ;
	unsigned long * ulval ;
	
	printf( "ulCheckCode:%08X\n" , (unsigned int)stcOptionsData.ulCheckCode ) ;
	printf( "ubMagicCode:" ) ;
	for ( loop = 0 ; loop < 16 ; loop ++ )
	{
		if ( loop == 15 )
		{
			printf( "0x%02X" , stcOptionsData.ubMagicCode[loop] ) ;
		}
		else
		{
			printf( "0x%02X," , stcOptionsData.ubMagicCode[loop] ) ;
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
				printf( "%02X" , stcOptionsData.ubMAC[loop1][loop] ) ;
			}
			else
			{
				printf( "%02X:" , stcOptionsData.ubMAC[loop1][loop] ) ;
			}
		}
		printf( "\n" ) ;
	}
	
	printf( "Product Name:" ) ;
	for ( loop = 0 ; loop < 128 ; loop ++ )
	{
		if ( stcOptionsData.ubProductName[loop] == 0x00 )
		{
			break ;
		}
		if ( stcOptionsData.ubProductName[loop] >= 32 && stcOptionsData.ubProductName[loop] < 128 )
		{
			printf( "%c" , stcOptionsData.ubProductName[loop] ) ;
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
		if ( stcOptionsData.ubProductSerialNO[loop] == 0x00 )
		{
			break ;
		}
		if ( stcOptionsData.ubProductSerialNO[loop] >= 32 && stcOptionsData.ubProductSerialNO[loop] < 128 )
		{
			printf( "%c" , stcOptionsData.ubProductSerialNO[loop] ) ;
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
		if ( stcOptionsData.ubBSPVersion[loop] == 0x00 )
		{
			break ;
		}
		if ( stcOptionsData.ubBSPVersion[loop] >= 32 && stcOptionsData.ubBSPVersion[loop] < 128 )
		{
			printf( "%c" , stcOptionsData.ubBSPVersion[loop] ) ;
		}
		else
		{
			printf( "%c" , '?' ) ;
		}
	}
	printf( "\n" ) ;

	for ( loop = 0 ; loop < sizeof(stcOptionsFuncData) / sizeof(OPTIONSFUNCDATA) ; loop ++ )
	{
		if ( stcOptionsData.ulFunction & stcOptionsFuncData[loop].ulMask )
		{
			printf( "function %s : enable \n", stcOptionsFuncData[loop].sbName ) ;
		}
		else
		{
			printf( "function %s : disable \n", stcOptionsFuncData[loop].sbName ) ;
		}
	}
	
	printf( "lvds parameter:" ) ;
	ulval = &stcOptionsData.sLVDSVal.ulchecksum ;
	for ( loop = 0 ; loop < sizeof(stcOptionsData.sLVDSVal)/sizeof(unsigned long) ; loop ++ )
	{
		if ( loop == (sizeof(stcOptionsData.sLVDSVal)/sizeof(unsigned long))-1 )
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
    printf( "%d" , (unsigned int)(stcOptionsData.ulMcuWatchDog & 0xFFFF) ) ;
	printf( "\n" ) ;

	for ( loop1 = 0 ; loop1 < 4 ; loop1 ++ )
	{
		printf( "ubMAC0%d_Vendor:" , loop1 ) ;
		for ( loop = 0 ; loop < 8 ; loop ++ )
		{
			if ( loop == 7 )
			{
				printf( "%02X" , stcOptionsData.ubMAC01_Vendor[loop+loop1*8] ) ;
			}
			else
			{
				printf( "%02X:" , stcOptionsData.ubMAC01_Vendor[loop+loop1*8] ) ;
			}
		}
		printf( "\n" ) ;
	}

	printf( "Product Serial_Vendor:" ) ;
	for ( loop = 0 ; loop < 64 ; loop ++ )
	{
		if ( stcOptionsData.ubProductSerialNO_Vendor[loop] == 0x00 )
		{
			break ;
		}
		if ( stcOptionsData.ubProductSerialNO_Vendor[loop] >= 32 && stcOptionsData.ubProductSerialNO_Vendor[loop] < 128 )
		{
			printf( "%c" , stcOptionsData.ubProductSerialNO_Vendor[loop] ) ;
		}
		else
		{
			printf( "%c" , '?' ) ;
		}
	}
	printf( "\n" ) ;

	printf( "ulDdrType:%08lX\n" , stcOptionsData.ulDdrType ) ;
	printf( "\n" ) ;

	return CMD_RET_SUCCESS;
}

/* get setting information*/
U_BOOT_CMD(
	setting_info, 1, 0,	do_OptionsDumpData,
	"show the Retronix data.",
	""
);
#endif
