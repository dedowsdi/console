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
get_target_property(CONSOLE_INCLUDES console INCLUDE_DIRECTORIES)

add_executable(test ${TEST_SRCS})
target_link_libraries(test console)
target_link_libraries(test ${GTEST_LIBRARIES})

include_directories(include)
include_directories(${GTest_INCLUDE_DIRS})
