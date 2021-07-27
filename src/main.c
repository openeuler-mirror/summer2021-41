#include <stdio.h>
#include <getopt.h>
#include <limits.h>
#include "global_info.h"
#include "user_procinfo.h"

#define MEMO_GLOBAL_FLAG (1 << 0)
#define MEMO_USED_FLAG (1 << 1)
#define MEMO_KERNEL_FLAG (1 << 2)
#define MEMO_PROCESS_FLAG (1 << 3)
#define MEMO_PID_FLAG (1 << 4)
#define MEMO_TOPN_FLAG (1 << 5)
#define MEMO_REVERSE_FLAG (1 << 6)
#define MEMO_VSS_FLAG (1 << 7)

int main(int argc, char *argv[]){

    int c, idx = 0, flags = 0;
    // 格式 a 只有选项不带参数
    // a:   其后需要跟参数
    const static char *Qury = "agcktuprv";

    enum {
        SI_OPTION = CHAR_MAX + 1,
        KILO_OPTION,
        MEGA_OPTION,
        GIGA_OPTION,
        TERA_OPTION,
        PETA_OPTION,
        TEBI_OPTION,
        PEBI_OPTION,
        HELP_OPTION
    };

    // longopt 为长选项结构体
    static const struct option longopts[] = {
            {  "help",	no_argument,	    NULL,  HELP_OPTION	},
            {  "version",	no_argument,	    NULL,  'V'		},
            {  NULL,	0,		    NULL,  0		}
    };


    while((c = getopt_long(argc, argv, Qury, longopts, &idx)) != -1){
        switch (c) {
            case 'g':
                flags |= MEMO_GLOBAL_FLAG;
                break;
            case 'k':
                flags |= MEMO_KERNEL_FLAG;
                break;
            case 'u':
                flags |= MEMO_USED_FLAG;
                break;
            case 'p':
                flags |= MEMO_PROCESS_FLAG;
                break;
            case 't':
                flags |= MEMO_PROCESS_FLAG;
                flags |= MEMO_TOPN_FLAG;
            case 'r':
                flags |= MEMO_REVERSE_FLAG;
            case 'v':
                flags |= MEMO_PROCESS_FLAG;
                flags |= MEMO_VSS_FLAG;
            default:
                printf("Error Command\n");
        }
    }

    if(flags & MEMO_GLOBAL_FLAG){
        global_info();
    }
    if(flags & MEMO_KERNEL_FLAG){
        kernel_memory();
    }
    if(flags & MEMO_USED_FLAG){
        used_memory_info();
    }
    if(flags & MEMO_PROCESS_FLAG){
        if(flags & MEMO_PID_FLAG){
            ;
        }

        if(flags & MEMO_TOPN_FLAG){
            ;
        }

        if(flags & MEMO_PID_FLAG){
            ;
        }
    }
    if(flags == 0){
        global_info();
    }
    return 0;
}