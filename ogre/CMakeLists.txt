include( ${CMAKE_SOURCE_DIR}/CMake/Dependencies/OGRE.cmake  )
set(OGRE_PLUGIN_DIR_REL /usr/local/lib/OGRE CACHE PATH "where ogre plugin lib resides")

setupOgre( OGRE_SOURCE, OGRE_BINARIES, OGRE_LIBRARIES, OGRE_INCLUDE_DIRS )
add_recursive(./src OGRECONSOLE_SRCS)
add_recursive(./include OGRECONSOLE_HEADS)


option(BUILD_DEMO_MYGUI "build mygui demo" on)

IF (APPLE)
	#FIND_LIBRARY(MYGUI MYGUI REQUIRED)
	FIND_LIBRARY(OIS OIS REQUIRED)
ELSE (APPLE) 
	IF (UNIX)
		MESSAGE(STATUS "UNIX system")
		FIND_PACKAGE(PkgConfig REQUIRED)
		IF(BUILD_DEMO_MYGUI)
			#PKG_CHECK_MODULES(MYGUI REQUIRED MYGUI)
			PKG_CHECK_MODULES(OIS REQUIRED OIS)
		ENDIF()
	ENDIF()
ENDIF (APPLE)



add_library(ogreconsole ${BUILD_TYPE} ${OGRECONSOLE_SRCS})
set_target_properties(ogreconsole PROPERTIES VERSION 0.0)

target_LINK_LIBRARIES(ogreconsole console ${OGRE_LIBRARIES})
include_directories(include ${OGRE_INCLUDE_DIRS})

set(OGRE_MEDIA_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR}/media)

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
