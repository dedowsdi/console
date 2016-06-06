cmake_minimum_required(VERSION 2.8.12)
project(CONSOLE)

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG"  )

if( CMAKE_BUILD_TYPE STREQUAL ""  )
	set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "One of: Debug Release
	RelWithDebInfo MinSizeRel." FORCE  )
endif()

#options
set(BUILD_LIB_TYPE "SHARED" CACHE STRING "library build type")
set_property(CACHE BUILD_LIB_TYPE PROPERTY STRINGS "STATIC" "SHARED")
option(BOOST_LOG_DYN_LINK "use shared boost log" on)
option(BUILD_TEST "build unit test" on)
option(BUILD_OGRE "build ogre module" on)

#flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Wno-unused-local-typedef -Wno-overloaded-virtual -Wno-inconsistent-missing-override")
if(BOOST_LOG_DYN_LINK)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK")
endif()

#heads and srcs
macro( add_recursive dir retVal  )
	file( GLOB_RECURSE ${retVal} ${dir}/*.h ${dir}/*.cpp ${dir}/*.c  ${dir}/*.hpp )
endmacro()

add_recursive(./src  CONSOLE_SRCS)
add_recursive(./include  CONSOLE_HEADS)

#third lib
find_package(Boost REQUIRED COMPONENTS regex log log_setup system thread)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#target
add_library(console ${BUILD_LIB_TYPE} ${CONSOLE_SRCS} ${CONSOLE_HEADS} )
set_target_properties(console PROPERTIES VERSION 0.0)

target_link_libraries(console ${Boost_LIBRARIES})
include_directories(include ${Boost_INCLUDE_DIRS})

#install
install(TARGETS console DESTINATION lib)
install(FILES ${CONSOLE_HEADS} DESTINATION include/pac)

# uninstall target
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

#test
if(BUILD_TEST)
	add_subdirectory(test)
endif()
#ogre console
if(BUILD_OGRE)
	add_subdirectory(ogre)
endif()
