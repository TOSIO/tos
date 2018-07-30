project(sdag)
file(GLOB sources "*.cpp")
file(GLOB headers "*.h")


add_definitions(-std=c++11)

add_library(sdag ${sources} ${headers})

include_directories("../toscore")

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(sdag PUBLIC toscore Boost::system Boost::filesystem  Boost::log Boost::thread Boost::program_options)




