//
// Created by lenovo on 2021/7/10.
//

#ifndef SUMMER2021_41_GLOBAL_INFO_H
#define SUMMER2021_41_GLOBAL_INFO_H


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

extern void free_memo(int, char**, int);
extern void global_info();
extern void test_curse();
extern void test_color();
extern void kernel_memory();
extern const char *scale_size(unsigned long size, int flags, struct commandline_arguments args);

#endif //SUMMER2021_41_GLOBAL_INFO_H
