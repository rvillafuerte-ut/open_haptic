cmake_minimum_required(VERSION 3.16)
project(open_haptic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimización para mejor performance
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Include directories
set(EIGEN3_INCLUDE_DIR "/home/utec/libs_utils/eigen")
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories("/usr/local/include/dynamixel_sdk")

find_package(pinocchio REQUIRED)

# Main executable
add_executable(open_haptic src/CNL_L6_act_2.cpp)
target_compile_definitions(open_haptic PRIVATE linux HDAPIENTRY= HDCALLBACK=)
target_link_libraries(open_haptic PRIVATE dxl_x64_cpp pthread HD ncurses tinfo)

