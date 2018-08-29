cmake_minimum_required(VERSION 3.12)
project(Framework)

set(CMAKE_CXX_STANDARD 17)

add_executable(Framework main.cpp
        include/pthread.h
        include/sched.h
        include/semaphore.h
        )

target_link_libraries(Framework ${CMAKE_SOURCE_DIR}/lib/libabc.a)
target_include_directories(Framework PRIVATE ${CMAKE_SOURCE_DIR}/include)

