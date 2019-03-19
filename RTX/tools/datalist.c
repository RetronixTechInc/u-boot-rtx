#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "datalist.h"

#define BLOCK_SKIP (12288 * 1024)
#define BLOCK_SIZE (8 * 1024)
#define CRC_SIZE 4
#define ENV_SIZE ( BLOCK_SIZE - CRC_SIZE )

static	const char path_android_utf[] =  "/dev/block/mmcblk0";
static	const char path_utf[] =  "/dev/mmcblk0";

static char data_buf[BLOCK_SIZE];
static int g_mmcblk0_fd = 0;

static int total_data_size = 0;

static int check_crc = 0;
static char crc_buf[CRC_SIZE + 1];

#ifdef CREATE_FILE
static char create_init_file = 0;
#endif

typedef struct env_item{
	char* variable;
	char* value;
	int variable_length;
	int value_length;
	struct env_item* prev;
	struct env_item* next;
}env_item_t;

typedef struct env_list{
	env_item_t* first;
	env_item_t* last;
}env_list_t;

static env_list_t EnvList;

env_item_t* Search_Envitem(char* variable);

int Create_Envitem(char* variable, char* data);
int Destroy_Envitem(env_item_t* item);
int Edit_Envitem( env_item_t* item, char* variable, char* data);

int Create_Datalist(){
	int read_size = 0;
	char* item_variable = NULL;
	char* item_data = NULL;
	int val = REV_SUCCESS;

	if( EnvList.first != NULL || g_mmcblk0_fd > 0 ) Destroy_Datalist();

	memset( data_buf, 0, BLOCK_SIZE );

	g_mmcblk0_fd = open( path_utf, O_RDWR );

	if( g_mmcblk0_fd <= 0 ){
		printf( "[Create List] Error opening file: %s\n", path_utf );
		g_mmcblk0_fd = open( path_android_utf, O_RDWR );
		if( g_mmcblk0_fd <= 0 ){
			printf( "[Create List] Error opening file: %s\n", path_android_utf );
			return REV_FAIL;
		}
	}

	lseek( g_mmcblk0_fd, (BLOCK_SKIP + CRC_SIZE), SEEK_SET );

	read_size = read( g_mmcblk0_fd, &data_buf[0], ENV_SIZE );

	if( read_size != ENV_SIZE ){
		printf( "read_size = %d\n", read_size );
		Destroy_Datalist();
		return REV_FAIL;
	}

	item_variable = strtok( &data_buf[0], "=" );
	if( item_variable != NULL ){
		item_data = strtok( NULL, "\0" );

		if( item_data != NULL){
			val = Create_Envitem( item_variable, item_data );

			if( val == REV_FAIL ){
				printf("[Create List] create list fail\n");
				return REV_FAIL;
			}
		}
	}

	while( item_variable != NULL && item_data != NULL && total_data_size != ENV_SIZE ){
                item_variable = strtok( &data_buf[total_data_size], "=" );
                if( item_variable != NULL ){
                        item_data = strtok( NULL, "\0" );

                        if( item_data != NULL){
                                val = Create_Envitem( item_variable, item_data );

				if( val == REV_FAIL ){
					printf("[Create List] create list fail\n");
					return REV_FAIL;
				}
                        }
                }
	}

	return REV_SUCCESS;
}

int Save_Datalist(){
	int temp = CRC_SIZE;
	env_item_t* temp_item = EnvList.first;
	int crc_value = 0;
	int i = 0;
	int read_size;

#ifdef CREATE_FILE
	if( create_init_file == 1 ){
		printf("[Save List] Create init file\n");
		return REV_SUCCESS;
	}
#endif

	if( EnvList.first == NULL ){
		printf("[Save List] EnvList is NULL\n");
		return REV_SUCCESS;
	}

	if( g_mmcblk0_fd <= 0 ){
		g_mmcblk0_fd = open( path_android_utf, O_RDWR );
		if( g_mmcblk0_fd <= 0 ){
			printf( "[Create List] Error opening file: %s\n", path_android_utf );
			return REV_FAIL;
		}
	}

	memset( data_buf, 0, BLOCK_SIZE );

	while( temp_item != NULL ){
		memcpy( &data_buf[temp], temp_item->variable, temp_item->variable_length );
		temp += (temp_item->variable_length - 1);

		data_buf[temp] = '=';
		temp++;

		memcpy( &data_buf[temp], temp_item->value, temp_item->value_length );
		temp += temp_item->value_length;

		temp_item = temp_item->next;
	}

	crc_value = crc32( 0, &data_buf[CRC_SIZE], ENV_SIZE );

	for( i = 0 ; i < CRC_SIZE ; i++ )
	{
		data_buf[i] = (char) ( (crc_value >> (8 * i)) & 0x000000FF );
	}

	if( check_crc ){
		if( strncmp( data_buf, crc_buf, CRC_SIZE ) != 0 ){
			return REV_FAIL;
		}
	}

	lseek( g_mmcblk0_fd, BLOCK_SKIP, SEEK_SET );
	read_size = read( g_mmcblk0_fd, &crc_buf[0], CRC_SIZE );

	if( strncmp( data_buf, crc_buf, CRC_SIZE ) == 0 ){
		printf("[Save List] list is same, not save!\n");
		return REV_SUCCESS;
	}

	lseek( g_mmcblk0_fd, BLOCK_SKIP, SEEK_SET );
	write( g_mmcblk0_fd, &data_buf[0], BLOCK_SIZE );

	return REV_SUCCESS;
}

int Destroy_Datalist(){

	if( g_mmcblk0_fd > 0 ){
	    close( g_mmcblk0_fd );
	    g_mmcblk0_fd = 0;
	}

	while( EnvList.first != NULL ){
		Destroy_Envitem( EnvList.first );
	}

	return REV_SUCCESS;
}

void Show_Datalist(){
	env_item_t* temp_item = EnvList.first;

	printf("[Env Data List] : \n");

	while( temp_item != NULL ){
		printf( "    %s = %s\n", temp_item->variable, temp_item->value );

		temp_item = temp_item->next;
	}
}

int Init_Datalist( char* path ){
	FILE* fin = NULL;
	char buffer[256];
	int i;

	if( path == NULL ){
		printf("[Init] please set path!\n");
		return REV_FAIL;
	}

	check_crc = 1;

        while( EnvList.first != NULL ){
                Destroy_Envitem( EnvList.first );
        }

	memset( crc_buf, 0, (CRC_SIZE + 1) );

        fin = fopen( path, "rb" );

	if( fin == NULL ){
		printf("[Init] can not open %s to read!\n", path);
		return REV_FAIL;
	}

	if( feof(fin) ){
		printf("[Init] initial file is null!\n");
		return REV_FAIL;
	}else{
		for( i = 0; i < CRC_SIZE && !feof(fin); i++ ){
			crc_buf[i] = fgetc( fin );
		}
		if( !feof(fin) )
			buffer[0] = fgetc( fin );

		while( 1 ){
			if( feof(fin) ){
				break;
			}

			memset( buffer, 0, 256 );
			if( fgets( buffer, 255, fin ) != NULL ){
				char* variable = NULL;
				char* data = NULL;
				variable = strtok( buffer, "=" );
				data = strtok( NULL, "\r\n" );
				Set_Envitem( variable, data );
			}else{
				break;
			}
		}
        }

	fclose( fin );

	return REV_SUCCESS;
}

#ifdef CREATE_FILE
void Create_Init_File( char* inpath, char* outpath ){
        FILE* fin = NULL;
	FILE* fout = NULL;
        char buffer[256];
	int crc_value = 0;
	env_item_t* temp_item;
	int i;
	int temp = CRC_SIZE;

        if( inpath == NULL || outpath == NULL ){
                printf("[Create File] please set in out path!\n");
                return;
        }

        create_init_file = 1;

        while( EnvList.first != NULL ){
                Destroy_Envitem( EnvList.first );
        }

        fin = fopen( inpath, "rb" );

        if( fin == NULL ){
                printf("[Create File] can not open %s to read!\n", inpath);
                return;
        }

	fout = fopen( outpath, "wb" );

        if( fout == NULL ){
                printf("[Create File] can not open %s to write!\n", outpath);
		fclose( fin );
                return;
        }

        if( feof(fin) ){
                printf("[Create File] in file is null!\n");
        }else{
                while( 1 ){
                        if( feof(fin) ){
                                break;
                        }

                        memset( buffer, 0, 256 );
                        if( fgets( buffer, 255, fin ) != NULL ){
                                char* variable = NULL;
                                char* data = NULL;
                                variable = strtok( buffer, "=" );
                                data = strtok( NULL, "\r\n" );
                                Set_Envitem( variable, data );
                        }else{
                                break;
                        }
                }
        }

	temp_item = EnvList.first;

        memset( data_buf, 0, BLOCK_SIZE );

        while( temp_item != NULL ){
                memcpy( &data_buf[temp], temp_item->variable, temp_item->variable_length );
                temp += (temp_item->variable_length - 1);

                data_buf[temp] = '=';
                temp++;

                memcpy( &data_buf[temp], temp_item->value, temp_item->value_length );
                temp += temp_item->value_length;

                temp_item = temp_item->next;
        }

        crc_value = crc32( 0, &data_buf[CRC_SIZE], ENV_SIZE );

        for( i = 0 ; i < CRC_SIZE ; i++ )
        {
                data_buf[i] = (char) ( (crc_value >> (8 * i)) & 0x000000FF );
		fputc( data_buf[i], fout );
        }
	fputc( '\n', fout );

	temp_item = EnvList.first;

        while( temp_item != NULL ){
		fputs( temp_item->variable, fout );
		fputc( '=', fout );
                fputs( temp_item->value, fout );
                fputc( '\n', fout );

                temp_item = temp_item->next;
        }

        fclose( fin );
	fclose( fout );
}
#endif

env_item_t* Search_Envitem( char* variable ){
	env_item_t* temp_item = EnvList.first;

	if( variable == NULL ){
		printf("[Search] variable not set\n");
		return NULL;
	}

	while( temp_item != NULL ){
		if( strcmp( variable, temp_item->variable ) == 0 ){
			break;
		}else{
			temp_item = temp_item->next;
		}
	}

	return temp_item;
}

int Create_Envitem( char* variable, char* data ){
	env_item_t* temp_item = (env_item_t*) calloc( 1, sizeof(env_item_t) );

	if( variable == NULL ){
		printf("[Create] variable not set\n");
		return REV_FAIL;
	}else if( data == NULL ){
		printf("[Create] no data\n");
		return REV_FAIL;
	}

	temp_item->variable_length = strlen(variable) + 1;
	temp_item->value_length = strlen(data) + 1;

	if( (total_data_size + temp_item->variable_length + temp_item->value_length) > ENV_SIZE ){
		free( temp_item );
		return REV_FAIL;
	}

	temp_item->variable = (char*) calloc( temp_item->variable_length, sizeof(char) );
	strcpy( temp_item->variable, variable );
	temp_item->value = (char*) calloc( temp_item->value_length, sizeof(char) );
	strcpy( temp_item->value, data );

	if( EnvList.first == NULL ){
		EnvList.first = temp_item;
		EnvList.last = temp_item;
		temp_item->prev = NULL;
		temp_item->next = NULL;
	}else{
		EnvList.last->next = temp_item;
		temp_item->prev = EnvList.last;
		temp_item->next = NULL;
		EnvList.last = temp_item;
	}

	total_data_size += temp_item->variable_length;
	total_data_size += temp_item->value_length;

	return REV_SUCCESS;
}

int Destroy_Envitem( env_item_t* item ){
	if( item == NULL){
		printf("[Destroy] not set item\n");
		return REV_FAIL;
	}

	if( item == EnvList.first ){
		EnvList.first = item->next;

		if( EnvList.first == NULL ){
			EnvList.last = NULL;
		}else{
			EnvList.first->prev = NULL;
		}
	}else{
		item->prev->next = item->next;

		if( item == EnvList.last ){
			EnvList.last = item->prev;
		}
	}

	if( item->variable != NULL ) free( item->variable );
	if( item->value != NULL ) free( item->value );
	total_data_size -= item->variable_length;
	total_data_size -= item->value_length;
	free( item );

	return REV_SUCCESS;
}

int Edit_Envitem( env_item_t* item, char* variable, char* data ){
	int variable_length = 0;
	int value_length = 0;

	if( item == NULL ){
		printf("[Edit] not set item\n");
		return REV_FAIL;
	}else if( variable == NULL ){
		printf("[Edit] variable not set\n");
		return REV_FAIL;
	}else if( data == NULL ){
		printf("[Edit] no data\n");
		return REV_FAIL;
	}

        variable_length = strlen(variable) + 1;
        value_length = strlen(data) + 1;

	if( (total_data_size - item->variable_length - item->value_length + variable_length + value_length) <= ENV_SIZE ){
		total_data_size -= item->variable_length;
		total_data_size -= item->value_length;

		item->variable_length = variable_length;
		item->value_length = value_length;

		if( item->variable != NULL ) free( item->variable );
		item->variable = (char*) calloc( item->variable_length, sizeof(char) );
		strcpy( item->variable, variable );

		if( item->value != NULL ) free( item->value );
		item->value = (char*) calloc( item->value_length, sizeof(char) );
		strcpy( item->value, data );

		total_data_size += item->variable_length;
		total_data_size += item->value_length;

		return REV_SUCCESS;
	}else{
		printf("[Edit] memory not enough\n");
		return REV_FAIL;
	}
}

int Add_Envitem( char* variable, char* data ){
	env_item_t* temp_item = Search_Envitem( variable );

	if( temp_item != NULL ){
		printf("[Add] variable still in\n");
		return REV_FAIL;
	}else if( data == NULL ){
		printf("[Add] no data\n");
		return REV_FAIL;
	}else{
		return Create_Envitem( variable, data );
	}
}

int Delete_Envitem( char* variable ){
	env_item_t* temp_item = Search_Envitem( variable );

	if( temp_item == NULL ){
		printf("[Delete] variable not find\n");
		return REV_FAIL;
	}else{
		return Destroy_Envitem( temp_item );
	}
}

int Modify_Envitem( char* variable, char* data ){
	env_item_t* temp_item = Search_Envitem( variable );

	if( temp_item == NULL ){
		printf("[Modify] variable not find\n");
		return REV_FAIL;
	}else if( data == NULL ){
		printf("[Modify] no data\n");
		return REV_FAIL;
	}else{
		return Edit_Envitem( temp_item, variable, data );
	}
}

int Set_Envitem( char* variable, char* data ){
	env_item_t* temp_item = Search_Envitem( variable );

	if( temp_item == NULL ){
		if( data == NULL ){
			printf("[Set] variable not find, and no data\n");
			return REV_FAIL;
		}else{
			return Create_Envitem( variable, data );
		}
	}else{
		if( data == NULL ){
			return Destroy_Envitem( temp_item );
		}else{
			return Edit_Envitem( temp_item, variable, data );
		}
	}
}

