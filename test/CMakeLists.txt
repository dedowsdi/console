set(TEST_HEADS
	include/testAbsDir.hpp
	include/testArgHandler.hpp
	include/testCmdHistory.hpp
	include/testCommand.hpp
	include/testConsole.hpp
	include/testConsolePattern.hpp
	include/testSingleton.hpp
	include/testStdUtil.hpp
	include/testStringUtil.hpp
	include/testConsoleUI.hpp
	)
set(TEST_SRCS
	src/test.cpp
	)

find_package(GTest REQUIRED)
#get_target_property(CONSOLE_INCLUDES console INCLUDE_DIRECTORIES)

add_executable(testBasic ${TEST_SRCS})
#set_target_properties(test PROPERTIES DEBUG_POSTFIX _d)
include_directories(include ${GTest_INCLUDE_DIRS})
target_link_libraries(testBasic  ${GTEST_MAIN_LIBRARIES}  ${GTEST_LIBRARIES} console)
