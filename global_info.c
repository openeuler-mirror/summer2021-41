//
// Created by lenovo on 2021/7/10.
//

#include "global_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <proc/sysinfo.h>
#include <ncurses.h>
#include "color.h"

#define FREE_HUMANREADABLE	(1 << 1)
#define FREE_LOHI		(1 << 2)
#define FREE_WIDE		(1 << 3)
#define FREE_TOTAL		(1 << 4)
#define FREE_SI			(1 << 5)
#define FREE_REPEAT		(1 << 6)
#define FREE_REPEATCOUNT	(1 << 7)

struct commandline_arguments {
    int exponent;		/* demanded in kilos, magas... */
    float repeat_interval;	/* delay in seconds */
    int repeat_counter;	/* number of repeats */
};


static const char *scale_size(unsigned long size, int flags, struct commandline_arguments args);

void free_memo(int argc, char **argv, int idx){
    int fock_rv;
    char* arglist[2];

    arglist[0] = "free";
    arglist[1] = "-h";

    fock_rv = fork();
    if(fock_rv == 0){
        execvp("free", arglist);
        exit(1);
    }else {
        int ret, ret_val;
        wait(&ret);
        ret_val = ret >> 8;
        if( ret_val != 0){
            printf("Free exec error, ret :%d \n", ret_val);
        }else{
            exit(0);
        }
    }
}

void global_info(){
    struct commandline_arguments args;

    /* defaults */
    args.exponent = 0;
    args.repeat_interval = 1000000;
    args.repeat_counter = 0;
    meminfo();

    printf("               total     ");
    SET_color(WHITE, BG_RED);
    printf("   used    ");

    SET_color(WHITE, BG_GREEN);
    printf("    free    ");

    SET_color(WHITE, BG_YELLOW)
    printf(" shared  buff/cache   ");

    RESET_color()

    printf(" available");

    printf("\n");

    printf("%-9s", "Mem:");

    int flags = FREE_HUMANREADABLE;

    printf("%11s", scale_size(kb_main_total, flags, args));
    printf(" %11s", scale_size(kb_main_used, flags, args));
    printf(" %11s", scale_size(kb_main_free, flags, args));
    printf(" %11s", scale_size(kb_main_shared, flags, args));
    if (flags & FREE_WIDE) {
        printf(" %11s", scale_size(kb_main_buffers, flags, args));
        printf(" %11s", scale_size(kb_main_cached, flags, args));
    } else {
        printf(" %11s", scale_size(kb_main_buffers+kb_main_cached, flags, args));
    }
    printf(" %11s", scale_size(kb_main_available, flags, args));
    printf("\n");

    printf("%-9s", "Swap:");
    printf("%11s", scale_size(kb_swap_total, flags, args));
    printf(" %11s", scale_size(kb_swap_used, flags, args));
    printf(" %11s", scale_size(kb_swap_free, flags, args));
    printf("\n");

    exit(EXIT_SUCCESS);
}

void kernel_memory(){
    struct commandline_arguments args;

    /* defaults */
    args.exponent = 0;
    args.repeat_interval = 1000000;
    args.repeat_counter = 0;
    meminfo();
    unsigned long kernel_used = kb_slab + kb_pagetables;

//    long long kernel_all = kb_slab + kb_pagetables +

    prints(RED, BG_WHITE, "Linux Kernel Internal Memory:", 1);

    printf("%-13s", "Toral = ");
    printf("%-12s", scale_size(kernel_used, FREE_HUMANREADABLE, args));
    printf("%-13s", "Cached = ");
    printf("%-12s", scale_size(kb_main_cached, FREE_HUMANREADABLE, args));
    printf("%-13s", "Active = ");
    printf("%-12s", scale_size(kb_active, FREE_HUMANREADABLE, args));
    printf("\n");

    printf("%-13s", "Buffers = ");
    printf("%-12s", scale_size(kb_main_buffers, FREE_HUMANREADABLE, args));
    printf("%-13s", "Swapcached = ");
    printf("%-12s", scale_size(kb_swap_cached, FREE_HUMANREADABLE, args));
    printf("%-13s", "Inactivate = ");
    printf("%-12s", scale_size(kb_main_buffers, FREE_HUMANREADABLE, args));
    printf("\n");

    printf("%-13s", "Dirty = ");
    printf("%-12s", scale_size(kb_dirty, FREE_HUMANREADABLE, args));
    printf("%-13s", "Writeback = ");
    printf("%-12s", scale_size(kb_writeback, FREE_HUMANREADABLE, args));
    printf("%-13s", "Mapped = ");
    printf("%-12s", scale_size(kb_mapped, FREE_HUMANREADABLE, args));
    printf("\n");

    printf("%-13s", "Slab = ");
    printf("%-12s", scale_size(kb_slab, FREE_HUMANREADABLE, args));
    printf("%-13s", "Commit_As = ");
    printf("%-12s", scale_size(kb_committed_as, FREE_HUMANREADABLE, args));
    printf("%-13s", "PageTables = ");
    printf("%-12s", scale_size(kb_pagetables, FREE_HUMANREADABLE, args));
    printf("\n");
}

void test_color(){
    prints(WHITE, BG_RED,"JT ", 1);
    prints(WHITE, BG_GREEN,"Fuck ", 0);
    prints(WHITE, BG_YELLOW, "oooo", 0);
//    printf("\033[31m Fukc jt \033[0m \n");
}

void test_curse(){
    int x = 0;
    int y = 0;
    int max_x = 0,max_y = 0;
    int next_x = 0;
    int direction = 1;
#define DELAY 30000
    initscr(); /* 初始化屏幕 */

    noecho(); /* 屏幕上不返回任何按键 */

    curs_set(FALSE); /* 不显示光标 */

    /* getmaxyx(stdscr, max_y, max_x);/* 获取屏幕尺寸 */

    mvprintw(5, 5, "Hello, world!");

    refresh(); /* 更新显示器 */

    sleep(1);

    while(1)
    {
        getmaxyx(stdscr, max_y, max_x);/* 获取屏幕尺寸 */
        clear(); /* 清屏 */
        mvprintw(y, x, "O");
        refresh();

        usleep(DELAY);

        next_x = x + direction;

        if(next_x >= max_x || next_x < 0)
        {
            direction = (-1) * direction;
        }
        else
        {
            x = x + direction;
        }
    }
}

double power(unsigned int base, unsigned int expo)
{
    return (expo == 0) ? 1 : base * power(base, expo - 1);
}

/* idea of this function is copied from top size scaling */
static const char *scale_size(unsigned long size, int flags, struct commandline_arguments args)
{
    static char up[] = { 'B', 'K', 'M', 'G', 'T', 'P', 0 };
    static char buf[BUFSIZ];
    int i;
    float base;
    long long bytes;

    base = (flags & FREE_SI) ? 1000.0 : 1024.0;
    bytes = size * 1024LL;

    if (!(flags & FREE_HUMANREADABLE)) {
        switch (args.exponent) {
            case 0:
                /* default output */
                snprintf(buf, sizeof(buf), "%ld", size);
                return buf;
            case 1:
                /* in bytes, which can not be in SI */
                snprintf(buf, sizeof(buf), "%lld", bytes);
                return buf;
            default:
                /* In desired scale. */
                snprintf(buf, sizeof(buf), "%ld",
                         (long)(bytes / power(base, args.exponent-1)));
                return buf;
        }
    }

    /* human readable output */
    if (4 >= snprintf(buf, sizeof(buf), "%lld%c", bytes, up[0]))
        return buf;

    for (i = 1; up[i] != 0; i++) {
        if (flags & FREE_SI) {
            if (4 >= snprintf(buf, sizeof(buf), "%.1f%c",
                              (float)(bytes / power(base, i)), up[i]))
                return buf;
            if (4 >= snprintf(buf, sizeof(buf), "%ld%c",
                              (long)(bytes / power(base, i)), up[i]))
                return buf;
        } else {
            if (5 >= snprintf(buf, sizeof(buf), "%.1f%ci",
                              (float)(bytes / power(base, i)), up[i]))
                return buf;
            if (5 >= snprintf(buf, sizeof(buf), "%ld%ci",
                              (long)(bytes / power(base, i)), up[i]))
                return buf;
        }
    }
    /*
     * On system where there is more than exbibyte of memory or swap the
     * output does not fit to column. For incoming few years this should
     * not be a big problem (wrote at Apr, 2015).
     */
    return buf;
}
