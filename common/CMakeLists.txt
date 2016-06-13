add_recursive(./ COMMON_SRCS)

add_library(common ${COMMON_SRCS})
target_link_libraries(common ${Boost_LIBRARIES} ${OGRE_LIBRARIES} ${SDL2_LIBRARIES})
include_directories(${OGRE_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR})
