cmake_minimum_required(VERSION 3.16)
project(summer2021_41 C)

set(CMAKE_C_STANDARD 11)

set(SRC_LIST memory_collection.c
        global_info.c
        user_procinfo.c
        )

add_executable(summer2021_41 ${SRC_LIST})

# procps库路径
include_directories(/usr/include/proc/ /usr/include/)
link_directories( /usr/lib/libprocps.la /usr/lib/x86_64-linux-gnu/libncurses.so /usr/lib/lx86_64-linux-gnu/ibprocps.a)

#include_directories(/usr/include/proc/ /usr/include/)
#link_directories(  /usr/lib/x86_64-linux-gnu/libncurses.so )
# 链接动态库
target_link_libraries(summer2021_41 libprocps.so libncurses.so libprocps.a)
#target_link_libraries(summer2021_41 libncurses.so)
