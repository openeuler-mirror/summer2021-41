#include <stdio.h>
#include <getopt.h>
#include <limits.h>
#include "global_info.h"

int main(int argc, char *argv[]){

    int c, idx = 0;
    // 格式 a 只有选项不带参数
    // a:   其后需要跟参数
    const static char *Qury = "agck";
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
            case 'a':
                free_memo(argc, argv, idx);
                break;
            case 'g':
                puts("G option");
                global_info();
                break;
            case 'c':
//                test_curse();
                test_color();
                break;
            case 'k':
                kernel_memory();
                break;
            default:
                printf("Error \n");
        }
    }
    printf("Done!");
    return 0;
}