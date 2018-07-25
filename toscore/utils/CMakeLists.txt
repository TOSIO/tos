file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(utils ${sources} ${headers})

target_link_libraries(utils PUBLIC Boost::filesystem Boost::system)
