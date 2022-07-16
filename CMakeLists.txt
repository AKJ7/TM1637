cmake_minimum_required(VERSION 3.10)
project(TM1637 CXX C)
set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME}
         src/TM1637.h
         src/mI2C.h
         src/type_traits.h
         src/animator.h
         examples/basic/basic.ino
         examples/animation/animation.ino)
target_include_directories(
        ${PROJECT_NAME} INTERFACE
        "/home/hp/Program/arduino-1.8.9/hardware/arduino/avr/cores/arduino"
        "src"
)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wpedantic)