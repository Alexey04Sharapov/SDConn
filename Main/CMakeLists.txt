cmake_minimum_required(VERSION 3.15)
project(Main C)

set(CMAKE_C_STANDARD 11)

add_executable(Main main.c presentation/pres.c plugin_manager/pm.c)

target_link_libraries(Main ${CMAKE_DL_LIBS})
