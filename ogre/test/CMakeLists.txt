set(TEST_HEADS
	include/testOgreConsole.hpp
	include/testOgreSI.hpp
	)
set(TEST_SRCS
	src/testOgre.cpp
	src/ogreTestApp.cpp
	)

find_package(GTest REQUIRED)
setupOgrePlugins()

add_executable(testOgre ${TEST_SRCS})
target_link_libraries(testOgre ogreconsole ${GTEST_LIBRARIES})
include_directories(include ../../test/include ${GTest_INCLUDE_DIRS})

configure_file(${CMAKE_SOURCE_DIR}/CMake/Templates/resources.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg @ONLY)
