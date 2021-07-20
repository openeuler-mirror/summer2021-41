//
// Created by lenovo on 2021/7/18.
//

#include "user_procinfo.h"

//#include <curses.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <term.h>            // foul sob, defines all sorts of stuff...
#undef    raw
#undef    tab
#undef    TTY
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/select.h>      // also available via <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>       // also available via <stdlib.h>

#include <proc/alloc.h>
#include <proc/devname.h>
#include <proc/numa.h>
#include <proc/procps.h>
#include <proc/readproc.h>
#include <proc/sig.h>
#include <proc/sysinfo.h>
#include <proc/version.h>
#include <proc/wchan.h>
#include <proc/whattime.h>

/* Frame oriented stuff that can't remain local to any 1 function
   and/or that would be too cumbersome managed as parms,
   and/or that are simply more efficiently handled as globals
   [ 'Frames_...' (plural) stuff persists beyond 1 frame ]
   [ or are used in response to async signals received ! ] */
static volatile int Frames_signal;     // time to rebuild all column headers
static          int Frames_libflags;   // PROC_FILLxxx flags
static int          Frame_maxtask;     // last known number of active tasks
// ie. current 'size' of proc table
static float        Frame_etscale;     // so we can '*' vs. '/' WHEN 'pcpu'
static unsigned     Frame_running,     // state categories for this frame
Frame_sleepin,
        Frame_stopped,
        Frame_zombied;
static int          Frame_srtflg,      // the subject window's sort direction
Frame_ctimes,      // the subject window's ctimes flag
Frame_cmdlin;      // the subject window's cmdlin flag

/* Specific process id monitoring support */
//static pid_t Monpids [MONPIDMAX+1] = { 0 };
static int   Monpidsidx = 0;

/* Global/Non-windows mode stuff that is NOT persistent */
static int Batch = 0,           // batch mode, collect no input, dumb output
Loops = -1,          // number of iterations, -1 loops forever
Secure_mode = 0,     // set if some functionality restricted
Thread_mode = 0,     // set w/ 'H' - show threads via readeither()
Width_mode = 0;      // set w/ 'w' - potential output override

/* Our four WIN_t's, and which of those is considered the 'current'
   window (ie. which window is associated with any summ info displayed
   and to which window commands are directed) */
static WIN_t  Winstk [GROUPSMAX];
static WIN_t *Curwin;

/*######  Low Level Memory/Keyboard/File I/O support  ####################*/

/*
 * Handle our own memory stuff without the risk of leaving the
 * user's terminal in an ugly state should things go sour. */

static void *alloc_c (size_t num) MALLOC;
static void *alloc_c (size_t num) {
    void *pv;

    if (!num) ++num;
    if (!(pv = calloc(1, num)))
        printf("alloc_c error");
    return pv;
} // end: alloc_c


static void *alloc_r (void *ptr, size_t num) MALLOC;
static void *alloc_r (void *ptr, size_t num) {
    void *pv;

    if (!num) ++num;
    if (!(pv = realloc(ptr, num)))
        printf("alloc_r error");
    return pv;
} // end: alloc_r


static char *alloc_s (const char *str) MALLOC;
static char *alloc_s (const char *str) {
    return strcpy(alloc_c(strlen(str) +1), str);
} // end: alloc_s

void user_memory_info(){
    int fock_rv;
    char* arglist[2];

    arglist[0] = "top";
    arglist[1] = NULL;

    fock_rv = fork();
    if(fock_rv < 0) {
        printf("fork error \n");
    }else if(fock_rv == 0){
        execvp("top", arglist);
        exit(1);
    }else {
        int ret, ret_val;
        wait(&ret);
        ret_val = ret >> 8;
        if( ret_val != 0){
            printf("Top exec error, ret :%d \n", ret_val);
        }else{
            exit(0);
        }
    }
}
static void procs_refresh (void);
void test_read_proc(){
    procs_refresh();
}

static void procs_hlp (proc_t *this);
/*
 * This guy's modeled on libproc's 'readproctab' function except
 * we reuse and extend any prior proc_t's.  He's been customized
 * for our specific needs and to avoid the use of <stdarg.h> */
static void procs_refresh (void) {
#define n_used  Frame_maxtask                   // maintained by procs_hlp()
    static proc_t **private_ppt;                  // our base proc_t ptr table
    static int n_alloc = 0;                       // size of our private_ppt
    static int n_saved = 0;                       // last window ppt size
    unsigned long total_proc_memo = 0;
    proc_t *ptask;
    PROCTAB* PT;
    int i;
    proc_t *(*read_something)(PROCTAB*, proc_t *);

    procs_hlp(NULL);                              // prep for a new frame
    Frames_libflags = PROC_FILLSTATUS | PROC_FILLUSR;
    if (NULL == (PT = openproc(Frames_libflags)))
        printf("openproc Error!");
    read_something = Thread_mode ? readeither : readproc;

    for (;;) {
        if (n_used == n_alloc) {
            /* we're subject to integer overflow if total linux tasks ever approach |
               400+ million (but, do you think memory might be the bigger problem?) | */
            n_alloc = 10 + ((n_alloc * 5) / 4);     // grow by over 25%
            private_ppt = alloc_r(private_ppt, sizeof(proc_t *) * n_alloc);
            // ensure NULL pointers for the additional memory just acquired
            memset(private_ppt + n_used, 0, sizeof(proc_t *) * (n_alloc - n_used));
        }
        // on the way to n_alloc, the library will allocate the underlying
        // proc_t storage whenever our private_ppt[] pointer is NULL...
        if (!(ptask = read_something(PT, private_ppt[n_used]))) break;
        procs_hlp((private_ppt[n_used] = ptask));  // tally this proc_t
    }

    closeproc(PT);
//    printf("P");
    for(int i = 0; i < n_used; i++){
        total_proc_memo += private_ppt[i] -> vm_rss;
//        printf("%d, %s %s : %ld kB  %ld kB\n" ,private_ppt[i]->tid,private_ppt[i]->cmd,
//               private_ppt[i]->ruser, private_ppt[i]->vm_rss, private_ppt[i]->vm_size);
    }

    printf("proc used: %ld kB\n", total_proc_memo);
#undef n_used
} // end: procs_refresh

static void procs_hlp (proc_t *this) {
#ifdef OFF_HST_HASH
    static unsigned maxt_sav = 0;        // prior frame's max tasks
#endif
//    TIC_t tics;
//    HST_t *h;

    if (!this) {
        static double uptime_sav;
        double uptime_cur;
        float et;
        void *v;

        uptime(&uptime_cur, NULL);
        et = uptime_cur - uptime_sav;
        if (et < 0.01) et = 0.005;
        uptime_sav = uptime_cur;

        // if in Solaris mode, adjust our scaling for all cpus
//        Frame_etscale = 100.0f / ((float)Hertz * (float)et * (Rc.mode_irixps ? 1 : smp_num_cpus));
//#ifdef OFF_HST_HASH
//        maxt_sav = Frame_maxtask;
//#endif
//        Frame_maxtask = Frame_running = Frame_sleepin = Frame_stopped = Frame_zombied = 0;
//
//        // prep for saving this frame's HST_t's (and reuse mem each time around)
//        v = PHist_sav;
//        PHist_sav = PHist_new;
//        PHist_new = v;
//#ifdef OFF_HST_HASH
//        // prep for binary search by sorting the last frame's HST_t's
//      qsort(PHist_sav, maxt_sav, sizeof(HST_t), (QFP_t)sort_HST_t);
//#else
//        v = PHash_sav;
//        PHash_sav = PHash_new;
//        PHash_new = v;
//        memcpy(PHash_new, HHash_nul, sizeof(HHash_nul));
//#endif
        return;
    }

    switch (this->state) {
        case 'R':
            Frame_running++;
            break;
        case 't':     // 't' (tracing stop)
        case 'T':
            Frame_stopped++;
            break;
        case 'Z':
            Frame_zombied++;
            break;
        default:
            /* currently: 'D' (disk sleep),
                          'I' (idle),
                          'P' (parked),
                          'S' (sleeping),
                          'X' (dead - actually 'dying' & probably never seen)
            */
            Frame_sleepin++;
            break;
    }

//    if (Frame_maxtask+1 >= HHist_siz) {
//        /* we're subject to integer overflow if total linux tasks ever approach |
//           400+ million (but, do you think memory might be the bigger problem?) | */
//        HHist_siz = HHist_siz * 5 / 4 + 100;
//        PHist_sav = alloc_r(PHist_sav, sizeof(HST_t) * HHist_siz);
//        PHist_new = alloc_r(PHist_new, sizeof(HST_t) * HHist_siz);
//    }

    /* calculate time in this process; the sum of user time (utime) and
       system time (stime) -- but PLEASE dont waste time and effort on
       calcs and saves that go unused, like the old top! */
//    PHist_new[Frame_maxtask].pid  = this->tid;
//    PHist_new[Frame_maxtask].tics = tics = (this->utime + this->stime);
//    // finally, save major/minor fault counts in case the deltas are displayable
//    PHist_new[Frame_maxtask].maj = this->maj_flt;
//    PHist_new[Frame_maxtask].min = this->min_flt;
//
//#ifdef OFF_HST_HASH
//    // find matching entry from previous frame and make stuff elapsed
//   if ((h = hstbsrch(PHist_sav, maxt_sav - 1, this->tid))) {
//      tics -= h->tics;
//      this->maj_delta = this->maj_flt - h->maj;
//      this->min_delta = this->min_flt - h->min;
//   }
//#else
//    // hash & save for the next frame
//    hstput(Frame_maxtask);
//    // find matching entry from previous frame and make stuff elapsed
//    if ((h = hstget(this->tid))) {
//        tics -= h->tics;
//        this->maj_delta = this->maj_flt - h->maj;
//        this->min_delta = this->min_flt - h->min;
//    }
//#endif
//
//    /* we're just saving elapsed tics, to be converted into %cpu if
//       this task wins it's displayable screen row lottery... */
//    this->pcpu = tics;

    // shout this to the world with the final call (or us the next time in)
    Frame_maxtask++;
} // end: procs_hlp
