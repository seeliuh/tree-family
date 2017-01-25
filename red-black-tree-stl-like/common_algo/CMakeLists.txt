cmake_minimum_required(VERSION 3.0)
project(binary-search-tree-testor)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# platform： PC TK1
IF(NOT TARGET_PLATFORM)
	SET(TARGET_PLATFORM "MACOS")
ENDIF()

#大家都需要的头文件目录

include_directories(../unit_test ../)
aux_source_directory(. SRC_LIST)
add_definitions(-g -O0)

add_executable(common_algo ${SRC_LIST})
