cmake_minimum_required(VERSION 3.16)

project(open_haptic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Rutas conocidas en tu equipo (ajusta si cambian)
set(EIGEN3_INCLUDE_DIR "/home/utec/libs_utils/eigen")

add_executable(open_haptic_demo src/main.cpp)

# Eigen es header-only: solo necesitamos el include
target_include_directories(open_haptic_demo PRIVATE ${EIGEN3_INCLUDE_DIR})

# OpenHaptics: headers ya están en /usr/include/HD; enlazamos librerías necesarias
target_link_libraries(open_haptic_demo PRIVATE HD ncurses tinfo)

# Fix OpenHaptics headers en Linux: definen 'linux' (no __linux__) y usan stdcall
if(UNIX AND NOT APPLE)
    target_compile_definitions(open_haptic_demo PRIVATE linux HDAPIENTRY= HDCALLBACK=)
endif()
