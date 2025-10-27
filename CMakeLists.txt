cmake_minimum_required(VERSION 3.16)
project(open_haptic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimización para mejor performance
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Eigen (header-only)
set(EIGEN3_INCLUDE_DIR "/home/utec/libs_utils/eigen")

add_executable(test_arm_control src/CNL_L6_act_2.cpp)

target_include_directories(test_arm_control PRIVATE ${EIGEN3_INCLUDE_DIR} /usr/local/include/dynamixel_sdk)

target_compile_definitions(test_arm_control PRIVATE linux HDAPIENTRY= HDCALLBACK=)

target_link_libraries(test_arm_control PRIVATE dxl_x64_cpp pthread ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(test_arm_control PRIVATE HD ncurses tinfo)
