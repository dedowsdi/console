include( ${CMAKE_SOURCE_DIR}/cmake/MyGUI.cmake  )
set(DEMO_MYGUI_SRCS
    MyguiConsoleUI.cpp	
    MyguiManager.cpp
    SdlToMygui.cpp
	)

setupMyGUI()

#FIND_LIBRARY(libMyguiOgrePlatform MyGUI.OgrePlatform)
#IF(NOT libMyguiOgrePlatform)
#MESSAGE(FATAL_ERROR "libMyguiOgre not found")
#ENDIF()

add_executable(demo_mygui ${DEMO_MYGUI_SRCS})
target_link_libraries(demo_mygui common  X11 ogreConsole ${MYGUI_LIBRARIES} ${SDL2_LIBRARIES})
include_directories(include ${OGRE_INCLUDE_DIRS} ${MYGUI_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/common )
#mygui_demo(demoMyguiConsole)
