project(streams)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -std=c11   -ggdb  -Wall -Wextra -Wfloat-equal -Wswitch-default -Wconversion -Wuninitialized -Wunreachable-code " )
set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -std=c++14 -ggdb  -Wall -Wextra -Wfloat-equal -Wswitch-default -Wconversion -Wuninitialized -Wunreachable-code " )

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
