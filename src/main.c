#include <stdio.h>
#include <getopt.h>
#include <limits.h>
#include "global_info.h"
#include "user_procinfo.h"

int main(int argc, char *argv[]){

    int c, idx = 0, flags = 0;
    int sortN = 10;

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
    // 格式 a 只有选项不带参数
    // a:   其后需要跟参数
    const static char *Qury = "gkupt:rvs:";

    struct procs_show_settings procs_settings = {0, 10, NULL};


    while((c = getopt_long(argc, argv, Qury, longopts, &idx)) != -1){
        // printf("%c \n", c);
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
                printf("%s \n", optarg);
                procs_settings.show_nums = atoi(optarg);
                break;
            case 'r':
                flags |= MEMO_PROCESS_FLAG;
                flags |= MEMO_REVERSE_FLAG;
                break;
            case 'v':
                flags |= MEMO_PROCESS_FLAG;
                flags |= MEMO_VSS_FLAG;
                break;
            case 's':
                flags |= MEMO_PID_FLAG;
                flags |= MEMO_PROCESS_FLAG;
                procs_settings.pids = optarg;
                break;
            default:
                printf("Error Command, _%c_\n", c);
                return -1;
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
        procs_settings.flag = flags;
        // printf("%s \n", procs_settings.pids);
        user_procs(&procs_settings);
    }
    if(flags == 0){
        global_info();
    }
    return 0;
}