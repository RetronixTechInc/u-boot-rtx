#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define TOOL_DATE "2013.04.01"
#define TOOL_VERSION "0.0.1"

#define get_usage(format, arg...) (printf("Usage: %s ", __FILE__ ), printf(format ,## arg))
#define get_ver(format, arg...) (printf("Version: %s ", __FILE__ ), printf(format ,## arg))


#define veb_on
#define msg_on
#define war_on
#define err_on
#define eng_on

#ifdef veb_on
#define debug_veb(format, arg...) (printf("[veb]%s[%d]:%s : ", __FILE__ , __LINE__ , __FUNCTION__  ), printf(format ,## arg))
#else
#define debug_veb(format, arg...)
#endif

#ifdef msg_on
#define debug_msg(format, arg...) (printf("[msg]%s[%d]:%s : ", __FILE__ , __LINE__ , __FUNCTION__  ), printf(format ,## arg))
#else
#define debug_msg(format, arg...)
#endif

#ifdef war_on
#define debug_war(format, arg...) (printf("[war]%s[%d]:%s : ", __FILE__ , __LINE__ , __FUNCTION__  ), printf(format ,## arg))
#else
#define debug_war(format, arg...)
#endif

#ifdef err_on
#define debug_err(format, arg...) (printf("[err]%s[%d]:%s : ", __FILE__ , __LINE__ , __FUNCTION__  ), printf(format ,## arg))
#else
#define debug_err(format, arg...)
#endif

#ifdef eng_on
#define debug_eng(format, arg...) (printf("[eng]%s[%d]:%s : ", __FILE__ , __LINE__ , __FUNCTION__  ), printf(format ,## arg))
#else
#define debug_eng(format, arg...)
#endif

//support for --gpio --rs232 --rs485 --test
/*struct option
   {
	const char *name;   //parameter name
	int has_arg;		//是否有帶parameter 0: none, 1: must get by optarg, 2: don't care but optarg is null
	int *flag;			//
	int val;			//
	}
*/
static struct option COMMON_LONG_OPTSTR[] = {
	{"gpio", 1, 0, 0},
	{"rs232", 1, 0, 1},
	{"rs485", 1, 0, 2},
	{"startup", 1, 0, 3},
	{"display", 1, 0, 4},
	{"devopen", 1, 0, 5},
	{"cat", 1, 0, 6},
	{"version", 0, 0, 'v'},
	{"test", 1, 0, 'z'},
	{0 ,0 , 0, 0}
};

#define ARRAY_MAX 64 //maximun number of array size

//support for -a -b -c -d
#define  COMMON_SHORT_OPTSTR "ab:c:dv"

//RS232
#define DTR_LowActive



