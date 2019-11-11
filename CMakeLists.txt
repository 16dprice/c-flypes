cmake_minimum_required(VERSION 3.10)
project(c_flypes C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/local/Cellar/gsl/2.6/include")

link_directories(libplcurve_src/src)
add_library(PL_CURVE libplcurve_src/src/plcTopology.c libplcurve_src/config.h.in)

add_executable(c_flypes main.c src/utilities.c src/queue.c)

target_link_libraries(c_flypes libplcurve_src/src)
