#ifndef _DATALIST_H
#define _DATALIST_H

#define REV_SUCCESS 1
#define REV_FAIL    0

#define CREATE_FILE

int Create_Datalist();
int Save_Datalist();
int Destroy_Datalist();
void Show_Datalist();

int Init_Datalist( char* path );
#ifdef CREATE_FILE
void Create_Init_File( char* inpath, char* outpath );
#endif

int Add_Envitem(char* variable, char* data);
int Delete_Envitem(char* variable);
int Modify_Envitem(char* variable, char* data);
int Set_Envitem(char* variable, char* data);

#endif
