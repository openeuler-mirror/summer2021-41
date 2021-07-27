//
// Created by lenovo on 2021/7/18.
//

#ifndef SUMMER2021_41_USER_PROCINFO_H
#define SUMMER2021_41_USER_PROCINFO_H

#include <proc/readproc.h>

extern void user_memory_info();
extern void test_read_proc();
extern void used_memory_info();

#ifdef STRINGCASENO
#define STRSTR  strcasestr
#define STRCMP  strcasecmp
#else
#define STRSTR  strstr
#define STRCMP  strcmp
#endif


/*######  Some Miscellaneous constants  ##################################*/

/* The default delay twix updates */
#ifdef ORIG_TOPDEFS
#define DEF_DELAY  3.0
#else
#define DEF_DELAY  1.5
#endif

/* Length of time a message is displayed and the duration
   of a 'priming' wait during library startup (in microseconds) */
#define MSG_USLEEP  1250000
#define LIB_USLEEP  150000

/* Specific process id monitoring support (command line only) */
#define MONPIDMAX  20

/* Output override minimums (the -w switch and/or env vars) */
#define W_MIN_COL  3
#define W_MIN_ROW  3

/* Miscellaneous buffers with liberal values and some other defines
   -- mostly just to pinpoint source code usage/dependancies */
#define SCREENMAX   512
/* the above might seem pretty stingy, until you consider that with every
   field displayed the column header would be approximately 250 bytes
   -- so SCREENMAX provides for all fields plus a 250+ byte command line */
#define CAPBUFSIZ    32
#define CLRBUFSIZ    64
#define PFLAGSSIZ   100
#define SMLBUFSIZ   128
#define MEDBUFSIZ   256
#define LRGBUFSIZ   512
#define OURPATHSZ  1024
#define BIGBUFSIZ  2048
/* in addition to the actual display data, our row might have to accommodate
   many termcap/color transitions - these definitions ensure we have room */
#define ROWMINSIZ  ( SCREENMAX +  4 * (CAPBUFSIZ + CLRBUFSIZ) )
#define ROWMAXSIZ  ( SCREENMAX + 16 * (CAPBUFSIZ + CLRBUFSIZ) )
// minimum size guarantee for dynamically acquired 'readfile' buffer
#define READMINSZ  2048
// size of preallocated search string buffers, same as ioline()
#define FNDBUFSIZ  MEDBUFSIZ


// space between task fields/columns
#define COLPADSTR   " "
#define COLPADSIZ   ( sizeof(COLPADSTR) - 1 )
// continuation ch when field/column truncated
#define COLPLUSCH   '+'

// support for keyboard stuff (cursor motion keystrokes, mostly)
#define kbd_ESC    '\033'
#define kbd_SPACE  ' '
#define kbd_ENTER  '\n'
#define kbd_UP     129
#define kbd_DOWN   130
#define kbd_LEFT   131
#define kbd_RIGHT  132
#define kbd_PGUP   133
#define kbd_PGDN   134
#define kbd_HOME   135
#define kbd_END    136
#define kbd_BKSP   137
#define kbd_INS    138
#define kbd_DEL    139
#define kbd_CtrlO  '\017'

/* Special value in Pseudo_row to force an additional procs refresh
   -- used at startup and for task/thread mode transitions */
#define PROC_XTRA  -1

#ifndef CPU_ZEROTICS
/* This is the % used in establishing the tics threshold below
   which a cpu is treated as 'idle' rather than displaying
   misleading state percentages */
#define TICS_EDGE  20
#endif

/* /////////////////////////////////////////////////////////////// */
/* Special Section: multiple windows/field groups  --------------- */
/* ( kind of a header within a header: constants, types & macros ) */

#define CAPTABMAX  9             /* max entries in each win's caps table   */
#define GROUPSMAX  4             /* the max number of simultaneous windows */
#define WINNAMSIZ  4             /* size of RCW_t winname buf (incl '\0')  */
#define GRPNAMSIZ  WINNAMSIZ+2   /* window's name + number as in: '#:...'  */

/* The Persistent 'Mode' flags!
   These are preserved in the rc file, as a single integer and the
   letter shown is the corresponding 'command' toggle */
// 'View_' flags affect the summary (minimum), taken from 'Curwin'
#define View_CPUSUM  0x008000     // '1' - show combined cpu stats (vs. each)
#define View_CPUNOD  0x400000     // '2' - show numa node cpu stats ('3' also)
#define View_LOADAV  0x004000     // 'l' - display load avg and uptime summary
#define View_STATES  0x002000     // 't' - display task/cpu(s) states summary
#define View_MEMORY  0x001000     // 'm' - display memory summary
#define View_NOBOLD  0x000008     // 'B' - disable 'bold' attribute globally
#define View_SCROLL  0x080000     // 'C' - enable coordinates msg w/ scrolling
// 'Show_' & 'Qsrt_' flags are for task display in a visible window
#define Show_COLORS  0x000800     // 'z' - show in color (vs. mono)
#define Show_HIBOLD  0x000400     // 'b' - rows and/or cols bold (vs. reverse)
#define Show_HICOLS  0x000200     // 'x' - show sort column emphasized
#define Show_HIROWS  0x000100     // 'y' - show running tasks highlighted
#define Show_CMDLIN  0x000080     // 'c' - show cmdline vs. name
#define Show_CTIMES  0x000040     // 'S' - show times as cumulative
#define Show_IDLEPS  0x000020     // 'i' - show idle processes (all tasks)
#define Show_TASKON  0x000010     // '-' - tasks showable when Mode_altscr
#define Show_FOREST  0x000002     // 'V' - show cmd/cmdlines with ascii art
#define Qsrt_NORMAL  0x000004     // 'R' - reversed column sort (high to low)
#define Show_JRSTRS  0x040000     // 'j' - right justify "string" data cols
#define Show_JRNUMS  0x020000     // 'J' - right justify "numeric" data cols
// these flag(s) have no command as such - they're for internal use
#define NOPRINT_xxx  0x010000     // build task rows only (not for display)
#define EQUWINS_xxx  0x000001     // rebalance all wins & tasks (off i,n,u/U)
#ifndef USE_X_COLHDR
#define NOHISEL_xxx  0x200000     // restrict Show_HICOLS for osel temporarily
#define NOHIFND_xxx  0x100000     // restrict Show_HICOLS for find temporarily
#endif

// Default flags if there's no rcfile to provide user customizations
#ifdef ORIG_TOPDEFS
#define DEF_WINFLGS ( View_LOADAV | View_STATES | View_CPUSUM | View_MEMORY \
   | Show_HIBOLD | Show_HIROWS | Show_IDLEPS | Show_TASKON | Show_JRNUMS \
   | Qsrt_NORMAL )
#define DEF_GRAPHS2  0, 0
#define DEF_SCALES2  SK_Mb, SK_Kb
#define ALT_WINFLGS  DEF_WINFLGS
#define ALT_GRAPHS2  0, 0
#else
#define DEF_WINFLGS ( View_LOADAV | View_STATES | View_MEMORY | Show_CMDLIN \
   | Show_COLORS | Show_FOREST | Show_HIROWS | Show_IDLEPS | Show_JRNUMS | Show_TASKON \
   | Qsrt_NORMAL )
#define DEF_GRAPHS2  1, 2
#define DEF_SCALES2  SK_Gb, SK_Mb
#define ALT_WINFLGS (DEF_WINFLGS | Show_HIBOLD) & ~Show_FOREST
#define ALT_GRAPHS2  2, 0
#endif

typedef unsigned char FLG_t;

/* These are used to direct wins_reflag */
enum reflag_enum {
    Flags_TOG, Flags_SET, Flags_OFF
};

/* These are used to direct win_warn */
enum warn_enum {
    Warn_ALT, Warn_VIZ
};

/* This type helps support both a window AND the rcfile */
typedef struct RCW_t {  // the 'window' portion of an rcfile
    int    sortindx,               // sort field (represented as procflag)
    winflags,               // 'view', 'show' and 'sort' mode flags
    maxtasks,               // user requested maximum, 0 equals all
    graph_cpus,             // 't' - View_STATES supplementary vals
    graph_mems,             // 'm' - View_MEMORY supplememtary vals
    double_up,              // '4' - show individual cpus 2 abreast
    combine_cpus,           // '!' - keep combining additional cpus
    summclr,                // a colors 'number' used for summ info
    msgsclr,                //             "           in msgs/pmts
    headclr,                //             "           in cols head
    taskclr;                //             "           in task rows
    char   winname [WINNAMSIZ],    // name for the window, user changeable
    fieldscur [PFLAGSSIZ];  // the fields for display & their order
} RCW_t;

/* This represents the complete rcfile */
typedef struct RCF_t {
    char   id;                   // rcfile version id
    int    mode_altscr;          // 'A' - Alt display mode (multi task windows)
    int    mode_irixps;          // 'I' - Irix vs. Solaris mode (SMP-only)
    float  delay_time;           // 'd'/'s' - How long to sleep twixt updates
    int    win_index;            // Curwin, as index
    RCW_t  win [GROUPSMAX];      // a 'WIN_t.rc' for each window
    int    fixed_widest;         // 'X' - wider non-scalable col addition
    int    summ_mscale;          // 'E' - scaling of summary memory values
    int    task_mscale;          // 'e' - scaling of process memory values
    int    zero_suppress;        // '0' - suppress scaled zeros toggle
} RCF_t;

/* This structure stores configurable information for each window.
   By expending a little effort in its creation and user requested
   maintenance, the only real additional per frame cost of having
   windows is an extra sort -- but that's just on pointers! */
typedef struct WIN_t {
    FLG_t  pflgsall [PFLAGSSIZ],        // all 'active/on' fieldscur, as enum
    procflgs [PFLAGSSIZ];        // fieldscur subset, as enum
    RCW_t  rc;                          // stuff that gets saved in the rcfile
    int    winnum,          // a window's number (array pos + 1)
    winlines,        // current task window's rows (volatile)
    maxpflgs,        // number of displayed procflgs ("on" in fieldscur)
    totpflgs,        // total of displayable procflgs in pflgsall array
    begpflg,         // scrolled beginning pos into pflgsall array
    endpflg,         // scrolled ending pos into pflgsall array
    begtask,         // scrolled beginning pos into Frame_maxtask
    begnext,         // new scrolled delta for next frame's begtask
#ifndef SCROLLVAR_NO
            varcolbeg,       // scrolled position within variable width col
#endif
            varcolsz,        // max length of variable width column(s)
    usrseluid,       // validated uid for 'u/U' user selection
    usrseltyp,       // the basis for matching above uid
    usrselflg,       // flag denoting include/exclude matches
    hdrcaplen;       // column header xtra caps len, if any
    char   capclr_sum [CLRBUFSIZ],      // terminfo strings built from
    capclr_msg [CLRBUFSIZ],      //   RCW_t colors (& rebuilt too),
    capclr_pmt [CLRBUFSIZ],      //   but NO recurring costs !
    capclr_hdr [CLRBUFSIZ],      //   note: sum, msg and pmt strs
    capclr_rowhigh [CLRBUFSIZ],  //         are only used when this
    capclr_rownorm [CLRBUFSIZ],  //         window is the 'Curwin'!
    cap_bold [CAPBUFSIZ],        // support for View_NOBOLD toggle
    grpname [GRPNAMSIZ],         // window number:name, printable
#ifdef USE_X_COLHDR
    columnhdr [ROWMINSIZ],       // column headings for procflgs
#else
            columnhdr [SCREENMAX],       // column headings for procflgs
#endif
            *captab [CAPTABMAX];          // captab needed by show_special()
    struct osel_s *osel_1st;            // other selection criteria anchor
    int    osel_tot;                    // total of other selection criteria
    char  *findstr;                     // window's current/active search string
    int    findlen;                     // above's strlen, without call overhead
    proc_t **ppt;                       // this window's proc_t ptr array
    struct WIN_t *next,                 // next window in window stack
    *prev;                 // prior window in window stack
} WIN_t;

//struct {
//    proc_t **ppt;
//} PROC_cur;

#endif //SUMMER2021_41_USER_PROCINFO_H
