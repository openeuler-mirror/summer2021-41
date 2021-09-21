# Summer2021-No.41 开发一个系统内存使用情况统计工具

#### Description
https://gitee.com/openeuler-competition/summer-2021/issues/I3EG0G

#### Software Architecture
A software for especially for memory usage information collection.

#### Installation
1. `sudo apt install libprocps-dev` install `libprocps` develop library, and check the library path in the `CMakeLists.txt`.

2. Clone this repository: `git clone git@gitee.com:openeuler-competition/summer2021-41.git`.

3. Then `cd summer2021-41`.

4. Install `cmake` tools by `sudo apt-get install cmake`, and `mkdir build && cd build` for the cmake temporary file build.

5. `cmake ..` uses `CMakeLists.txt` for generating `make` file.

6. `make memo` gets the executable file.

#### Usage

+ Usage: memo [-g] [-k] [-u] [-r] [-v] [-p] [-t <topN>] [-s <PID1, PID2, ...>] 
   + -g to show global memory usage info like free
   + -k to show kernel memory usage info 
   + -u to show user memory usage info
   + -p to show processes memory usage info, default top 10, from big to small
   + -t <topN> to show topN processes memory usage info
   + -r reverse sort memory use from small to big
   + -v sort processes by VSS
   + -s <PID1,PID2,...> to show special processes memory usage info split by ','
   + -n show numa stat
   + -m show numa memory usage1.  xxxx

#### Contribution

1.  Fork the repository
2.  Create Feat_xxx branch
3.  Commit your code
4.  Create Pull Request

