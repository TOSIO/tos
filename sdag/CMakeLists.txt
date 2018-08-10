project(sdag)
file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

file(GLOB sources1 "transaction/*.cpp")
file(GLOB headers1 "transaction/*.h")

add_definitions(-std=c++11)

add_library(sdag ${sources} ${headers} ${sources1} ${headers1})
# SET(SRC_LIST ${sources} ${headers})
# aux_source_directory(. SRC_LIST)

include_directories("../toscore")

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(sdag PUBLIC toscore Boost::system Boost::filesystem  Boost::log Boost::thread Boost::program_options)




