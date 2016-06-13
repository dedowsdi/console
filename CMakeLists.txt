cmake_minimum_required(VERSION 2.8.12)
project(CONSOLE)

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG"  )

if( CMAKE_BUILD_TYPE STREQUAL ""  )
    set( CMAKE_BUILD_TYPE Debug CACHE STRING "One of: Debug Release
    RelWithDebInfo MinSizeRel." FORCE  )
endif()

#options
set(BUILD_LIB_TYPE "SHARED" CACHE STRING "library build type")
set_property(CACHE BUILD_LIB_TYPE PROPERTY STRINGS "STATIC" "SHARED")
option(BOOST_LOG_DYN_LINK "use shared boost log" on)
option(BUILD_TESTCONSOLE "build unit test" on)
option(BUILD_TESTOGRECONSOLE "build ogre module" on)

#flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Wno-unused-local-typedef -Wno-overloaded-virtual -Wno-inconsistent-missing-override -Wno-reorder")
if(BOOST_LOG_DYN_LINK)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK")
endif()
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D_DEBUG")
ENDIF()

#heads and srcs
macro( add_recursive dir retVal  )
    file( GLOB_RECURSE ${retVal} ${dir}/*.h ${dir}/*.cpp ${dir}/*.c  ${dir}/*.hpp )
endmacro()

#third lib
find_package(Boost REQUIRED COMPONENTS regex log log_setup system thread)
set(CONSOLE_BOOST_LIBRARIES ${Boost_LIBRARIES})
set(CONSOLE_BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include( ${CMAKE_SOURCE_DIR}/cmake/OGRE.cmake  )
set(OGRE_SDK /usr/local/ogre/ogre2.1/gcc/Debug CACHE PATH "OGRE SDK")
set(OGRE_SOURCE_DIR /usr/local/source/ogre/ogre2.1 CACHE PATH "ogre source" FORCE)

setupOgre()

add_subdirectory(console)
include_directories(console/include ogreConsole/include)
add_subdirectory(ogreConsole)
add_subdirectory(common)

IF(BUILD_TESTCONSOLE OR BUILD_TESTOGRECONSOLE)
    find_package(GTest REQUIRED)
ENDIF()

if(BUILD_TESTCONSOLE)
    add_subdirectory(testConsole)
endif()
if(BUILD_TESTOGRECONSOLE)
    add_subdirectory(testOgreConsole)
endif()

add_subdirectory(demo)
