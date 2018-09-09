cmake_minimum_required(VERSION 3.12)
project(abc_LAST_if)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DABC_NAMESPACE=abc)


add_executable(abc_LAST_if main.cpp)
add_subdirectory(abc)
add_subdirectory(util)
add_subdirectory(abc/include/map/if)

include_directories(${ext_include})
include_directories(${ext_lib})
include_directories(${util_include})


target_link_libraries(abc_LAST_if
        ${if_libraries}
        ${abc_libraries}
        )
