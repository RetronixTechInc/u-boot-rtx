#ifndef _BOOT_SYSTEM_H_
#define _BOOT_SYSTEM_H_

/* boot main */
void vBootMain_Init( void ) ;
void vBootMain_SetWatchdog( void ) ;

/* boot menu */
int iBootMenu_Select( int iSelect ) ;

/* boot env */
void vBootEnv_SetFromOptions( void ) ;

/* boot update */
int iBootUpdate_LoadBackupSystem( void ) ;
int iBootUpdate_Check( void );

/* options */
void vOptions_Init( void ) ;
int  iOptions_PasswordEnable( void ) ;
int  iOptions_ChangePasswordEnable( void ) ;
int  iOptions_ExtSDEnable( void ) ;
int  iOptions_UsbStorageEnable( void ) ;
int  iOptions_MenuEnable( void ) ;
int  iOptions_AutoDetectStorageEnable( void ) ;
int  iOptions_BootStorageScanMagicCodeEnable( void ) ;
int  iOptions_CheckPassword( int iLen , char *pPasswd ) ;
void vOptions_SetPassword( int iLen , char *pPasswd ) ;
unsigned char * pOptions_GetMacData( int iIndex ) ;
unsigned long * pOptions_GetLVDSData( void ) ;
int iOptions_GetWatchDogTimes( void ) ;
int iOptions_CheckMagicCode( void *pData ) ;

/* password */
void vPassword_Process( void ) ;
void vPassword_SetNewPassword( void ) ;

/* storage */
enum STORAGE_TYPE {
	STORAGE_TYPE_BOOT   = 0,
	STORAGE_TYPE_EXTSD0 ,
	STORAGE_TYPE_EXTSD1 ,
	STORAGE_TYPE_EXTSD2 ,
	STORAGE_TYPE_EXTSD3 ,
	STORAGE_TYPE_USB    ,
	STORAGE_TYPE_COUNT			/* Number of storage types */
};

void vStorage_Init( void ) ;
int iStorage_IsExist( enum STORAGE_TYPE iType ) ;
int iStorage_FileIsExist( enum STORAGE_TYPE iType , char *pFileName ) ;
int iStorage_LoadFile( enum STORAGE_TYPE iType , char *pFileName , void *pBuffer ) ;
unsigned long ulStorage_Read( enum STORAGE_TYPE iType , lbaint_t ulStart , lbaint_t ulBlkCnt, void *pBuffer ) ;
unsigned long ulStorage_Write( enum STORAGE_TYPE iType , lbaint_t ulStart , lbaint_t ulBlkCnt, void *pBuffer ) ;

#endif	/* _BOOT_SYSTEM_H_ */
