add_recursive(./src  CONSOLE_SRCS)
add_recursive(./include  CONSOLE_HEADS)

add_library(console ${BUILD_LIB_TYPE} ${CONSOLE_SRCS} ${CONSOLE_HEADS} )
set_target_properties(console PROPERTIES VERSION 0.0)

target_link_libraries(console ${CONSOLE_BOOST_LIBRARIES})
include_directories(include ${CONSOLE_BOOST_INCLUDE_DIRS})

#install
install(TARGETS console DESTINATION lib)
install(FILES ${CONSOLE_HEADS} DESTINATION include/pac)

# uninstall target
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
