cmake_minimum_required (VERSION 2.6)
project (sdk-core)
# set the version number.
set (SDK_VERSION_MAJOR 0)
set (SDK_VERSION_MINOR 1)
 
# bring in the headers
include_directories(include)

#find_package(PkgConfig REQUIRED)
#pkg_search_module(OPENSSL REQUIRED openssl)

#if( OPENSSL_FOUND )
#    include_directories(${OPENSSL_INCLUDE_DIRS})
#    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
#else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
#endif()

file(GLOB_RECURSE SOURCES "src/*.c")


# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

include_directories("${PROJECT_BINARY_DIR}")
 
set(CMAKE_BUILD_TYPE Debug)

# add options for warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")

# add the executable
#add_executable(sdk ${SOURCES})
ADD_LIBRARY(sdk-xyo STATIC ${SOURCES})


#target_link_libraries(simulation ${OPENSSL_LIBRARIES})
