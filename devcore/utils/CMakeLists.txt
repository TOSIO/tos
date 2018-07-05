cmake_minimum_required(VERSION 3.5.1)
SET(CMAKE_CXX_FLAGS "-std=c++11 -O3")




file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")



#target_link_libraries(${Boost_LIBRARIES})
project(devcoreutil)





aux_source_directory(. DIR_SRCS)
add_executable(devcoreutil ${DIR_SRCS})
# add_library(devcoreutil ${SOURCES} ${HEADERS})
# target_include_directories(devcoreutil SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

# target_include_directories(utils PRIVATE ${UTILS_INCLUDE_DIR})
