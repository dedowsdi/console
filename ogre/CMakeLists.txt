set (OGRECONSOLE_SRCS
	include/OgreArgHandler.h
	include/OgreCommand.h
	include/OgreConsole.h
	include/OgreConsolePreRequisite.h
	include/OgreSI.h
	include/OgreSiWrapper.h
	src/OgreArgHandler.cpp
	src/OgreCommand.cpp
	src/OgreConsole.cpp
	src/OgreSI.cpp
	src/OgreSiWrapper.cpp
	)

IF (APPLE)
   FIND_LIBRARY(OGRE OGRE REQUIRED)
ELSE (APPLE) 
	IF (UNIX)
		MESSAGE(STATUS "UNIX system")
		FIND_PACKAGE(PkgConfig REQUIRED)
		PKG_CHECK_MODULES(OGRE REQUIRED OGRE)
		IF(OGRE_FOUND)
			MESSAGE(STATUS "ogre found")
		ENDIF()
	ENDIF()
ENDIF (APPLE)

LINK_DIRECTORIES(${OGRE_LIBRARIES_DIRS})
LINK_LIBRARIES(${OGRE_LIBRARIES})

message(STATUS "CONSOLE:" ${CONSOLE_INCLUDE_DIRS})
message(STATUS "OGRE:" ${OGRE_INCLUDE_DIRS})

add_library(ogreconsole ${BUILD_TYPE} ${OGRECONSOLE_SRCS})
set_target_properties(ogreconsole PROPERTIES VERSION 0.0)
include_directories(include ${OGRE_INCLUDE_DIRS})



