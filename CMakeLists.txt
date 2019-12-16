cmake_minimum_required(VERSION 3.10)
project(c_flypes C)

set(CMAKE_C_STANDARD 99)

#set(PL_CURVE "-lplCurve")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PL_CURVE}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_C_FLAGS} ${PL_CURVE}")
#set(CMAKE_SHARED_LINKER_FLAGS  "-lplCurve")

#add_executable(myexe main.c src/print_structures.h src/utilities.h src/queue.h)
add_executable(myexe main.c src/print_structures.c src/utilities.c src/queue.c)

#add_library(mylib STATIC src/print_structures.c src/utilities.c src/queue.c)
#target_link_libraries(myexe mylib)
target_link_libraries(myexe plCurve)




#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/${PROJECT_SOURCE_DIR}/libplcurve_src/src")
#add_executable(c_flypes main.c src/print_structures.h src/utilities.h src/queue.h)

#add_link_options(-lplCurve)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/local/Cellar/gsl/2.6/include")

#link_directories(${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/libplcurve_src/src)
#link_directories(/home/dj/CLionProjects/c-flypes/libplcurve_src/src)
#find_library(lplCurve REQUIRED)
#include_directories(${lplCurve_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${lplCurve_LIBRARIES})
#target_link_libraries(c_flypes ${LIBS})
#add_library(PL_CURVE libplcurve_src/src/plcTopology.c libplcurve_src/config.h.in)
#add_library(PL_CURVE libplcurve_src/src)


#target_link_libraries(c_flypes ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/libplcurve_src/src)
#target_link_libraries(c_flypes PL_CURVE)

