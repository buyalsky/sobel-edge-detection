cmake_minimum_required(VERSION 3.17)
project(image_processing_asg1 C)

set(CMAKE_C_STANDARD 99)
add_executable(image_processing_asg1 main.c pgm_lib.c pgm_lib.h)
target_link_libraries(image_processing_asg1 m)
