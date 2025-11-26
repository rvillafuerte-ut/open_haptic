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
include_directories(include)
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories("/usr/local/include/dynamixel_sdk")

find_package(pinocchio REQUIRED)

# --- Original Monolithic Executable (Reference) ---
add_executable(test_arm_control src/CNL_L6_act_2.cpp)
target_compile_definitions(test_arm_control PRIVATE linux HDAPIENTRY= HDCALLBACK=)
target_link_libraries(test_arm_control PRIVATE dxl_x64_cpp pthread HD ncurses tinfo)

# --- New Modular Executable ---
set(SOURCES
    src/main.cpp
    src/dynamixel_controller.cpp
    src/haptic_controller.cpp
    src/gripper_controller.cpp
    src/robot_controller.cpp
    src/trajectory_generator.cpp
    src/safety_monitor.cpp
    src/utilities.cpp
)

add_executable(modular_arm_control ${SOURCES})

target_compile_definitions(modular_arm_control PRIVATE linux HDAPIENTRY= HDCALLBACK=)

target_link_libraries(modular_arm_control PRIVATE 
    dxl_x64_cpp 
    pthread 
    HD 
    ncurses 
    tinfo
)

