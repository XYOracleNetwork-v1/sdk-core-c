cmake_minimum_required (VERSION 2.6)
project (sdk-tests C CXX)

set (SDK_VERSION_MAJOR 0)
set (SDK_VERSION_MINOR 1)

# bring in the headers
#include_directories(../../src/include)
#include_directories(../../src)




find_package (Threads REQUIRED)

file(GLOB_RECURSE SOURCES "./*.cc")

#include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_BUILD_TYPE Debug)

if(UNIX AND NOT APPLE)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O1 -fsanitize=fuzzer,address")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=fuzzer,address")
	endif()
endif()

ADD_LIBRARY(sdk-xyo STATIC "../../src/XYObjects/Array/WeakArray/WeakArray.c" "../../src/XYObjects/XYObject.c" "../../src/endian.c" "./stubs.c")

add_executable(fuzzer ${SOURCES})
target_link_libraries(fuzzer sdk-xyo)
