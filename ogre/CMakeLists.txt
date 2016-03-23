set (OGRECONSOLE_SRCS
	src/OgreArgHandler.cpp
	src/OgreCommand.cpp
	src/OgreConsole.cpp
	src/OgreSI.cpp
	src/OgreSiWrapper.cpp
	src/OgreParticleSystemSI.cpp
	)

set (OGRECONSOLE_HEADS
	include/OgreArgHandler.h
	include/OgreCommand.h
	include/OgreConsole.h
	include/OgreConsolePreRequisite.h
	include/OgreSI.h
	include/OgreSiWrapper.h
	include/OgreParticleSystemSI.h
	)

option(BUILD_DEMO_MYGUI "build mygui demo" on)

IF (APPLE)
	FIND_LIBRARY(OGRE OGRE REQUIRED)
	FIND_LIBRARY(MYGUI MYGUI REQUIRED)
	FIND_LIBRARY(OIS OIS REQUIRED)
ELSE (APPLE) 
	IF (UNIX)
		MESSAGE(STATUS "UNIX system")
		FIND_PACKAGE(PkgConfig REQUIRED)
		PKG_CHECK_MODULES(OGRE REQUIRED OGRE)
		IF(BUILD_DEMO_MYGUI)
			PKG_CHECK_MODULES(MYGUI REQUIRED MYGUI)
			PKG_CHECK_MODULES(OIS REQUIRED OIS)
		ENDIF()
		MESSAGE(STATUS "OGRE:${OGRE_LIBRARIES}")
	ENDIF()
ENDIF (APPLE)

IF(BUILD_DEMO_MYGUI)
	FIND_LIBRARY(libMyguiOgrePlatform MyGUI.OgrePlatform)
	IF(NOT libMyguiOgrePlatform)
		MESSAGE(FATAL_ERROR "libMyguiOgre not found")
	ENDIF()
ENDIF()


add_library(ogreconsole ${BUILD_TYPE} ${OGRECONSOLE_SRCS})
set_target_properties(ogreconsole PROPERTIES VERSION 0.0)

target_LINK_LIBRARIES(ogreconsole console ${OGRE_LIBRARIES})
include_directories(include ${OGRE_INCLUDE_DIRS})

set(OGRE_MEDIA_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR}/media)
set(OGRE_PLUGIN_DIR_REL /usr/local/lib/OGRE CACHE PATH "where ogre plugin lib resides")

#test
if(BUILD_TEST)
	add_subdirectory(test)
endif()

#install
install(TARGETS ogreconsole DESTINATION lib)
install(FILES ${OGRECONSOLE_HEADS} DESTINATION include/ogreconsole)

IF(BUILD_DEMO_MYGUI)
	add_subdirectory(demo)
ENDIF()
