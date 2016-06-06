include( ${CMAKE_SOURCE_DIR}/cmake/MyGUI.cmake  )
set(DEMO_MYGUI_HEADS
	include/BaseMyguiApp.h
	)
set(DEMO_MYGUI_SRCS
	src/BaseMyguiApp.cpp
	src/MyguiConsoleUI.cpp
	)

setupMyGUI()

#FIND_LIBRARY(libMyguiOgrePlatform MyGUI.OgrePlatform)
#IF(NOT libMyguiOgrePlatform)
#MESSAGE(FATAL_ERROR "libMyguiOgre not found")
#ENDIF()

add_executable(myguiConsole ${DEMO_MYGUI_SRCS})
target_link_libraries(myguiConsole ogreconsole ${OGRE_SAMPLES_LIBRARIES} ${MYGUI_LIBRARIES} ${MYGUI_COMMON_LIBRARIES} ${OIS_LIBRARIES})
include_directories(include ${MYGUI_INCLUDE_DIRS})
