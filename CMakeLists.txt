cmake_minimum_required(VERSION 2.8)
project(my_ftp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Wextra -W -g")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)
set(SOURCE_FILES src/main.c
                 src/ftp_commands.c
                 src/ftp_commands2.c
                 src/ftp_commands3.c
                 src/ftp_commands4.c
                 src/ftp_commands5.c
                 src/ftp_commands6.c
                 src/ftp_commands7.c
                 src/ftp_commands8.c
                 src/main2.c)
add_executable(my_ftp ${SOURCE_FILES})