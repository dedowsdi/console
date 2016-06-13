set(TEST_HEADS
	./testAbsDir.h
	./testArgHandler.h
	./testCmdHistory.h
	./testCommand.h
	./testConsole.h
	./testConsolePattern.h
	./testSingleton.h
	./testStdUtil.h
	./testStringUtil.h
	./testConsoleUI.h
	)
set(TEST_SRCS
	./test.cpp
	)

#get_target_property(CONSOLE_INCLUDES console INCLUDE_DIRECTORIES)

add_executable(testConsole ${TEST_SRCS})
#set_target_properties(test PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(testConsole console ${CONSOLE_BOOST_LIBRARIES} ${GTEST_MAIN_LIBRARIES}  ${GTEST_LIBRARIES} )
