cmake_minimum_required(VERSION 3.0)
project(tree-family)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# platform： PC TK1
IF(NOT TARGET_PLATFORM)
	SET(TARGET_PLATFORM "MACOS")
ENDIF()

#大家都需要的头文件目录

if(TARGET_PLATFORM STREQUAL "TK1")
	include_directories(/arm-tk1/include/)
elseif(TARGET_PLATFORM STREQUAL "PC")
	include_directories(/usr/local/include/)
elseif(TARGET_PLATFORM STREQUAL "MACOS")
	include_directories(/usr/local/include/)
	link_directories(/usr/local/lib)
endif()

add_subdirectory(binary-search-tree)
