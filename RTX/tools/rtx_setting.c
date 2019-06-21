#include "common.h"

/* options args */
static const char *short_options = "i:c:w";

static struct option long_options[] = {
  {"info"	        , required_argument, 0, 'i'                      },
  {"clean"        	, no_argument, 		 0, 'c'                      },
  {"write"          , required_argument, 0, 'w'                      },
  {0                , no_argument,       0, 0                        }
};

static void show_usage(){
	printf("Options:\n");
    printf("        -info     	show setting info\n");
	printf("        -clean     clean all variable\n");
	printf("        -write mac 0 000000000000 [mp] \n");
	printf("        -write mac 1 000000000000 [mp] \n");
	printf("        -write mac 2 000000000000 [mp] \n");
	printf("        -write mac 3 000000000000 [mp] \n");
	printf("        -write lvds parameter fps,hv,vv,clk,lm,rm,um,dm,hs,vs,sy,mode \n");
	printf("        -write name RTX-R064Roymark-Digitalsinige(max128) \n");
	printf("        -write serialno RTX001258746(max64) [mp] \n");
	printf("        -write bspver rtx123456789(max32) \n");
	printf("        -write function password <enable/disable> \n");
	printf("        -write function change <enable/disable> \n");
	printf("        -write function extsd <enable/disable> \n");
	printf("        -write function usb <enable/disable> \n");
	printf("        -write function menu <enable/disable> \n");
	printf("        -write function storage <enable/disable> \n");
	printf("        -write function selfmagic <enable/disable> \n");
	printf("        -write function usbstart <enable/disable> \n");
	printf("        -write watchdog time 0(max 65535 seconds.) \n");
}

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
	unsigned long ulDataExistInfo ;
	lvdspar sLVDSVal ;
    unsigned long ulMcuWatchDog ; //MCU watch dog time. 0 is disable.
	unsigned char ubRecv01[116] ;
	unsigned char ubProductSerialNO_Vendor[64] ;
	unsigned char ubMAC01_Vendor[8] ;
	unsigned char ubMAC02_Vendor[8] ;
	unsigned char ubMAC03_Vendor[8] ;
	unsigned char ubMAC04_Vendor[8] ;
	unsigned char ubRecv02[416] ;
} bootselinfo ;

enum __BOOTSEL_FUNC__{
	DEF_BOOTSEL_FUNC_PASSWORD    = 0x00000001 ,
	DEF_BOOTSEL_FUNC_CHANG_PW    = 0x00000002 ,
	DEF_BOOTSEL_FUNC_UD_EXTSD    = 0x00000004 ,
	DEF_BOOTSEL_FUNC_UD_USB      = 0x00000008 ,
	DEF_BOOTSEL_FUNC_MENU        = 0x00000010 ,
	DEF_BOOTSEL_FUNC_CHG_STORAGE = 0x00000020 ,
	DEF_BOOTSEL_FUNC_SCANFILE_SELF = 0x00000040 ,
	DEF_BOOTSEL_FUNC_USBSTART    = 0x00000080 ,
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
	{ (char *)"usbstart" , DEF_BOOTSEL_FUNC_USBSTART    } ,
} ;


#define BLOCK_SKIP (768 * 1024)
#define BLOCK_SKIP1 (3 * 512)

static	const char path_android_utf[] =  "/dev/block/mmcblk0";
static	const char path_utf[] =  "/dev/mmcblk0";

static int g_mmcblk0_fd = 0;
static bootselinfo bootselinfodata ;

#define CMD_RET_SUCCESS 1
#define CMD_RET_USAGE    0

static int Init_Ubootlist(){
	int read_size = 0;
	int val = CMD_RET_SUCCESS;
	
	memset( (void *)&bootselinfodata , 0 , sizeof(bootselinfo) ) ;

	g_mmcblk0_fd = open( path_utf, O_RDWR );

	if( g_mmcblk0_fd <= 0 ){
		//printf( "[Create List] Error opening file: %s\n", path_utf );
		g_mmcblk0_fd = open( path_android_utf, O_RDWR );
		if( g_mmcblk0_fd <= 0 ){
			printf( "[Create List] Error opening file: %s and %s\n", path_utf ,path_android_utf );
			return CMD_RET_USAGE;
		}
	}
	lseek( g_mmcblk0_fd, BLOCK_SKIP, SEEK_SET );
	read_size = read( g_mmcblk0_fd, &bootselinfodata, sizeof(bootselinfo) );

	if( read_size != sizeof(bootselinfo) ){
		printf( "read_size = %d\n", read_size );
		val = CMD_RET_USAGE;
	}

	close( g_mmcblk0_fd );
	g_mmcblk0_fd = 0;

	return val;
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

static int bootsel_write_setting_data( void )
{
	int write_size = 0;
	int val = CMD_RET_SUCCESS;

	g_mmcblk0_fd = open( path_utf, O_RDWR );

	if( g_mmcblk0_fd <= 0 ){
		g_mmcblk0_fd = open( path_android_utf, O_RDWR );
		if( g_mmcblk0_fd <= 0 ){
			printf( "[Create List] Error opening file: %s and %s\n", path_utf ,path_android_utf );
			return CMD_RET_USAGE;
		}
	}

	lseek( g_mmcblk0_fd, BLOCK_SKIP, SEEK_SET );
	write_size = write( g_mmcblk0_fd, &bootselinfodata, sizeof(bootselinfo) );

	if( write_size != sizeof(bootselinfo) ){
		printf( "write_size = %d\n", write_size );
		val = CMD_RET_USAGE;
	}

	close( g_mmcblk0_fd );
	g_mmcblk0_fd = 0;
	
    sync();
    
	return val;
}

static int do_show_setting_info( )
{
	int loop = 0 ;
	int loop1 = 0 ;
	unsigned long * ulval ;
    unsigned char  *pMac = NULL ;
	
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
        switch( loop1 )
        {
            case 0 : pMac = &bootselinfodata.ubMAC01[0] ; break ;
            case 1 : pMac = &bootselinfodata.ubMAC02[0] ; break ;
            case 2 : pMac = &bootselinfodata.ubMAC03[0] ; break ;
            case 3 : pMac = &bootselinfodata.ubMAC04[0] ; break ;
			default :
				pMac = NULL ;
        }
		printf( "ubMAC0%d:" , loop1 ) ;
		for ( loop = 0 ; loop < 8 ; loop ++ )
		{
            if( pMac == NULL )
            {
                if ( loop == 7 )
                {
                    printf( "00" ) ;
                }
                else
                {
                    printf( "00:" ) ;
                }            
            }
            else
            {
                if ( loop == 7 )
                {
                    printf( "%02X" , pMac[loop] ) ;
                }
                else
                {
                    printf( "%02X:" , pMac[loop] ) ;
                }
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
			printf( "function %s:enable \n", bootselfuncarray[loop].name ) ;
		}
		else
		{
			printf( "function %s:disable \n", bootselfuncarray[loop].name ) ;
		}
	}
	
	printf( "lvds parameter:" ) ;
	ulval = &bootselinfodata.sLVDSVal.ulchecksum ;
	for ( loop = 0 ; loop < sizeof(bootselinfodata.sLVDSVal)/sizeof(unsigned long) ; loop ++ )
	{
		if ( loop == (sizeof(bootselinfodata.sLVDSVal)/sizeof(unsigned long))-1 )
		{
			printf( "%d" , (unsigned int)*ulval++ ) ;
		}
		else
		{
			printf( "%d," , (unsigned int)*ulval++ ) ;
		}
	}
	printf( "\n" ) ;

	printf( "McuWatchDog seconds:" ) ;
    printf( "%d" , (unsigned int)(bootselinfodata.ulMcuWatchDog & 0xFFFF) ) ;
	printf( "\n" ) ;

	for ( loop1 = 0 ; loop1 < 4 ; loop1 ++ )
	{
        switch( loop1 )
        {
            case 0 : pMac = &bootselinfodata.ubMAC01_Vendor[0] ; break ;
            case 1 : pMac = &bootselinfodata.ubMAC02_Vendor[0] ; break ;
            case 2 : pMac = &bootselinfodata.ubMAC03_Vendor[0] ; break ;
            case 3 : pMac = &bootselinfodata.ubMAC04_Vendor[0] ; break ;
			default :
				pMac = NULL ;
        }
		printf( "ubMAC0%d_Vendor:" , loop1 ) ;
		for ( loop = 0 ; loop < 8 ; loop ++ )
		{
            if( pMac == NULL )
            {
                if ( loop == 7 )
                {
                    printf( "00" ) ;
                }
                else
                {
                    printf( "00:" ) ;
                }            
            }
            else
            {
                if ( loop == 7 )
                {
                    printf( "%02X" , pMac[loop] ) ;
                }
                else
                {
                    printf( "%02X:" , pMac[loop] ) ;
                }
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

static int do_reset_setting()
{
	memset( (void *)&bootselinfodata , 0 , sizeof(bootselinfo) ) ;
	bootsel_write_setting_data( ) ;
	return CMD_RET_SUCCESS;
}

static int do_set_bootsel_setting(int argc, char * const arg[])
{
	int loop ;
	int value = 0 ;
    unsigned long ulval = 0 ;
	int ch ;
	unsigned char * pMac ;
	unsigned char * pMac_ven ;
	char** argv = arg ;
	lvdspar lvds_temp ;
	unsigned long* ullvds ;
	unsigned long* ullvds_temp ;

	if ( argc < 4 )
	{
		return CMD_RET_USAGE ;
	}

	argv++;
	if ( strcmp( argv[1] , "function" ) == 0 )
	{
		if ( argc < 5 )
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
				return bootsel_write_setting_data( ) ;
			}
		}
	}
	else if ( strcmp( argv[1] , "mac" ) == 0 )
	{
		pMac = 0 ;
		pMac_ven = 0 ;
		if ( argc < 5 )
		{
			return CMD_RET_USAGE ;
		}
		
		switch( argv[2][0] )
		{
			case '0' :
				pMac = &bootselinfodata.ubMAC01[0] ; 
				pMac_ven = &bootselinfodata.ubMAC01_Vendor[0] ; 
				break ;
			case '1' :
				pMac = &bootselinfodata.ubMAC02[0] ;
				pMac_ven = &bootselinfodata.ubMAC02_Vendor[0] ; 
				break ;
			case '2' :
				pMac = &bootselinfodata.ubMAC03[0] ;
				pMac_ven = &bootselinfodata.ubMAC03_Vendor[0] ; 
				break ;
			case '3' :
				pMac = &bootselinfodata.ubMAC04[0] ;
				pMac_ven = &bootselinfodata.ubMAC04_Vendor[0] ; 
				break ;
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
			if ( argv[4] != NULL && strcmp( argv[4] , "mp" ) == 0 )
			{
				pMac_ven[loop] = value ;
				pMac_ven[6] = 1 ;
			}
		}
		pMac[6] = 1 ;
		return bootsel_write_setting_data( ) ;
	}
	else if ( strcmp( argv[1] , "name" ) == 0 )
	{
		if ( argc < 4 )
		{
			return CMD_RET_USAGE ;
		}
		
		if( strlen( argv[2] ) < 128)
		{
			strcpy(&bootselinfodata.ubProductName[0], argv[2]) ;
			return bootsel_write_setting_data( ) ;
		}
	}
	else if ( strcmp( argv[1] , "serialno" ) == 0 )
	{
		if ( argc < 4 )
		{
			return CMD_RET_USAGE ;
		}
		
		if( strlen( argv[2] ) < 64)
		{
			strcpy(&bootselinfodata.ubProductSerialNO[0], argv[2]) ;
			if ( argv[3] != NULL && strcmp( argv[3] , "mp" ) == 0 )
			{
				strcpy(&bootselinfodata.ubProductSerialNO_Vendor[0], argv[2]) ;
			}
			return bootsel_write_setting_data( ) ;
		}
	}
	else if ( strcmp( argv[1] , "bspver" ) == 0 )
	{
		if ( argc < 4 )
		{
			return CMD_RET_USAGE ;
		}
		
		if( strlen( argv[2] ) < 32)
		{
			strcpy(&bootselinfodata.ubBSPVersion[0], argv[2]) ;
			return bootsel_write_setting_data( ) ;
		}
	}
	else if ( strcmp( argv[1] , "lvds" ) == 0 )
	{
		if ( argc < 5 )
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
		if ( argc < 5 )
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

int main(int argc, char **argv)
{
    int c ='?';
    int option_index = 0;
	int error = 0;

	error = Init_Ubootlist();

	if(error == CMD_RET_USAGE){
		goto error_message;
	}
	
	while( (c = getopt_long(argc, argv, short_options,long_options, &option_index)) != EOF ){
		switch( c ){
			case 'i':
				error = do_show_setting_info( );
				break;
			case 'c':
				error = do_reset_setting( );
				break;
			case 'w':
				error = do_set_bootsel_setting(argc , argv);
				break;
			default:
				error = -1;
				break;
		}

		if( error <= 0 ){
			show_usage();
			break;
		}else if( error == CMD_RET_SUCCESS ){
			break;
		}
	}
	
error_message:
	return 0;
}

