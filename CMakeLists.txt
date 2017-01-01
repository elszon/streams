project(streams)
cmake_minimum_required(VERSION 2.8)

#set( CMAKE_C_COMPILER   "gcc-5" )
#set( CMAKE_CXX_COMPILER "g++-5" )


#set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_C_FLAGS_RELEASE   "-DNDEBUG -v")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -v")

set(CMAKE_C_FLAGS_DEBUG   "-g3 -v")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -v")


#set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -std=c11  " )
#set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -std=c++98" )

#set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -O -c" )
#set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -O -c" )

set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wfloat-equal -Wswitch-default -Wuninitialized -Wunreachable-code" )
set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wfloat-equal -Wswitch-default -Wuninitialized -Wunreachable-code" )

#Dr.Memory:              on x64 require g++-multilib
#set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -m32 -g -fno-inline -fno-omit-frame-pointer" )

FILE(GLOB_RECURSE HEADER_FILES include *.h)

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADER_FILES})

#target_link_libraries(${PROJECT_NAME} pthread)
