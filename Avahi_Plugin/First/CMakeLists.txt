cmake_minimum_required(VERSION 3.15)
project(First C)

set(CMAKE_C_STANDARD 11)

add_library(First SHARED library.c)