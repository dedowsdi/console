set(TEST_SRCS
	./testOgre.cpp
	)

add_executable(testOgreConsole ${TEST_SRCS})
target_link_libraries(testOgreConsole ogreConsole ${GTEST_LIBRARIES} common)
include_directories(include ../testConsole/include ${GTest_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS})
add_dependencies(testOgreConsole ogreConsole)
