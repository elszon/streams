project(streams)
cmake_minimum_required(VERSION 3.1)


#set( CMAKE_C_COMPILER   "gcc-5" )
#set( CMAKE_CXX_COMPILER "g++-5" )


#set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")

set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS}   -g3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g3")


set (CMAKE_C_STANDARD   11)
set (CMAKE_CXX_STANDARD 14)

#set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -c" )
#set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -c" )

#set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -v" )
#set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -v" )

set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wfloat-equal -Wswitch-default -Wuninitialized -Wunreachable-code -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override" )
set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wfloat-equal -Wswitch-default -Wuninitialized -Wunreachable-code -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override" )

#Dr.Memory:              on x64 require g++-multilib
#set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -m32 -g -fno-inline -fno-omit-frame-pointer" )

#GPROF
#set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS}   -pg")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -pg")

file(GLOB_RECURSE HEADER_FILES include *.h*)

aux_source_directory(. SRC_LIST)

# find apr lib
#message(STATUS "looking for apr library.")
#find_path(APR_INCLUDE apr_version.h
#           /usr/include/apr-1.0
## other dirs to search
#)
#include_directories(${APR_INCLUDE})
#message(STATUS "apr inclues ${APR_INCLUDE}")
#
#find_library(APR_LIBRARIES apr-1)
#message(STATUS "apr link with ${APR_LIBRARIES}")

# find xml2 by modules
#include(FindLibXml2)
#include_directories(${LIBXML2_INCLUDE_DIR})
#message(STATUS "xml2 inclues ${LIBXML2_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADER_FILES})

#message(STATUS "xml2 link with ${LIBXML2_LIBRARIES}")
#target_link_libraries(${PROJECT_NAME} ${LIBXML2_LIBRARIES})
