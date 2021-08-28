cmake_minimum_required(VERSION 3.10)
project(memo C)

# 添加gdb调试信息
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_C_STANDARD 11)

# 递归查找src文件下的所有c文件
file(GLOB_RECURSE 
        native_srcs src/*.c)

 # 源文件列表
set(SRC_LIST 
        ${native_srcs})

# 头文件列表，包含库文件和非库文件
set(INCLUDE_LIST
        /usr/include/proc/ 
        /usr/include/ 
        ./include/)

# 库文件列表
set(LIB_LIST_PATH
        /usr/lib/libprocps.la
        /usr/lib/lx86_64-linux-gnu/libprocps.a)

set(LIB_LIST
        libprocps.so
        libprocps.a)

# 生成名字为工程名字的二进制文件
add_executable(${PROJECT_NAME}
        ${SRC_LIST})

# procps库路径
include_directories( ${INCLUDE_LIST})

link_directories( ${LIB_LIST_PATH})

# 链接动态库
target_link_libraries(${PROJECT_NAME}  ${LIB_LIST})
