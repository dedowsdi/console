set(TEST_SRCS
	include/testAbsDir.hpp
	include/testArgHandler.hpp
	include/testCmdHistory.hpp
	include/testCommand.hpp
	include/testConsole.hpp
	include/testConsolePattern.hpp
	include/testSingleton.hpp
	include/testStdUtil.hpp
	include/testStringUtil.hpp
	include/testUiConsole.hpp
	src/test.cpp)

find_package(GTest REQUIRED)
#get_target_property(CONSOLE_INCLUDES console INCLUDE_DIRECTORIES)

add_executable(test ${TEST_SRCS})
target_link_libraries(test console ${GTEST_LIBRARIES})

include_directories(include ${GTest_INCLUDE_DIRS})
