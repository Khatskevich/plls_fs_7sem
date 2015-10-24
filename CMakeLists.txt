cmake_minimum_required(VERSION 3.3)
project(main)
project(my_log_library)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
set (CMAKE_EXE_LINKER_FLAGS -pthread)
set(SOURCE_FILES
        include/fat32_structures.h
        include/log.h
        src/log.c
        src/fat_reader.c
        src/fs_driver.c include/fs_driver.h)

include_directories(include)
add_library(my_log_library SHARED src/log.c)
add_executable(main ${SOURCE_FILES} src/fs_driver.c include/fs_driver.h)
target_link_libraries(main my_log_library)