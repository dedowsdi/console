cmake_minimum_required(VERSION 2.7)
project(CONSOLE)

#options
set(BUILD_TYPE "SHARED" CACHE STRING "library build type")
set_property(CACHE BUILD_TYPE PROPERTY STRINGS "STATIC" "SHARED")
option(BOOST_LOG_DYN_LINK "use shared boost log" ON)
option(BUILD_TEST "build unit test" on)
option(BUILD_OGRE "build ogre module" on)

#flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")
if(BOOST_LOG_DYN_LINK)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK")
endif()

#src
set (CONSOLE_SRCS 
	include/pacAbsDir.h
	include/pacArgHandler.h
	include/pacCmdHistory.h
	include/pacCommand.h
	include/pacConfig.h
	include/pacConsole.h
	include/pacConsolePattern.h
	include/pacConsolePreRequisite.h
	include/pacException.h
	include/pacIntrinsicArgHandler.h
	include/pacIntrinsicCmd.h
	include/pacLogger.h
	include/pacPlatform.h
	include/pacSingleton.h
	include/pacStable.h
	include/pacStdUtil.h
	include/pacStringInterface.h
	include/pacStringUtil.h
	include/pacUiConsole.h
	src/pacAbsDir.cpp
	src/pacArgHandler.cpp
	src/pacCmdHistory.cpp
	src/pacCommand.cpp
	src/pacConsole.cpp
	src/pacConsolePattern.cpp
	src/pacException.cpp
	src/pacIntrinsicArgHandler.cpp
	src/pacIntrinsicCmd.cpp
	src/pacLogger.cpp
	src/pacPlatform.cpp
	src/pacStdUtil.cpp
	src/pacStringInterface.cpp
	src/pacStringUtil.cpp
	src/pacUiConsole.cpp
	)

#include and lib
find_package(Boost REQUIRED COMPONENTS regex log log_setup system thread)

add_library(console ${BUILD_TYPE} ${CONSOLE_SRCS} )
set_target_properties(console PROPERTIES VERSION 0.0)

target_link_libraries(console ${Boost_LIBRARIES})

include_directories(include ${Boost_INCLUDE_DIRS})

#install
install(TARGETS console DESTINATION lib)
install(FILES 
	include/pacAbsDir.h
	include/pacArgHandler.h
	include/pacCmdHistory.h
	include/pacCommand.h
	include/pacConfig.h
	include/pacConsole.h
	include/pacConsolePattern.h
	include/pacConsolePreRequisite.h
	include/pacException.h
	include/pacIntrinsicArgHandler.h
	include/pacIntrinsicCmd.h
	include/pacLogger.h
	include/pacPlatform.h
	include/pacSingleton.h
	include/pacStable.h
	include/pacStdUtil.h
	include/pacStringInterface.h
	include/pacStringUtil.h
	include/pacUiConsole.h DESTINATION include/pac)

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

#test
if(BUILD_TEST)
	add_subdirectory(test)
endif()
if(BUILD_OGRE)
	add_subdirectory(ogre)
endif()
