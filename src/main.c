#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include "global_info.h"
#include "user_procinfo.h"
#include "numa.h"

void show_help(){
    fprintf(stderr, "Usage: memo [-g] [-k] [-u] [-r] [-v] [-p] [-t <topN>] [-s <PID1, PID2, ...>] \n");
    fprintf(stderr, "   -g to show global memory usage info like free\n");
    fprintf(stderr, "   -k to show kernel memory usage info \n");
    fprintf(stderr, "   -u to show user memory usage info\n");
    fprintf(stderr, "   -p to show processes memory usage info, default top 10, from big to small\n");
    fprintf(stderr, "   -t <topN> to show topN processes memory usage info\n");
    fprintf(stderr, "   -r reverse sort memory use from small to big\n");
    fprintf(stderr, "   -v sort processes by VSS\n");
    // fprintf(stderr, "-V to show the %s code version\n", prog_name);
    fprintf(stderr, "   -s <PID1,PID2,...> to show special processes memory usage info split by ','\n");
    fprintf(stderr, "   -n show numa stat\n");
    fprintf(stderr, "   -m show numa memory usage\n");
    exit(1);
}

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
    const static char *Qury = "gkupt:rvs:nhm";

    struct procs_show_settings procs_settings = {0, 10, NULL};


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
                // printf("%s \n", optarg);
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
            case 'n':
                flags |= MEMO_NUMA_FLAG;
                break;
            case 'm':
                flags |= MEMO_NUMA_M_FLAG;
                break;
            case 'h':
                show_help();
                return -1;
            default:
                show_help();
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

    if(flags & MEMO_NUMA_FLAG){

        numa_show(argc, argv);
    }

    if(flags & MEMO_NUMA_M_FLAG){
        numa_show(argc, argv);
    }
    if(flags == 0){
        global_info();
    }
    return 0;
}