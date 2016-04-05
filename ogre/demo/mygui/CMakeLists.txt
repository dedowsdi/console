include( ${CMAKE_SOURCE_DIR}/CMake/Dependencies/MyGUI.cmake  )
set(DEMO_MYGUI_HEADS
	include/BaseMyguiApp.h
	)
set(DEMO_MYGUI_SRCS
	src/BaseMyguiApp.cpp
	src/MyguiConsoleUI.cpp
	)

setupMyGUI(MYGUI_SOURCE, MYGUI_BINARIES, MYGUI_LIBRARIES, MYGUI_INCLUDE_DIRS)

#FIND_LIBRARY(libMyguiOgrePlatform MyGUI.OgrePlatform)
#IF(NOT libMyguiOgrePlatform)
#MESSAGE(FATAL_ERROR "libMyguiOgre not found")
#ENDIF()

add_executable(myguiConsole ${DEMO_MYGUI_SRCS})
target_link_libraries(myguiConsole ogreconsole ${MYGUI_LIBRARIES} ${OIS_LIBRARIES} )
include_directories(include ${MYGUI_INCLUDE_DIRS} ${OIS_INCLUDE_DIRS})

setupOgrePlugins()
configure_file(${CMAKE_SOURCE_DIR}/CMake/Templates/resources.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg @ONLY)
#append mygui resource
FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg.in myguiRes)
set(MYGUI_MEDIA_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR})
STRING(CONFIGURE ${myguiRes} myguiRes)
FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg ${myguiRes})
