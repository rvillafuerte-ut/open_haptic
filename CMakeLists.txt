cmake_minimum_required(VERSION 3.16)
project(open_haptic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Eigen (header-only)
set(EIGEN3_INCLUDE_DIR "/home/utec/libs_utils/eigen")

# Pinocchio
find_package(pinocchio REQUIRED)

add_executable(open_haptic_demo src/main.cpp)

target_include_directories(open_haptic_demo PRIVATE ${EIGEN3_INCLUDE_DIR})
target_compile_definitions(open_haptic_demo PRIVATE linux HDAPIENTRY= HDCALLBACK=)
target_link_libraries(open_haptic_demo PRIVATE 
    HD 
    ncurses 
    tinfo 
    pthread 
    dxl_x64_cpp
    pinocchio::pinocchio
)
