//
// Created by lenovo on 2021/7/12.
//

#ifndef SUMMER2021_41_COLOR_H
#define SUMMER2021_41_COLOR_H

#define PREFIX "\033["
#define RESET   "\033[0m"
#define BLACK   "30;"      /* Black */
#define RED     "31;"      /* Red */
#define GREEN   "32;"      /* Green */
#define YELLOW  "33;"      /* Yellow */
#define BLUE    "34;"      /* Blue */
#define MAGENTA "35;"      /* Magenta */
#define CYAN    "36;"      /* Cyan */
#define WHITE   "37;"      /* White */

#define BG_BLACK   "40"      /* Black */
#define BG_RED     "41"      /* Red */
#define BG_GREEN   "42"      /* Green */
#define BG_YELLOW  "43"      /* Yellow */
#define BG_BLUE    "44"      /* Blue */
#define BG_MAGENTA "45"      /* Magenta */
#define BG_CYAN    "46"      /* Cyan */
#define BG_WHITE   "47"      /* White */

#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define prints(color, bgcolor, context, line) {printf(PREFIX);printf(color); \
printf(bgcolor); printf(";1m") ;printf("%s", context); printf(RESET);  line > 0 ? printf("\n"): printf("");}

#define SET_color(color, bgcolor) {printf(PREFIX);printf(color);printf(bgcolor); printf(";1m");}

#define RESET_color() printf(RESET);

#endif //SUMMER2021_41_COLOR_H
