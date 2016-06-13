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

add_library(ogreConsole ${BUILD_LIB_TYPE} ${OGRECONSOLE_SRCS})
set_target_properties(ogreConsole PROPERTIES VERSION 0.0)

target_LINK_LIBRARIES(ogreConsole console ${OGRE_LIBRARIES})
include_directories(include ${OGRE_INCLUDE_DIRS})

set(OGRE_MEDIA_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR}/media)

#test
if(BUILD_TEST)
	add_subdirectory(test)
endif()

#install
install(TARGETS ogreConsole DESTINATION lib)
install(FILES ${OGRECONSOLE_HEADS} DESTINATION include/ogreconsole)
