cmake_minimum_required(VERSION 3.12)
project(abc_LAST)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DABC_NAMESPACE=abc)


add_executable(abc_LAST main.cpp)
add_subdirectory(abc)
add_subdirectory(util)

include_directories(${ext_include})
include_directories(${ext_lib})
include_directories(${util_include})


target_link_libraries(abc_LAST
        ${abc_libraries}
        ${ext_lib}
        util_libraries
        )
