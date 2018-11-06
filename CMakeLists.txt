cmake_minimum_required (VERSION 2.6)
project (sdk-core)
# set the version number.
set (SDK_VERSION_MAJOR 0)
set (SDK_VERSION_MINOR 1)

# bring in the headers
include_directories(include)
include_directories(include/WolfSSL)

#find_package(PkgConfig REQUIRED)
#pkg_search_module(OPENSSL REQUIRED openssl)
find_package (Threads REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.c")


# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_BUILD_TYPE Release)

#find_program(iwyu_path NAMES iwyu-tool iwyu-fix-includes)
#if(NOT iwyu_path)
#  message(FATAL_ERROR "Could not find the program include-what-you-use")
#endif()

# add options for warnings
if(UNIX AND NOT APPLE)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything -fsanitize=address -fsanitize=leak -Wno-padded -Wno-zero-length-array -Wno-unused-parameter -ferror-limit=50")

	endif()
endif()
# add the executable
# add_executable(sdk-xyo ${SOURCES})


ADD_LIBRARY(sdk-xyo STATIC ${SOURCES})
target_link_libraries(sdk-xyo ${OPENSSL_LIBRARIES})
target_link_libraries (sdk-xyo ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (sdk-xyo wolfssl)

set_property(TARGET sdk-xyo PROPERTY C_INCLUDE_WHAT_YOU_USE ${iwyu_path})


INSTALL(TARGETS sdk-xyo
        DESTINATION "lib"
        )
INSTALL (
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION "include/xyo"
    FILES_MATCHING PATTERN "*.h*")
