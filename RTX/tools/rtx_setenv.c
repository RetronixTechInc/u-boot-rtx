#include "mp.h"
#include "datalist.h"

/* options args */
static const char *short_options = "?hl";

static struct option long_options[] = {
  {"help"           , no_argument,       0, 'h'                      },
  {"initial"        , required_argument, 0, 'i'                      },
  {"add"            , required_argument, 0, 'a'                      },
  {"modify"         , required_argument, 0, 'm'                      },
  {"delete"         , required_argument, 0, 'd'                      },
  {"set"            , required_argument, 0, 's'                      },   
  {"list"           , no_argument,       0, 'l'                      },
#ifdef CREATE_FILE
  {"create"         , required_argument, 0, 'c'                      },
#endif
  {0                , no_argument,       0, 0                        }
};

static void show_usage(){
	printf("Options:\n");
	printf("        --add     variable=data\n");
	printf("        --modify  variable=data\n");
	printf("        --delete  variable\n");
    printf("        --set     variable=data|variable\n");
	printf("        --list    show all data\n");
	printf("        --initial path\n");
	printf("        --help    help\n");
}

int main(int argc, char **argv)
{
        int c ='?';
        int option_index = 0;
	char* variable = NULL;
	char* data = NULL;
	int error = 0;

	Create_Datalist();

        while( (c = getopt_long(argc, argv, short_options,long_options, &option_index)) != EOF ){
		switch( c ){
			case 'a':
				variable = strtok( optarg, "=" );
				data = strtok( NULL, "\0" );
				error = Add_Envitem( variable, data );
				break;
			case 'm':
				variable = strtok( optarg, "=" );
				data = strtok( NULL, "\0" );
				error = Modify_Envitem( variable, data );
				break;
			case 'd':
				error = Delete_Envitem( optarg );
				break;
			case 's':
				variable = strtok( optarg, "=\0" );
				data = strtok( NULL, "\0" );
				error = Set_Envitem( variable, data );
				break;
			case 'l':
				Show_Datalist();
				error = 2;
				break;
			case 'i':
				error = Init_Datalist( optarg );
				break;
#ifdef CREATE_FILE
                        case 'c':
			{
				variable = strtok( optarg, ":" );
				data = strtok( NULL, "\0" );
                                Create_Init_File( variable, data );
                                error = 2;
                                break;
			}
#endif
			case 'h':
				error = 0;
				break;
			default:
				error = -1;
				break;
		}

		if( error <= 0 ){
			show_usage();
			break;
		}else if( error == 2 ){
			break;
		}
	}

	if( error == 1 ){
		Save_Datalist();
	}

	Destroy_Datalist();

	return 0;
}
